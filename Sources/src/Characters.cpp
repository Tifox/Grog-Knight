/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing,
 *  software distributed under the License is distributed on an
 *  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, either express or implied.  See the License for the
 *  specific language governing permissions and limitations
 * under the License.
 */

/**
 * File: Characters.cpp
 * Creation: 2015-02-27 04:44
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "Characters.hpp"
#include <cstdlib>

//! Base constructor
Characters::Characters(void) {
	return ;
}

//! Main constructor
/**
 * Setting base physic, some attributes and some intern variables
 * @param name The name of the character (Enemy, Hero, etc ...)
 * @todo gotta parse the right number of slots from json for inventory
 */
Characters::Characters(std::string name) : _name(name), _isRunning(0), _isJump(0) {
	this->addAttribute("physic", "1");
	this->addAttribute("class", name);
	this->SetDensity(1.0f);
	this->SetFriction(1);
	this->SetRestitution(0.0f);
	this->SetFixedRotation(true);
	this->_orientation = RIGHT;
	this->_latOrientation = RIGHT;
	this->_canMove = true;
	this->_canAttack = true;
	this->_canJump = true;
	this->_inventory = new Inventory(3);
	this->_invincibility = false;
	this->_grounds.clear();
	this->_item = nullptr;
	this->_isAttacking = 0;
	this->_gold = 0;
	this->_isLoadingAttack = 0;
	this->_speMoveReady = 1;
	this->_hasDashed = 0;
	this->SetLayer(100);
	this->_readFile(name);
}

//! Basic destructor
Characters::~Characters(void) {
	return ;
}

//! Read the json config file
/**
 * Read a json config file, based on the name of the class
 * For more documentation on the json style, uh, just wait we write it.
 * @param name (std::string)
 */
void	Characters::_readFile(std::string name) {
	std::string			file;
	std::stringstream 	buffer;
	std::ifstream		fd;

	file = "./Resources/Elements/" + name + ".json";
	fd.open(file.c_str());
	if (!fd.is_open())
		Log::error("Can't open the file for the " +
				name + " class. (Supposed path is Resources/Elements/" + name +".json)");
	buffer << fd.rdbuf();
	this->_parseJson(buffer.str());
}

//! Parse the json into workable value
/**
 * Parse, read and stock the info in the config file
 * @param file The file (A string contain the all content of a file, not is name.)
 */
void	Characters::_parseJson(std::string file) {
	Json::Reader	read;
	Json::Value		json;
	Json::ValueIterator i, v;
	std::map<std::string, Json::Value>	tmp;

	if (!read.parse(file, json))
		Log::error("Error in json syntax :\n" + read.getFormattedErrorMessages());
	if (!strncmp("Enemies/", this->_name.c_str(), 8)) {
		if (this->_name.substr(8, json["infos"].get("name", "").asString().length()) != json["infos"].get("name", "").asString())
			Log::error("The class name is different with the name in the config file.");
	} else {
		if (json["infos"].get("name", "").asString() != this->_name)
			Log::warning("The class name is different with the name in the config file.");
	}
	this->_name = json["infos"].get("name", "").asString();
	this->_id = json["infos"].get("id", "").asInt();
	this->_size = json["infos"].get("size", "").asFloat();
	this->_maxSpeed = json["infos"].get("maxSpeed", "").asFloat();
	this->_hp = json["infos"].get("HP", "").asInt();
	if (json["infos"].get("maxHP", "").isConvertibleTo(Json::ValueType::intValue))
		this->_maxHp = json["infos"].get("maxHP", "").asInt();
	if (json["infos"].get("mana", "").isConvertibleTo(Json::ValueType::intValue))
		this->_mana = json["infos"].get("mana", "").asInt();
	if (json["infos"].get("maxMana", "").isConvertibleTo(Json::ValueType::intValue))
		this->_maxMana = json["infos"].get("maxMana", "").asInt();
	this->_hitboxType = json["infos"].get("hitboxType", "").asString();
	this->_hitbox = json["infos"].get("hitbox", "").asString();
	this->addAttribute("spritesFrame", json["infos"].get("sprites", "").asString());

	for (i = json["Actions"].begin(); i != json["Actions"].end(); i++) {
		for (v = (*i).begin(); v != (*i).end(); v++) {
			tmp[v.key().asString()] = (*v);
			this->_attr[i.key().asString()] = tmp;
			// Subcribe to the broadcasts
			if (v.key().asString() == "subscribe") {
				theSwitchboard.SubscribeTo(this, (*v).asString() + "Pressed");
				theSwitchboard.SubscribeTo(this, (*v).asString() + "Released");
				this->_subsc.push_back((*v).asString() + "Pressed");
				//this->_subsc.push_back((*v).asString() + "Released");
			}
		}
	}
}

//! Get an attribute load in the conf file.
/**
 * Get a Json::Value of a key in the config file
 * @param category The category of the attribute
 * @param: key The name of the attribute
 * @return A Json::Value, See the docs of jsoncpp for the utilisation of Json::Value
 */
Json::Value		Characters::_getAttr(std::string category, std::string key) {
	if (this->_attr.find(category) != this->_attr.end()) {
		if (this->_attr[category].find(key) != this->_attr[category].end())
			return this->_attr[category][key];
		else
			Log::warning("The key " + key + " not in the category " + category);
	} else {
		Log::warning("The category " + category + " not in the config");
	}
	return nullptr;
}

//! Set the current working category
/**
 * This function is made for gain time and money, set first the category in this functions
 * then call the function _getAttr(std::string key).
 * See _getAttr(std::string key) for more info.
 * @param category The name of the category
 * @sa Characters::_getAttr
 */
void	Characters::_setCategory(std::string category) {
	this->_category = category;
}

//! Get an attribute with a pre-set category
/**
 * Get a Json::Value of a key in the config file, with a pre-set category
 * See Characters::_setCategory for more info.
 * @param: key The name of the attribute
 * @sa Characters::_setCategory
 */
Json::Value		Characters::_getAttr(std::string key) {
	if (this->_attr[this->_category].find(key) != this->_attr[this->_category].end())
		return this->_attr[this->_category][key];
	Log::warning("The key " + key + " not in the category " + this->_category);
	return nullptr;
}

//! The Broadcast function of Characters
/**
 * Receive and redistribute broadcasts messages
 * In this function, we call also the callback function of the child.
 * For more info on this, go to Characters::callback
 * @param: m The Message object
 * @sa Characters::callback
 */
void	Characters::ReceiveMessage(Message *m) {
	std::map<std::string, std::map<std::string, Json::Value> >::iterator	i;
	std::string		name, attrName;
	int				status;

	if (m->GetMessageName() == "canMove") {
		if (this->getHP() > 0) {
			this->_canMove = 1;
			if (this->_grounds.size() > 0)
				this->AnimCallback("base");
		}
	}
	else if (m->GetMessageName() == "fullChargedAttack") {
		this->_fullChargedAttack = true;
		theSwitchboard.UnsubscribeFrom(this, "fullChargedAttack");
	}
	else if (m->GetMessageName() == "startChargeAttack") {
		this->_isLoadingAttack = 1;
		theSwitchboard.UnsubscribeFrom(this, "startChargeAttack");
	}
	else if (m->GetMessageName() == "endInvincibility") {
		theSwitchboard.UnsubscribeFrom(this, "colorDamageBlink1");
		theSwitchboard.UnsubscribeFrom(this, "colorDamageBlink2");
		this->_invincibility = false;
		if (this->_enemiesTouched.size() > 0) {
			static_cast<Hero*>(this)->_takeDamage(this->_enemiesTouched.front());
			this->_enemiesTouched.clear();
		}
		this->SetColor(1,1,1,1);
	}
	else if (m->GetMessageName() == "colorDamageBlink1") {
		if (this->_invincibility == true) {
			theSwitchboard.DeferredBroadcast(new Message("colorDamageBlink2"), 0.1f);
			this->SetColor(1,1,1,1);
		}
	}
	else if (m->GetMessageName() == "colorDamageBlink2") {
		if (this->_invincibility == true) {
			theSwitchboard.DeferredBroadcast(new Message("colorDamageBlink1"), 0.1f);
			this->SetColor(1,0,0,0.9f);
		}
	}
	else if (m->GetMessageName() == "enableAttackHitbox") {
		this->_weapon->attack(this);
	}
	else if (m->GetMessageName() == "disableAttackHitbox") {
		this->_isAttacking = false;
	}
	else if (m->GetMessageName() == "moveHeroDown") {
		if (this->_latOrientation == RIGHT && this->_canMove && this->_isAttacking == false)
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
					this->_getAttr("jump", "fallingFrame_right").asInt(),
					this->_getAttr("jump", "endFrame_right").asInt() - 3, "jump");
		else if (this->_latOrientation == LEFT && this->_canMove && this->_isAttacking == false)
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
					this->_getAttr("jump", "fallingFrame_left").asInt(),
					this->_getAttr("jump", "endFrame_left").asInt() - 3, "jump");
	}
 	else if (m->GetMessageName() == "destroyEnemy") {
		new Loot(this);
		this->_destroyEnemy();
		return;
	}
	else if (m->GetMessageName() == "setToStatic" + this->GetName()) {
		this->GetBody()->SetType(b2_staticBody);
		return;
	}
	else if (m->GetMessageName() == "equipSelectedItem") {
		std::string item;

		item = this->_inventory->equipSelectedItem();
		if (item != "") {
			if (Game::wList->checkExists(item) == 1) {
				Weapon* w = new Weapon(Game::wList->getWeapon(item));
				this->unequipWeapon();
				this->equipWeapon(w);
			} else if (Game::aList->checkExists(item) == 1) {
				Armor* w = new Armor(Game::aList->getArmor(item));
				this->unequipArmor();
				this->equipArmor(w);
			} else if (Game::rList->checkExists(item) == 1) {
				Ring* w = new Ring(Game::rList->getRing(item));
				this->unequipRing();
				this->equipRing(w);
			}
			Game::getHUD()->consumable(this->_inventory->getItems());
		}
	}
	else if (m->GetMessageName() == "cycleInventory") {
		this->_inventory->changeItemFocus();
	}
	else if (m->GetMessageName() == "dropItem") {
		new Loot(this, this->_inventory->dropSelectedItem());
	}
	else if (m->GetMessageName() == "specialMove") {
		this->_specialMove();
	}
	else if (m->GetMessageName() == "speMoveReady") {
		theSwitchboard.UnsubscribeFrom(this, "speMoveReady");
		this->_speMoveReady = 1;
	}
	else if (m->GetMessageName() == "dashEnd") {
		theSwitchboard.UnsubscribeFrom(this, "dashEnd");
		this->GetBody()->SetGravityScale(1);
		this->_canMove = 1;
		this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
	}
	else if (m->GetMessageName() == "chargeEnd") {
		theSwitchboard.UnsubscribeFrom(this, "chargeEnd");
		this->_canMove = 1;
		this->_isCharging = false;
		this->_invincibility = false;
		this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
	}
	else if (m->GetMessageName() == "stompEnd") {
		theSwitchboard.UnsubscribeFrom(this, "stompEnd");
		this->_invincibility = false;
		this->_isStomping = false;
		this->_isCharging = false;
		this->GetBody()->SetBullet(false);
	}
	else if (m->GetMessageName() == "attackReady") {
		this->_canAttack = true;
		return;
	}
	for (i = this->_attr.begin(); i != this->_attr.end(); i++) {
		attrName = this->_getAttr(i->first, "subscribe").asString();
		if (!strncmp(attrName.c_str(), m->GetMessageName().c_str(), strlen(attrName.c_str()))) {

			// Get the key status (1 = Pressed, 0 = Released)
			status = (m->GetMessageName().substr(strlen(attrName.c_str()), 7) == "Pressed" ? 1 : 0);
			if (this->_canMove == false)
				return;
			if (attrName == "forward") {
				this->_forward(status);
			} else if (attrName == "backward") {
				this->_backward(status);
			} else if (attrName == "up") {
				this->_up(status);
			} else if (attrName == "down") {
				this->_down(status);
			} else if (attrName == "jump") {
				this->_jump(status);
			} else if (attrName == "attack") {
				this->_attack(status);
			} else if (attrName == "pickupItem") {
				this->_pickupItem(status);
			}
			this->_lastAction = attrName;
			this->actionCallback(attrName, status);
		}
	}
}

//! Animation callback
/**
 * The animation callback
 * See Angel doc for more information
 * String is the std::string object used in Angel2d.
 * @param s The name of the callback
 */
void	Characters::AnimCallback(String s) {
	this->_setCategory("breath");
	if (s == "base") {
		this->_isAttacking = 0;
		if (this->_isRunning == 0 && this->_isAttacking == 0 && this->_isLoadingAttack == 0) {
			this->changeSizeTo(Vector2(1, 1));
			if (this->_latOrientation == LEFT) {
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
						this->_getAttr("beginFrame_left").asInt(),
						this->_getAttr("endFrame_left").asInt(), "base");
			} else if (this->_latOrientation == RIGHT) {
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
						this->_getAttr("beginFrame_right").asInt(),
						this->_getAttr("endFrame_right").asInt(), "base");
			} else {
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
						this->_getAttr("beginFrame_right").asInt(),
						this->_getAttr("endFrame_right").asInt(), "base");
			}
		} else if (this->_isAttacking == 0) {
			if (this->_isLoadingAttack == 0) {
				this->changeSizeTo(Vector2(1, 1));
				if (this->_isRunning == 1) {
					this->_setCategory("forward");
				} else if (this->_isRunning == 2) {
					this->_setCategory("backward");
				}
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(),
										  SAT_Loop,
										  this->_getAttr("beginFrame").asInt(),
										  this->_getAttr("endFrame").asInt());
			} else {
				std::string orientation;
				if (this->_latOrientation == RIGHT) {
					orientation = "right";
				} else if (this->_latOrientation == LEFT)
					orientation = "left";
				this->_setCategory("loadAttack_charge");
				this->changeSizeTo(Vector2(2, 2));
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
										  this->_getAttr("endFrame_" + orientation).asInt(),
										  this->_getAttr("endFrame_" + orientation).asInt(), "loadAttack_charge");
			}
		}
	}
}

//! Collision function
/**
 * Collision begin callback
 * See Angel docs for more information.
 * /!\ This function is called BEFORE a collision happened.
 * @param elem The Element who collide
 * @param contact The b2Contact object of the collision. See Box2D docs for more info.
 */
void	Characters::BeginContact(Elements *elem, b2Contact *contact) {
	if (elem->getAttributes()["type"] == "ground") {
		if (this->_isAttacking == 0 && this->_isLoadingAttack == 0)
			this->changeSizeTo(Vector2(1, 1));
		if (this->GetBody()->GetWorldCenter().y - 0.905 >= elem->GetBody()->GetWorldCenter().y) {
			if (this->_grounds.size() > 0) {
				contact->SetEnabled(false);
				this->_hasDashed = 0;
			}
			else {
				if (this->_isCharging == 1) {
					theSwitchboard.Broadcast(new Message("chargeEnd"));
				}
				this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
				if (this->_hp <= 0) {
				  this->_heroDeath();
				  return;
				}
			if (this->_isStomping == true) {
			  theSwitchboard.Broadcast(new Message("stompEnd"));
			  new Weapon(this->_weapon, this, 1);
			  new Weapon(this->_weapon, this, -1);
			}
			  this->_isJump = 0;
			  this->_hasDashed = 0;
			  if (this->_latOrientation == RIGHT && this->_isAttacking == false && this->_isLoadingAttack == 0) {
					this->changeSizeTo(Vector2(1, 1));
					this->PlaySpriteAnimation(0.1f, SAT_OneShot,
							this->_getAttr("jump", "endFrame_right").asInt() - 2,
							this->_getAttr("jump", "endFrame_right").asInt(), "base");
				} else if (this->_latOrientation == LEFT &&
					this->_isAttacking == false &&
					this->_isLoadingAttack == 0) {
					this->changeSizeTo(Vector2(1, 1));
					this->PlaySpriteAnimation(0.1f, SAT_OneShot,
							this->_getAttr("jump", "endFrame_left").asInt() - 2,
							this->_getAttr("jump", "endFrame_left").asInt(), "base");
				}
			}
			this->_grounds.push_back(elem);
		} else if (this->GetBody()->GetWorldCenter().y <= elem->GetBody()->GetWorldCenter().y - 0.905) {
		  this->_ceiling.push_back(elem);
		} else if (this->GetBody()->GetWorldCenter().x >= elem->GetBody()->GetWorldCenter().x) {
			if (this->_isCharging == 1) {
				theSwitchboard.Broadcast(new Message("chargeEnd"));
			}
			if (this->_wallsLeft.size() > 0)
				contact->SetEnabled(false);
			else {
				if (this->_speMove == "wallJump" && this->_grounds.size() == 0) {
					this->GetBody()->SetGravityScale(0.3);
				}
			}
			this->_wallsLeft.push_back(elem);
		} else if (this->GetBody()->GetWorldCenter().x < elem->GetBody()->GetWorldCenter().x) {
				if (this->_isCharging == 1) {
					theSwitchboard.Broadcast(new Message("chargeEnd"));
				}
			if (this->_wallsRight.size() > 0)
				contact->SetEnabled(false);
			else {
				if (this->_speMove == "wallJump" && this->_grounds.size() == 0) {
					this->GetBody()->SetGravityScale(0.3);
				}
			}
			this->_wallsRight.push_back(elem);
		}
		if (elem->getAttribute("speType") == "canCross") {
			if (this->GetBody()->GetWorldCenter().y - 1 <
				elem->GetBody()->GetWorldCenter().y) {
				contact->SetEnabled(false);
				contact->enableContact = false;
			}
			else if (this->getOrientation() == Characters::DOWN) {
				contact->SetEnabled(false);
				contact->enableContact = false;
			}
		}
		if (elem->getAttribute("type") == "Object") {
			contact->SetEnabled(false);
			contact->enableContact = false;
		}
	}
}

//! End collision function
/**
 * Collision end callback
 * /!\ This function is called AFTER the elements leave another
 * @param elem The Element who left collision
 * @param: contact The b2Contact object of the collided element. See Box2D docs for more info.
 */
void	Characters::EndContact(Elements *elem, b2Contact *contact) {
	if (elem->getAttributes()["type"] == "ground") {
		this->_wallsLeft.remove(elem);
		this->_wallsRight.remove(elem);
		this->_ceiling.remove(elem);
		if (this->_grounds.size() == 1) {
			this->_grounds.remove(elem);
			if (this->_grounds.size() == 0) {
				this->_isJump++;
				if (this->_lastAction == "forward" && this->_canMove &&
					this->_isAttacking == false && this->_isLoadingAttack == 0) {
					this->changeSizeTo(Vector2(1, 1));
					this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
							this->_getAttr("jump", "fallingFrame_right").asInt(),
							this->_getAttr("jump", "endFrame_right").asInt() - 3, "jump");
				} else if (this->_lastAction == "backward" && this->_canMove &&
						   this->_isAttacking == false && this->_isLoadingAttack == 0) {
					this->changeSizeTo(Vector2(1, 1));
					this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
											  this->_getAttr("jump", "fallingFrame_left").asInt(),
											  this->_getAttr("jump", "endFrame_left").asInt() - 3, "jump");
				}
			}
		}
		else
			this->_grounds.remove(elem);
		if (this->_wallsLeft.size() == 0 && this->_wallsRight.size() == 0 && this->_speMove == "wallJump")
			this->GetBody()->SetGravityScale(1);
	}
}

//! Intern callbacks for moving
/**
 * Intern callbacks for making a character move.
 * This function handle the direction of the character.
 * This callback is called in Game::makeItRun, so you probalby don't want to call it.
 * @sa Game::makeItRun
 */
void	Characters::_run(void) {
	if (this->_isRunning == 1)
		this->_forward(2);
	else if (this->_isRunning == 2)
		this->_backward(2);
}

/****************************/
/*                          */
/*          ACTIONS         */
/*                          */
/****************************/

//! Forward action
/**
 * Making a Characters run forward.
 * This function handle the power size for moving, sprite animation, basicly everything.
 * So, if you want to make some modification, use the intern callback (Characters::callback), or override 
 * this function in the children.
 * But, please, DO NOT modify this one.
 * @param status The status of the key (0 | 1)
 */
void	Characters::_forward(int status) {
	this->_setCategory("forward");
	if (status == 1) {
		this->_orientation = RIGHT;
		this->_latOrientation = RIGHT;
		if ((this->GetSpriteFrame() < this->_getAttr("beginFrame").asInt() ||
					(this->GetSpriteFrame() >= this->_getAttr("backward", "beginFrame").asInt() &&
					 this->GetSpriteFrame() <= this->_getAttr("backward", "endFrame").asInt()))  &&
			!this->_isJump && !this->_isLoadingAttack)
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
					this->_getAttr("beginFrame").asInt(), this->_getAttr("endFrame").asInt());
		else if (this->_isJump && !this->_isLoadingAttack) {
			this->_setCategory("jump");
			if (this->GetSpriteFrame() >= this->_getAttr("beginFrame_left").asInt() && this->GetSpriteFrame() <= this->_getAttr("endFrame_left").asInt())
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
						this->GetSpriteFrame() - 8,
						this->_getAttr("endFrame_right").asInt() - 3, "jump");
			this->_setCategory("forward");
		} else if (this->_isLoadingAttack) {
			this->_setCategory("loadAttack_charge");
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
									  this->_getAttr("endFrame_right").asInt(),
									  this->_getAttr("endFrame_right").asInt(), "loadAttack_charge");
			this->_setCategory("forward");
		}
		Game::startRunning(this);
		if (this->_isRunning == 2)
			this->GetBody()->SetLinearVelocity(b2Vec2(this->_getAttr("force").asFloat(), this->GetBody()->GetLinearVelocity().y));
		this->_isRunning = 1;
	} else if (status == 0 && this->_latOrientation == RIGHT) {
		this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
		Game::stopRunning(this);
		this->_isRunning = 0;
		if (!this->_isJump && !this->_isAttacking)
			this->AnimCallback("base");
	} else {
			if (this->_wallsRight.size() == 0 && this->_canMove == true)
				this->GetBody()->SetLinearVelocity(b2Vec2(this->_getAttr("force").asFloat(), this->GetBody()->GetLinearVelocity().y));
	}
	return ;
}

//! Backward action
/**
 * Make the character go backward
 * Same as forward.
 * @param status The status of the key (1 | 0)
 * @sa Characters::_forward
 */
void	Characters::_backward(int status) {
	this->_setCategory("backward");
	if (status == 1) {
		this->_orientation = LEFT;
		this->_latOrientation = LEFT;
		if (this->GetSpriteFrame() < this->_getAttr("beginFrame").asInt() &&
			!this->_isJump && !this->_isLoadingAttack)
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
					this->_getAttr("beginFrame").asInt(),
					this->_getAttr("endFrame").asInt());
		else if (this->_isJump && !this->_isLoadingAttack) {
			this->_setCategory("jump");
			if (this->GetSpriteFrame() >= this->_getAttr("beginFrame_right").asInt() && this->GetSpriteFrame() <= this->_getAttr("endFrame_right").asInt())
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
						this->GetSpriteFrame() + 8,
						this->_getAttr("endFrame_left").asInt() - 3, "jump");
			this->_setCategory("backward");
		} else if (this->_isLoadingAttack) {
			this->_setCategory("loadAttack_charge");
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
									  this->_getAttr("endFrame_left").asInt(),
									  this->_getAttr("endFrame_left").asInt(), "loadAttack_charge");
			this->_setCategory("backward");
		}
		Game::startRunning(this);
		if (this->_isRunning == 1)
			this->GetBody()->SetLinearVelocity(b2Vec2(-this->_getAttr("force").asFloat(), this->GetBody()->GetLinearVelocity().y));
		this->_isRunning = 2;
	} else if (status == 0 && this->_latOrientation == LEFT) {
		this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
		Game::stopRunning(this);
		this->_isRunning = 0;
		if (!this->_isJump && !this->_isAttacking)
			this->AnimCallback("base");
	} else {
			if (this->_wallsLeft.size() == 0 && this->_canMove == true)
				this->GetBody()->SetLinearVelocity(b2Vec2(-this->_getAttr("force").asFloat(), this->GetBody()->GetLinearVelocity().y));
	}
	return ;
}

//! Jump action
/**
 * Make the character jump.
 * Same as forward.
 * @param: status The key status (1 | 0)
 * @sa Characters::_forward
 */
void	Characters::_jump(int status) {
	this->_setCategory("jump");

	if (status == 1 && this->_canJump == true) {
		this->_canJump = false;
		if (this->_speMove == "wallJump" && this->_grounds.size() == 0 &&
			(this->_wallsLeft.size() > 0 || this->_wallsRight.size() > 0)) {
			this->GetBody()->SetGravityScale(1);
			if (this->_wallsLeft.size() > 0 && this->_wallsRight.size() == 0) {
				this->GetBody()->SetLinearVelocity(b2Vec2(5, this->_getAttr("rejump").asFloat()));
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
										  this->_getAttr("beginFrame_right").asInt(),
										  this->_getAttr("endFrame_right").asInt() - 3, "jump");
			}
			else if (this->_wallsRight.size() > 0 && this->_wallsLeft.size() == 0) {
				this->GetBody()->SetLinearVelocity(b2Vec2(-5, this->_getAttr("rejump").asFloat()));
					this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
											  this->_getAttr("beginFrame_left").asInt(),
											  this->_getAttr("endFrame_left").asInt() - 3, "jump");
			}
			else
				this->GetBody()->SetLinearVelocity(b2Vec2(0, this->_getAttr("rejump").asFloat()));
			Game::stopRunning(this);
		}
		else if (this->_isJump == 0 || (this->_isJump < this->_getAttr("max").asInt())) {
			if (this->_isJump >= 1) {
				this->GetBody()->SetLinearVelocity(b2Vec2(this->GetBody()->GetLinearVelocity().x,
														  this->_getAttr("rejump").asFloat()));
			}
			else {
				this->ApplyLinearImpulse(Vector2(0, this->_getAttr("force").asFloat()), Vector2(0, 0));
			}
			if (this->_isAttacking == false && this->_isLoadingAttack == 0) {
				this->changeSizeTo(Vector2(1, 1));
				if (this->_latOrientation == RIGHT) {
					this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
							this->_getAttr("beginFrame_right").asInt(),
							this->_getAttr("endFrame_right").asInt() - 3, "jump");
				}
				else if (this->_latOrientation == LEFT) {
					this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
							this->_getAttr("beginFrame_left").asInt(),
							this->_getAttr("endFrame_left").asInt() - 3, "jump");
				}
			}
			if (this->_grounds.size() == 0)
				this->_isJump++;
		}
	}
	else if (status == 0) {
		this->_canJump = true;
	}
	return ;
}

//! Up button action
/**
 * Called when pressing the UP button
 * This function handle just the Orientation.
 * @param status The key status (1 | 0)
 */

void	Characters::_up(int status) {
	if (status == 1)
		this->_orientation = UP;
	else
		this->_orientation = this->_latOrientation;
}

//! Down button action
/**
 * Called when pressing the DOWN button
 * Same as Characters::_up
 * @param status The key status (1 | 0)
 * @sa Characters::_up
 */

void	Characters::_down(int status) {
	if (status == 1) {
		this->_orientation = DOWN;
		if (this->_grounds.size() > 0) {
			std::list<Elements*>::iterator it;
			for (it = this->_grounds.begin(); it != this->_grounds.end(); it++) {
				if ((*it)->getAttribute("speType") != "canCross")
					return;
			}
			theSwitchboard.SubscribeTo(this, "moveHeroDown");
			theSwitchboard.DeferredBroadcast(new Message("moveHeroDown"), 0.1);
			b2PolygonShape box = Game::hList->getHitbox(this->_hitbox);
			b2Shape *shape = &box;
			this->GetBody()->DestroyFixture(this->GetBody()->GetFixtureList());
			this->GetBody()->CreateFixture(shape, 1);
		}
	}
	else
		this->_orientation = this->_latOrientation;
}

//! Attack action
/**
 * Attack action
 * Call the weapon attack() function.
 * @param status The key status (1 | 0)
 * @sa Weapon::attack
 */
void	Characters::_attack(int status) {
	if (status == 1 && this->_canAttack == true) {
		if (!this->_attackPressed && !this->_isLoadingAttack) {
			this->_attackPressed = 1;
			theSwitchboard.SubscribeTo(this, "startChargeAttack");
			theSwitchboard.DeferredBroadcast(new Message("startChargeAttack"),
											 0.3f);
		}
		if (this->_isLoadingAttack == 1 && this->_attackPressed == 1) {
			this->_attackPressed = 0;
			this->_fullChargedAttack = false;
			theSwitchboard.SubscribeTo(this, "fullChargedAttack");
			theSwitchboard.DeferredBroadcast(new Message("fullChargedAttack"),
											 0.5f);
			this->actionCallback("loadAttack_charge", 1);
		}
	}
	else if (status == 0 && this->_canAttack == true && (this->_fullChargedAttack == true || this->_isLoadingAttack == 0))  {

		theSwitchboard.UnsubscribeFrom(this, "fullChargedAttack");
		theSwitchboard.UnsubscribeFrom(this, "startChargeAttack");
		this->_isAttacking = 1;
		this->_isLoadingAttack = 0;
		this->_attackPressed = 0;
		theSwitchboard.DeferredBroadcast(new Message("enableAttackHitbox"),
										 0.05f);
	} else if (status == 0) {
		theSwitchboard.UnsubscribeFrom(this, "fullChargedAttack");
		theSwitchboard.UnsubscribeFrom(this, "startChargeAttack");
		this->_isLoadingAttack = 0;
		this->_attackPressed = 0;
		this->AnimCallback("base");
	}
}

//! Pick an item
/**
 * Pick an item on the floor
 * Replace the current relatable item by the new one.
 * @todo Making throw the old item.
 * @param status The key status (1 | 0)
 */
void	Characters::_pickupItem(int status) {
	if (this->_item == nullptr)
		return;
	if (this->_inventory->addItemToInventory(this->_item->getAttribute("name")) == 1) {
		if (Game::wList->checkExists(this->_inventory->getCurrentFocus()))
			new Loot(this, Game::wList->getWeapon(this->_inventory->dropSelectedItem()));
		else if (Game::aList->checkExists(this->_inventory->getCurrentFocus()))
			new Loot(this, Game::aList->getArmor(this->_inventory->dropSelectedItem()));
		else if (Game::rList->checkExists(this->_inventory->getCurrentFocus()))
			new Loot(this, Game::rList->getRing(this->_inventory->dropSelectedItem()));
		else
			Log::error("An error occured trying to drop " + this->_inventory->getCurrentFocus());
		this->_inventory->addItemToInventory(this->_item->getAttribute("name"));
	}
	theSwitchboard.Broadcast(new Message("DeleteEquipment" +
										 this->_item->GetName()));
	this->_item = nullptr;

}

//! Executes special move
/**
 * Executes the characters special move, based on which one he picked up at the beginning
 * @todo should not be based off the json config
 */

void	Characters::_specialMove(void) {
	if (this->_speMove == "dash")
		this->_dash();
	else if (this->_speMove == "charge")
		this->_charge();
	else if (this->_speMove == "stomp")
		this->_stomp();
	else if (this->_speMove == "blink")
		this->_blink();
}

//! Special move: dash
/**
 * Character executes a dash if the cooldown is up and the conditions allows it
 * Properties of dash - no gravity, take damage, cant move
 * @sa Characters::_specialMove()
 */

void	Characters::_dash(void) {
	this->_setCategory("dash");
	if (this->_isAttacking == 0 && this->_canMove == 1 && this->_hasDashed == 0) {
		this->GetBody()->SetGravityScale(0);
		this->_canMove = 0;
		if (this->_grounds.size() == 0)
			this->_hasDashed = 1;
		theSwitchboard.SubscribeTo(this, "dashEnd");
		theSwitchboard.DeferredBroadcast(new Message("dashEnd"),
										 this->_getAttr("uptime").asFloat());
		if (this->_latOrientation == LEFT)
			this->GetBody()->SetLinearVelocity(b2Vec2(-this->_getAttr("dashSpeed").asInt(), 0));
		else if (this->_latOrientation == RIGHT)
			this->GetBody()->SetLinearVelocity(b2Vec2(this->_getAttr("dashSpeed").asInt(), 0));
	}
}

//! Special move: charge
/**
 * Character executes a charge if the cooldown is up and the conditions allows it
 * Properties of charge - invincibility, can move
 * @sa Characters::_specialMove()
 */

void	Characters::_charge(void) {
	this->_setCategory("charge");
	if (this->_isAttacking == 0 && this->_canMove == 1 && this->_speMoveReady == 1 && this->_grounds.size() > 0) {
		this->_speMoveReady = 0;
		this->_invincibility = true;
		this->_isCharging = true;
		this->_canMove = 0;
		theSwitchboard.SubscribeTo(this, "speMoveReady");
		theSwitchboard.SubscribeTo(this, "chargeEnd");
		theSwitchboard.DeferredBroadcast(new Message("speMoveReady"),
										 this->_getAttr("cooldown").asFloat());
		theSwitchboard.DeferredBroadcast(new Message("chargeEnd"),
										 this->_getAttr("uptime").asFloat());
		if (this->_latOrientation == LEFT)
			this->GetBody()->SetLinearVelocity(b2Vec2(-this->_getAttr("chargeSpeed").asInt(), 0));
		else if (this->_latOrientation == RIGHT)
			this->GetBody()->SetLinearVelocity(b2Vec2(this->_getAttr("chargeSpeed").asInt(), 0));
	}
}

//! Special move: stomp
/**
 * If airborne, will allow you to slam the ground and deal damage to enemies
 * Properties of stomp - invincibility, can move
 * @sa Characters::_specialMove()
 */

void	Characters::_stomp(void) {
	this->_setCategory("stomp");
	if (this->_isAttacking == 0 && this->_canMove == 1 && this->_speMoveReady == 1
		&& this->_grounds.size() == 0) {
		this->_speMoveReady = 0;
		this->GetBody()->SetBullet(true);
		this->_invincibility = true;
		this->_isStomping = true;
		this->_isCharging = true;
		theSwitchboard.SubscribeTo(this, "speMoveReady");
		theSwitchboard.SubscribeTo(this, "stompEnd");
		theSwitchboard.DeferredBroadcast(new Message("speMoveReady"),
										 this->_getAttr("cooldown").asFloat());
		this->GetBody()->SetLinearVelocity(b2Vec2(0, -this->_getAttr("stompSpeed").asInt()));
	}
}


//! Special move: blink
/**
 * Teleports the player in the direction he's facing
 * Properties of blink - instant
 * @sa Characters::_specialMove()
 */

void	Characters::_blink(void) {
	this->_setCategory("blink");
	Map m = Game::currentGame->maps->getMapXY()[Game::currentY][Game::currentX];
	int x = (this->GetBody()->GetWorldCenter().x) - m.getXStart();
	int y = -((this->GetBody()->GetWorldCenter().y) - m.getYStart());
	int range = this->_getAttr("blinkRange").asInt();
	std::vector<std::vector<int>> t = m.getPhysicMap();

	if (this->_isAttacking == 0 && this->_canMove == 1 && this->_speMoveReady == 1) {
		this->_speMoveReady = 0;
		theSwitchboard.SubscribeTo(this, "speMoveReady");
		theSwitchboard.DeferredBroadcast(new Message("speMoveReady"),
										 this->_getAttr("cooldown").asFloat());
		if (this->_orientation == UP) {
			while (range > 0) {
				if (y - range > 0 && !t[y - range][x])
					break;
				range--;
			}
			if (range > 0)
				this->GetBody()->SetTransform(b2Vec2(this->GetBody()->GetWorldCenter().x,
												 this->GetBody()->GetWorldCenter().y + range), 0);
		}
		else if (this->_orientation == DOWN) {
			while (range > 0) {
				if (y + range < (t.size() - 2) && !t[y + range][x])
					break;
				range--;
			}
			range--;
			if (range > 0)
				this->GetBody()->SetTransform(b2Vec2(this->GetBody()->GetWorldCenter().x,
												 this->GetBody()->GetWorldCenter().y - range), 0);
		}
		else if (this->_orientation == RIGHT) {
			while (range > 0) {
				if (x + range < t[y].size() && !t[y][x + range])
					break;
				range--;
			}
			if (range > 0)
				this->GetBody()->SetTransform(b2Vec2(this->GetBody()->GetWorldCenter().x + range,
												 this->GetBody()->GetWorldCenter().y), 0);
		}
		else if (this->_orientation == LEFT) {
			while (range > 0) {
				if (x - range > 0 && !t[y][x - range])
					break;
				range--;
			}
			if (range > 0)
				this->GetBody()->SetTransform(b2Vec2(this->GetBody()->GetWorldCenter().x - range,
												 this->GetBody()->GetWorldCenter().y), 0);
		}
		if (range > 0) {
			b2PolygonShape box = Game::hList->getHitbox(this->_hitbox);
			b2Shape *shape = &box;
			this->GetBody()->DestroyFixture(this->GetBody()->GetFixtureList());
			this->GetBody()->CreateFixture(shape, 1);
		}
	}
}

//! Equip a weapon
/**
 * Equip a new weapon to the Character, and update the HUD.
 * @param The Weapon object
 */
void	Characters::equipWeapon(Weapon* weapon) {
	this->_weapon = new Weapon(weapon);
	Game::getHUD()->items(this->_weapon);
}

//! Unequip a weapon
/**
 * Unequip old weapon to the Character, and update the HUD.
 * @param The Weapon object
 */
void	Characters::unequipWeapon(void) {
	this->_inventory->swapEquipmentAndInventory(this->_weapon->getAttribute("name"));
}

//! Equip a armor
/**
 * Equip a new armor to the Character, and update the HUD.
 * @param The armor object
 */
void	Characters::equipArmor(Armor* armor) {
	this->_armor = new Armor(armor);
	if (this->_armor->getAttribute("hpBuff") != ""){
	  	this->_maxHp += std::stoi(this->_armor->getAttribute("hpBuff"));
	  	this->_hp += std::stoi(this->_armor->getAttribute("hpBuff"));
	}
	if (this->_armor->getAttribute("manaBuff") != ""){
	  	this->_maxMana += std::stoi(this->_armor->getAttribute("manaBuff"));
	  	this->_mana += std::stoi(this->_armor->getAttribute("manaBuff"));
	}
	Game::getHUD()->items(this->_armor);
	Game::getHUD()->life(this->_maxHp);
	Game::getHUD()->mana(this->_maxMana);

}

//! Unequip a armor
/**
 * Unequip old armor to the Character, and update the HUD.
 * @param void
 */
void	Characters::unequipArmor(void) {
	this->_inventory->swapEquipmentAndInventory(this->_armor->getAttribute("name"));
	if (this->_armor->getAttribute("hpBuff") != "")
	  	this->_maxHp -= std::stoi(this->_armor->getAttribute("hpBuff"));
	if (this->_armor->getAttribute("manaBuff") != "")
	  	this->_maxMana -= std::stoi(this->_armor->getAttribute("manaBuff"));
}

//! Equip a ring
/**
 * Equip a new ring to the Character, and update the HUD.
 * @param The ring object
 */
void	Characters::equipRing(Ring* ring) {
	this->_ring = new Ring(ring);
	if (this->_ring->getAttribute("hpBuff") != "")
	  	this->_maxHp += std::stoi(this->_ring->getAttribute("hpBuff"));
	if (this->_ring->getAttribute("manaBuff") != "")
	  	this->_maxMana += std::stoi(this->_ring->getAttribute("manaBuff"));
	Game::getHUD()->items(this->_ring);
	Game::getHUD()->life(this->_maxHp);
	Game::getHUD()->mana(this->_maxMana);
}

//! Unequip a ring
/**
 * Unequip old ring to the Character, and update the HUD.
 * @param The ring object
 */
void	Characters::unequipRing(void) {
	this->_inventory->swapEquipmentAndInventory(this->_ring->getAttribute("name"));
	if (this->_ring->getAttribute("hpBuff") != "")
	  	this->_maxHp -= std::stoi(this->_ring->getAttribute("hpBuff"));
	if (this->_ring->getAttribute("manaBuff") != "")
	  	this->_maxMana -= std::stoi(this->_ring->getAttribute("manaBuff"));
}

//! Set basics hp
/**
 * Set HP to the Character.
 * @param hp The HP number
 */
void						Characters::setHP(int hp) {
	if (hp > this->_maxHp)
		this->_hp = this->_maxHp;
	else
		this->_hp = hp;
};

//! Set basics mana
/**
 * Set Mana to the Character.
 * @param mana The Mana number
 */
void						Characters::setMana(int mana) {
	if (mana > this->_maxMana)
		this->_mana = this->_maxMana;
	else
		this->_mana = mana;
};

//! Destroy an Enemy.
/**
 * Made theSwitchboard Unsubscribe from the object, and destroy it.
 */
void						Characters::_destroyEnemy(void) {
	theSwitchboard.UnsubscribeFrom(this, "destroyEnemy");
	Game::addToDestroyList(this);
}

//! Animation for the Hero death
/**
 * A beautiful for the Hero death.
 * Make things black, ang get a beer.
 */
void						Characters::_heroDeath(void) {
	this->_setCategory("death");
	this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
			this->_getAttr("beginFrame_right").asInt(),
			this->_getAttr("endFrame_right").asInt(), "death");
	Game::endGame = true;
	Actor* ghost = new Actor();
	ghost->SetPosition(this->GetBody()->GetWorldCenter().x, this->GetBody()->GetWorldCenter().y - 1.7f);
	ghost->SetSprite("Resources/Images/Ghost/ghost_000.png", 0);
	ghost->LoadSpriteFrames("Resources/Images/Ghost/ghost_000.png");
	ghost->MoveTo(Vector2(this->GetBody()->GetWorldCenter().x, this->GetBody()->GetWorldCenter().y + 7), 4);
	theWorld.Add(ghost);
	ghost->PlaySpriteAnimation(0.2f, SAT_OneShot, 0, 10, "ghost");
}

/**
 * Unsubscribe the character to all the broadcasts
 */
void						Characters::unsubscribeFromAll(void) {
	std::list<std::string>::iterator	it;

	for (it = this->_subsc.begin(); it != this->_subsc.end(); it++) {
		theSwitchboard.UnsubscribeFrom(this, *it);
	}
}

/**
 * Subscribe the character to all the broadcasts
 */
void						Characters::subscribeToAll(void) {
	std::list<std::string>::iterator	it;

	for (it = this->_subsc.begin(); it != this->_subsc.end(); it++) {
		theSwitchboard.SubscribeTo(this, *it);
	}
}

/* GETTERS */
Characters::Orientation		Characters::getOrientation(void) { return this->_orientation; }
std::string					Characters::getLastAction(void) { return this->_lastAction; };
Elements*					Characters::getItem(void) { return this->_item; }
int							Characters::getHP(void) { return this->_hp; };
int							Characters::getMana(void) { return this->_mana; };
int							Characters::getMaxMana(void) { return this->_maxMana; };
int							Characters::getMaxHP(void) { return this->_maxHp; };
int							Characters::getGold(void) { return this->_gold; };
void						Characters::changeCanMove(void) { this->_canMove = (this->_canMove ? false : true); };
Weapon						*Characters::getWeapon(void) { return this->_weapon; };
bool						Characters::getCharging(void) { return this->_isCharging; }
std::list<std::string>		Characters::getSubscribes(void) { return this->_subsc; };
