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
# include <cstdlib>


//! Base constructor
Characters::Characters(void) : _level(1) {
	return ;
}


//! Main constructor
/**
 * Setting base physic, some attributes and some intern variables
 * @param name The name of the character (Enemy, Hero, etc ...)
 * @todo gotta parse the right number of slots from json for inventory
 */
Characters::Characters(std::string name) : _name(name), _isRunning(0), _isJump(0), _level(1) {
	this->addAttribute("physic", "1");
	this->addAttribute("class", name);
	this->SetDensity(1.0f);
	this->SetName(name);
	this->inSpecialMap = false;
	this->SetFriction(1);
	this->SetRestitution(0.0f);
	this->SetFixedRotation(true);
	this->_execFlag = 0;
	this->_orientation = RIGHT;
	this->_latOrientation = RIGHT;
	this->_canMove = true;
	this->_canAttack = true;
	this->_canJump = true;
	this->_isFlying = false;
	this->_isTouchingBossDoor = false;
	this->_isTouchingSecretDoor = false;
	this->_isTouchingChest = false;
	this->_isTouchingDealer = false;
	this->_isChoosingItem = 0;
	this->_invincibility = false;
	this->_grounds.clear();
	this->_speMoveIsSet = false;
	this->_item = nullptr;
	this->_shopItem = "";
	this->_drug = "";
	this->_totem = nullptr;
	this->_isAttacking = 0;
	this->_totemPlaced = 0;
	this->_target = nullptr;
	this->_isLoadingAttack = 0;
	this->_isStomping = 0;
	this->_isCharging = 0;
	this->_isDashing = 0;
	this->_isWhirlwinding = false;
	this->_isShockWaving = false;
	this->_isRapidFiring = false;
	this->_speMoveReady = 1;
	this->_speAttReady = 1;
	this->_hasDashed = 0;
	this->SetLayer(100);
	this->_flyTrigger = false;
	this->_isDisengaging = false;
	this->buff.cur = "";
	this->buff.bonusDmg = 0;
	this->buff.bonusSpeed = 0;
	this->buff.drugSpeed = 0;
	this->buff.dmgReduc = 0;
	this->buff.critBuff = 0;
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
	std::string							type;

	if (!read.parse(file, json))
		Log::error("Error in json syntax :\n" + read.getFormattedErrorMessages());
	if (!strncmp("Enemies/", this->_name.c_str(), 8)) {
		if (this->_name.substr(8, json["infos"].get("name", "").asString().length()) != json["infos"].get("name", "").asString())
			Log::error("The class name is different with the name in the config file (" + this->_name + ").");
	} else {
		if (json["infos"].get("name", "").asString() != this->_name)
			Log::warning("The class name is different with the name in the config file.");
	}
	if (json["infos"].get("type", "").isConvertibleTo(Json::ValueType::stringValue))
		type = json["infos"].get("type", "").asString();
	this->_name = json["infos"].get("name", "").asString();
	this->_talk = json["infos"].get("talk", "").asString();
	this->addAttribute("talk", json["infos"].get("talk", "").asString());
	this->_size = json["infos"].get("size", "").asFloat();
	this->SetSize(this->_size);
	if (json["infos"].get("maxHP", "").isConvertibleTo(Json::ValueType::intValue))
		this->_maxHp = json["infos"].get("maxHP", "").asInt();
	this->_hitboxType = json["infos"].get("hitboxType", "").asString();
	this->_hitbox = json["infos"].get("hitbox", "").asString();
	this->addAttribute("spritesFrame", json["infos"].get("sprites", "").asString());
	this->_hp = this->_maxHp;
	if (type != "Boss") {
		this->_id = json["infos"].get("id", "").asInt();
		if (json["infos"].get("damage", "").isConvertibleTo(Json::ValueType::stringValue))
			this->addAttribute("damage", json["infos"].get("damage", "").asString());
		this->buff.critBuff = json["infos"].get("baseCrit", 0).asInt();
		this->buff.bonusDmg = json["infos"].get("baseDamage", 0).asInt();
		this->_maxHp += (this->_maxHp * json["Stats"].get("HPMult", 0).asFloat()) * this->_level;
		this->buff.critBuff += (this->buff.critBuff * json["Stats"].get("critMult", 0).asFloat() * this->_level);
		this->buff.bonusDmg += (this->buff.bonusDmg * json["Stats"].get("damageMult", 0).asFloat() * this->_level);
	}

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

//! Loop function called on every frame.
/**
 * Used to manage every frame based action on characters
 */
void	Characters::characterLoop(void) {
	this->_tryFly();
	this->_resetBroadcastFlags();
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
			this->_isLoadingAttack = 0;
			this->_isAttacking = 0;
			this->_fullChargedAttack = 0;
			if (this->_grounds.size() > 0)
				this->AnimCallback("base");
		}
	}
	else if (m->GetMessageName() == "drugPressed") {
		if(this->_drug != "") {
			Game::dList->useDrug(this->_drug);
		}
	}
	else if (m->GetMessageName() == "fullChargedAttack") {
		if (this->_isLoadingAttack)
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
		this->_setCategory("jump");
		if (this->_latOrientation == RIGHT && this->_canMove && this->_isAttacking == false && !this->_isWhirlwinding) {
			this->changeSizeTo(Vector2(this->_getAttr("x").asFloat(), this->_getAttr("y").asFloat()));
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
									  this->_getAttr("jump", "fallingFrame_right").asInt(),
									  this->_getAttr("jump", "fallingFrame_right").asInt(), "jump");
		}
		else if (this->_latOrientation == LEFT && this->_canMove && this->_isAttacking == false && !this->_isWhirlwinding) {
			this->changeSizeTo(Vector2(this->_getAttr("x").asFloat(), this->_getAttr("y").asFloat()));
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
									  this->_getAttr("jump", "fallingFrame_left").asInt(),
									  this->_getAttr("jump", "fallingFrame_left").asInt(), "jump");
		}
	} else if (m->GetMessageName() == "destroyEnemy" + this->GetName()) {
		new Loot(this);
		this->_destroyEnemy();
		return;
	}
	else if (m->GetMessageName() == "setToStatic" + this->GetName()) {
		this->GetBody()->SetType(b2_staticBody);
		this->_isDead = true;
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
	else if (m->GetMessageName() == "removeTotem") {
		theSwitchboard.UnsubscribeFrom(this, "removeTotem");
		if (this->_totem != nullptr) {
			Game::addToDestroyList(this->_totem);
			this->_totem = nullptr;
			Game::getHUD()->removeText("T");
			this->_specialMove(0);
			this->_totemPlaced = 1;
		}
	} else if (m->GetMessageName() == "cycleInventory") {
		this->_inventory->changeItemFocus();
	}
	else if (m->GetMessageName() == "dropItem") {
			new Loot(this, this->_inventory->dropSelectedItem());
	}
	else if (m->GetMessageName() == "speMoveReady") {
		this->_speMoveReady = 1;
	}
	else if (m->GetMessageName() == "speAttReady") {
		this->_speAttReady = 1;
	}
	else if (m->GetMessageName().substr(0, 10) == "chooseItem") {
		if (this->_isChoosingItem == 1)
			return;
		this->_isChoosingItem = 1;
		this->_inventory->chooseItemFocus(atoi(m->GetMessageName().substr(10, 1).c_str()));
	}
	else if (m->GetMessageName() == "dashEnd") {
		theSwitchboard.UnsubscribeFrom(this, "dashEnd");
		this->_isDashing = false;
		this->GetBody()->SetGravityScale(1);
		this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
	}
	else if (m->GetMessageName() == "disengageEnd") {
		theSwitchboard.UnsubscribeFrom(this, "disengageEnd");
		this->_isDisengaging = false;
		this->_canMove = 1;
		this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
	}
	else if (m->GetMessageName() == "chargeEnd") {
		theSwitchboard.UnsubscribeFrom(this, "chargeEnd");
		this->_canMove = 1;
		this->_isCharging = false;
		this->_invincibility = false;
		this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		this->AnimCallback("base");
	}
	else if (m->GetMessageName() == "stompEnd") {
		std::string orientation;
		theSwitchboard.UnsubscribeFrom(this, "stompEnd");
		this->_invincibility = false;
		this->_isStomping = false;
		this->_isCharging = false;
		this->GetBody()->SetBullet(false);
		this->_setCategory("stomp");
		if (this->_latOrientation == RIGHT) {
			orientation = "right";
		} else if (this->_latOrientation == LEFT)
			orientation = "left";
		this->PlaySpriteAnimation(0.3f, SAT_OneShot,
								  this->_getAttr("beginFrame_" + orientation).asInt() + 2,
								  this->_getAttr("endFrame_" + orientation).asInt());
		Actor* blast = new Actor();
		blast->SetPosition(this->GetBody()->GetWorldCenter().x, this->GetBody()->GetWorldCenter().y);
		blast->SetSprite("Resources/Images/Blast/blast_000.png", 0);
		blast->SetSize(2, 1);
		this->SetLayer(99);
		blast->LoadSpriteFrames("Resources/Images/Blast/blast_000.png");
		theWorld.Add(blast);
		theSwitchboard.SubscribeTo(this, "deleteStomp");
		theSwitchboard.DeferredBroadcast(new Message("deleteStomp"), .5f);
		blast->PlaySpriteAnimation(0.1f, SAT_OneShot, 0, 1);
		blast->ChangeColorTo(Color(1, 1, 1, 0), .5f);
		this->_blast = blast;
	} else if (m->GetMessageName() == "attackReady") {
		this->_canAttack = true;
		return;
	}
	else if (m->GetMessageName() == "lockTarget") {
		if (this->_target == nullptr)
			this->_target = new HUDTargeting();
		else
			this->_target->changeTarget();

	}
	else if (m->GetMessageName() == "unlockTarget") {
		destroyTarget();
	} else if (m->GetMessageName() == "deleteStomp") {
		theWorld.Remove(this->_blast);
		theSwitchboard.UnsubscribeFrom(this, "deleteStomp");
	} else if (m->GetMessageName() == "mapPressed") {
		Game::getHUD()->bigMap();
	} else if (m->GetMessageName() == "deleteMapPressed") {
		Game::getHUD()->deleteBigMap(1);
	} else if (m->GetMessageName() == "escapePressed") {
		theSwitchboard.UnsubscribeFrom(this, "escapePressed");
		Game::toggleMenu = true;
		this->subscribeToAll();
		Game::chest->removeInterface();
	}
	for (i = this->_attr.begin(); i != this->_attr.end(); i++) {
		attrName = this->_getAttr(i->first, "subscribe").asString();
		if (!strncmp(attrName.c_str(), m->GetMessageName().c_str(), strlen(attrName.c_str()))) {
			// Get the key status (1 = Pressed, 0 = Released)
			status = (m->GetMessageName().substr(strlen(attrName.c_str()), 7) == "Pressed" ? 1 : 0);
			//if (this->_actionFlag == false && status == 1)
				//return;
			this->_actionFlag = false;
			if (this->_canMove == false)
				return;
			if (attrName == "forward") {
				this->_forward(status);
			} if (attrName == "backward") {
				this->_backward(status);
			} if (attrName == "up") {
				this->_up(status);
			} if (attrName == "down") {
				this->_down(status);
			} if (attrName == "jump") {
				this->_jump(status);
			} if (attrName == "attack") {
				this->_attack(status);
			} if (attrName == "pickupItem") {
				this->_pickupItem(status);
			} if (attrName == "specialmove") {
				this->_specialMove(status);
			} if (attrName == "specialattack") {
				this->_specialAttack(status);
			} if (attrName == "action") {
				this->_executeAction(status);
			}
			this->_lastAction = attrName;
			this->actionCallback(attrName, status);
			return ;
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
	if (s == "base" || s == "takeDamage") {
		this->_isAttacking = 0;
		if (this->_isRunning == 0 && this->_isAttacking == 0 && this->_isLoadingAttack == 0 &&
				this->_grounds.size() > 0 && !this->_isWhirlwinding) {
			this->changeSizeTo(Vector2(this->_getAttr("x").asFloat(), this->_getAttr("y").asFloat()));
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
		} else if (this->_grounds.size() == 0 && this->getAttribute("type") == "Hero") {
			this->_setCategory("jump");
			this->changeSizeTo(Vector2(this->_getAttr("jump", "x").asFloat(), this->_getAttr("jump", "y").asFloat()));
			if (this->_latOrientation == LEFT && this->_isDashing == false && !this->_isWhirlwinding) {
				this->changeSizeTo(Vector2(this->_getAttr("jump", "x").asFloat(), this->_getAttr("jump", "y").asFloat()));
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
										  this->_getAttr("fallingFrame_left").asInt(),
										  this->_getAttr("fallingFrame_left").asInt(), "base");
			} else if (this->_latOrientation == RIGHT && this->_isDashing == false && !this->_isWhirlwinding) {
				this->changeSizeTo(Vector2(this->_getAttr("jump", "x").asFloat(), this->_getAttr("jump", "y").asFloat()));
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
										  this->_getAttr("fallingFrame_right").asInt(),
										  this->_getAttr("fallingFrame_right").asInt(), "base");
			}
		} else if (this->_isAttacking == 0 && !this->_isWhirlwinding) {
			if (this->_isLoadingAttack == 0) {
				if (this->getAttribute("class") == "Warrior" && this->_isDashing == false)
					this->changeSizeTo(Vector2(1, 1));
				if (this->_isRunning == 1) {
					this->_setCategory("forward");
				} else if (this->_isRunning == 2) {
					this->_setCategory("backward");
				}
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
										  this->_getAttr("beginFrame").asInt(),
										  this->_getAttr("endFrame").asInt(), "");
			} else if (this->_fullChargedAttack == true) {
				std::string orientation;
				if (this->_latOrientation == RIGHT) {
					orientation = "Right";
				} else if (this->_latOrientation == LEFT)
					orientation = "Left";
				this->_setCategory(this->_weapon->getType());
				this->changeSizeTo(Vector2(this->_getAttr("loadX").asInt(), this->_getAttr("loadY").asInt()));
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
										  this->_getAttr("holdFrameLoad" + orientation).asInt() - 1,
										  this->_getAttr("holdFrameLoad" + orientation).asInt(), "loadAttack_charge");
			}
		}
		else {
		if (this->_isRunning != 0 && !this->_isWhirlwinding)
			this->changeSizeTo(Vector2(this->_getAttr("x").asFloat(), this->_getAttr("y").asFloat()));
		}
		if (this->_isDashing == true) {
			this->_isDashing = false;
			this->_canMove = 1;
		}
	} else if (s == "endDash") {
		this->_setCategory("dash");
		std::string orientation;
		if (this->_latOrientation == RIGHT)
			orientation = "right";
		else if (this->_latOrientation == LEFT)
			orientation = "left";
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
								  this->_getAttr("endFrame_" + orientation).asInt() - 1,
								  this->_getAttr("endFrame_" + orientation).asInt(), "base");
	} else if (s == "stompEnd") {
		this->_canMove = 1;
		this->AnimCallback("base");
	} else if (s == "chargeHold") {
		std::string orientation;
		if (this->_latOrientation == RIGHT) {
			orientation = "Right";
		} else if (this->_latOrientation == LEFT)
			orientation = "Left";
		if (this->_isCharging == true) {
			this->changeSizeTo(Vector2(this->_getAttr(this->_weapon->getType(), "chargeX").asFloat(), this->_getAttr(this->_weapon->getType(), "chargeY").asFloat()));
		}
		else
			this->AnimCallback("base");
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
	if (!elem)
		return ;
	if (this->_hp <= 0 && this->getAttribute("type") == "Hero") {
		this->_heroDeath();
		return;
	}
	if (elem->getAttribute("trigger") != "") {
		this->_callTrigger(elem->getAttribute("trigger"), 1);
	} if (elem->getAttribute("triggerOn") != "") {
		this->trigger(elem->getAttribute("triggerOn"), 1);
	} if (elem->getAttribute("triggerOff") != "") {
		this->trigger(elem->getAttribute("triggerOff"), 0);
	}
	if (elem->getAttributes()["type"] == "ground") {
		if (this->_isAttacking == 0 && this->_isLoadingAttack == 0) {
			if (this->getAttribute("class") == "Warrior" && this->_isDashing == false && !this->_isWhirlwinding && !this->_isCharging)
				this->changeSizeTo(Vector2(1, 1));
		}
		if (this->GetBody()->GetWorldCenter().y - 0.905 >= elem->GetBody()->GetWorldCenter().y) {
			if (this->_grounds.size() > 0) {
				contact->SetEnabled(false);
				this->_hasDashed = 0;
			} else {
				if (this->_isCharging == 1) {
					theSwitchboard.Broadcast(new Message("chargeEnd"));
				}
				this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
				this->_isJump = 0;
				this->_hasDashed = 0;
				if (this->_latOrientation == RIGHT && this->_isAttacking == false &&
					this->_isLoadingAttack == 0 && this->_isDashing == false &&
					this->_isStomping == false && !this->_isWhirlwinding && !this->_isCharging) {
					this->changeSizeTo(Vector2(this->_getAttr("jump", "x").asFloat(), this->_getAttr("jump", "y").asFloat()));
					this->PlaySpriteAnimation(this->_getAttr("jump", "time").asFloat(), SAT_OneShot,
											  this->_getAttr("jump", "fallingFrame_right").asInt(),
											  this->_getAttr("jump", "endFrame_right").asInt(), "base");
				} else if (this->_latOrientation == LEFT &&
						   this->_isAttacking == false &&
						   this->_isLoadingAttack == 0 &&
						   this->_isStomping == false && !this->_isWhirlwinding && !this->_isCharging) {
					if (this->_isDashing == false)
						this->changeSizeTo(Vector2(this->_getAttr("jump", "x").asFloat(), this->_getAttr("jump", "y").asFloat()));
					this->PlaySpriteAnimation(this->_getAttr("jump", "time").asFloat(), SAT_OneShot,
											  this->_getAttr("jump", "fallingFrame_left").asInt(),
											  this->_getAttr("jump", "endFrame_left").asInt(), "base");
				}
			}
			this->_grounds.push_back(elem);
			if (this->_isStomping == true) {
				theSwitchboard.Broadcast(new Message("stompEnd"));
				new Weapon(this->_weapon, this, 1, "stompArea");
				new Weapon(this->_weapon, this, -1, "stompArea");
				this->_isStomping = false;
			}
		} else if (this->GetBody()->GetWorldCenter().y <= elem->GetBody()->GetWorldCenter().y - 0.905) {
			this->_ceiling.push_back(elem);
		} else if (this->GetBody()->GetWorldCenter().x >= elem->GetBody()->GetWorldCenter().x && elem->getAttribute("speType") != "canCross") {
			if (this->_isCharging == 1) {
				theSwitchboard.Broadcast(new Message("chargeEnd"));
			}
			if (this->_wallsLeft.size() > 0)
				contact->SetEnabled(false);
			else {
				if (this->_speMove == "wallJump" && this->_grounds.size() == 0) {
					this->changeSizeTo(Vector2(this->_getAttr("jump", "holdX").asInt(), this->_getAttr("jump", "holdY").asInt()));
					this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
											  this->_getAttr("jump", "holdFrame_left").asInt(),
											  this->_getAttr("jump", "holdFrame_left").asInt(), "jump");
					this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
					this->GetBody()->SetGravityScale(0.1);
				}
			}
			this->_wallsLeft.push_back(elem);
		} else if (this->GetBody()->GetWorldCenter().x < elem->GetBody()->GetWorldCenter().x && elem->getAttribute("speType") != "canCross") {
			if (this->_isCharging == 1) {
				theSwitchboard.Broadcast(new Message("chargeEnd"));
			}
			if (this->_wallsRight.size() > 0)
				contact->SetEnabled(false);
			else {
				if (this->_speMove == "wallJump" && this->_grounds.size() == 0) {
					this->changeSizeTo(Vector2(this->_getAttr("jump", "holdX").asInt(), this->_getAttr("jump", "holdY").asInt()));
					this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
											  this->_getAttr("jump", "holdFrame_right").asInt(),
											  this->_getAttr("jump", "holdFrame_right").asInt(), "jump");
					this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
					this->GetBody()->SetGravityScale(0.1);
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
	if (!elem)
		return ;
	if (elem->getAttribute("trigger") != "") {
		this->_callTrigger(elem->getAttribute("trigger"), 1);
	} if (elem->getAttribute("triggerOn") != "") {
		this->trigger(elem->getAttribute("triggerOn"), 1);
	} if (elem->getAttribute("triggerOff") != "") {
		this->trigger(elem->getAttribute("triggerOff"), 0);
	}

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
					if (this->_isDashing == false && !this->_isWhirlwinding && !this->_isCharging) {
						this->changeSizeTo(Vector2(this->_getAttr("jump", "x").asFloat(), this->_getAttr("jump", "y").asFloat()));
						this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
												  this->_getAttr("jump", "fallingFrame_right").asInt(),
												  this->_getAttr("jump", "fallingFrame_right").asInt(), "jump");
					}
				} else if (this->_lastAction == "backward" && this->_canMove &&
						   this->_isAttacking == false && this->_isLoadingAttack == 0) {
					if (this->_isDashing == false && !this->_isWhirlwinding && !this->_isCharging) {
						this->changeSizeTo(Vector2(this->_getAttr("jump", "x").asFloat(), this->_getAttr("jump", "y").asFloat()));
						this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
												  this->_getAttr("jump", "fallingFrame_left").asInt(),
												  this->_getAttr("jump", "fallingFrame_left").asInt(), "jump");
					}
				}
			}
		}
		else
			this->_grounds.remove(elem);
		if (this->_wallsLeft.size() == 0 && this->_wallsRight.size() == 0 && this->_speMove == "wallJump" && this->_grounds.size() == 0) {
			this->GetBody()->SetGravityScale(1);
			this->AnimCallback("base");
		}
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
/*      CHARACTER LOOP      */
/*                          */
/****************************/

void	Characters::_tryFly(void) {
	if (this->_flyTrigger == true)
		this->GetBody()->SetLinearVelocity(b2Vec2(this->GetBody()->GetLinearVelocity().x,
					this->_getAttr("fly","force").asFloat()));
	else if (this->_grounds.size() == 0 && this->_canMove == 1 && this->_isFlying == true)
		this->GetBody()->SetLinearVelocity(b2Vec2(this->GetBody()->GetLinearVelocity().x, -2));
}

void	Characters::_resetBroadcastFlags(void) {
	this->_actionFlag = true;
	this->_forwardFlag = false;
	this->_backwardFlag = false;
	this->_doFlyFlag = false;
	this->_isChoosingItem = 0;
}

/****************************/
/*                          */
/*          ACTIONS         */
/*                          */
/****************************/

//! Execute action
/**
 * The character will execute an action depending on what he currently touches
 * if nothing is touched, drug is used
 */
void	Characters::_executeAction(int status) {
	if (status == 1 && this->_execFlag == 1)
		return;
	if (status == 0) {
		this->_execFlag = 0;
		return;
	}
	this->_execFlag = 1;
	if (this->_isTouchingChest == true && Game::chest->isUsed() == 0) {
		theCamera.MoveTo(Vector3(Game::spawnChest.X, Game::spawnChest.Y, 4), true);
		Game::toggleMenu = false;
		this->unsubscribeFromAll();
		theSwitchboard.SubscribeTo(this, "escapePressed");
		Game::chest->displayInterface();
	} else if (this->_isTouchingBossDoor == true) {
		Game::stopPattern = true;
		Game::currentGame->getCurrentMap().destroyMap();
		this->inSpecialMap = 1;
		this->destroyTarget();
		Game::currentGame->maps->bossMap->display();
		theCamera.SetPosition(Game::currentGame->maps->bossMap->getXMid(), Game::currentGame->maps->bossMap->getYMid() + 1.8);
		Game::addBoss("Boss" + std::to_string(Game::World), Game::currentGame->maps->bossMap->getXMid(), Game::currentGame->maps->bossMap->getYMid());
		this->GetBody()->SetTransform(b2Vec2(Game::currentGame->maps->bossMap->getXMid() - 10, Game::currentGame->maps->bossMap->getYMid() + 1.8), 0);
	} else if (this->_isTouchingSecretDoor == true) {
		this->destroyTarget();
		if (this->inSpecialMap == 2) {
			Game::stopPattern = false;
			Game::currentGame->maps->secretMap->destroyMap();
			this->inSpecialMap = 0;
			theCamera.SetPosition(Game::currentGame->getCurrentMap().getXMid(), Game::currentGame->getCurrentMap().getYMid() + 1.8);
			this->GetBody()->SetTransform(b2Vec2(Game::spawnSecretDoor.X, Game::spawnSecretDoor.Y), 0);
 			b2PolygonShape box = Game::hList->getHitbox(this->_hitbox);
 			b2Shape *shape = &box;
 			this->GetBody()->DestroyFixture(this->GetBody()->GetFixtureList());
 			this->GetBody()->CreateFixture(shape, 1);
		} else {
			Game::stopPattern = true;
			this->inSpecialMap = 2;
			Game::currentGame->maps->secretMap->display();
			theCamera.SetPosition(Game::currentGame->getCurrentMap().getXMid(), Game::currentGame->getCurrentMap().getYMid() + 1.8);
			this->GetBody()->SetTransform(b2Vec2(Game::spawnSecretReturnDoor.X, Game::spawnSecretReturnDoor.Y), 0);
 			b2PolygonShape box = Game::hList->getHitbox(this->_hitbox);
 			b2Shape *shape = &box;
 			this->GetBody()->DestroyFixture(this->GetBody()->GetFixtureList());
 			this->GetBody()->CreateFixture(shape, 1);
		}
	}
	if(this->_isTouchingDealer == true)
		theSwitchboard.Broadcast(new Message("giveDrug"));
	if (this->_drug != "")
		theSwitchboard.Broadcast(new Message("drugPressed"));
}


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
		if (this->_forwardFlag == true)
			return;
		this->_forwardFlag = true;
		this->_orientation = RIGHT;
		this->_latOrientation = RIGHT;
		if ((this->GetSpriteFrame() < this->_getAttr("beginFrame").asInt() ||
					(this->GetSpriteFrame() >= this->_getAttr("backward", "beginFrame").asInt() &&
					 this->GetSpriteFrame() <= this->_getAttr("backward", "endFrame").asInt()))  &&
			!this->_isJump && !this->_isLoadingAttack && !this->_isWhirlwinding && !this->_isCharging) {
			this->changeSizeTo(Vector2(this->_getAttr("x").asInt(), this->_getAttr("y").asInt()));
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
									  this->_getAttr("beginFrame").asInt(), this->_getAttr("endFrame").asInt());
		}
		else if (this->_isJump && !this->_isLoadingAttack && !this->_isFlying && !this->_isWhirlwinding && !this->_isCharging) {
			this->_setCategory("jump");
			this->changeSizeTo(Vector2(this->_getAttr("x").asInt(), this->_getAttr("y").asInt()));
			if (this->GetSpriteFrame() >= this->_getAttr("beginFrame_left").asInt() && this->GetSpriteFrame() <= this->_getAttr("endFrame_left").asInt())
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
										  this->_getAttr("fallingFrame_right").asInt(),
										  this->_getAttr("fallingFrame_right").asInt(), "jump");
			this->_setCategory("forward");
		} else if (this->_isLoadingAttack) {
			this->_setCategory(this->_weapon->getType());
			this->changeSizeTo(Vector2(this->_getAttr("loadX").asInt(), this->_getAttr("loadY").asInt()));
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
					this->_getAttr("holdFrameLoadRight").asInt() - 1,
					this->_getAttr("holdFrameLoadRight").asInt(), "loadAttack_charge");
			this->_setCategory("forward");
		}
		Game::startRunning(this);
		if (this->_isRunning == 2 && this->_isDashing == false)
			this->GetBody()->SetLinearVelocity(b2Vec2(this->_getAttr("force").asFloat() + this->buff.bonusSpeed + this->buff.drugSpeed, this->GetBody()->GetLinearVelocity().y));
		this->_isRunning = 1;
	} else if (status == 0 && this->_latOrientation == RIGHT) {
	  if (this->_isDashing == false)
		this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
	  Game::stopRunning(this);
		this->_isRunning = 0;
		if (!this->_isJump && !this->_isAttacking && this->_isDashing == false)
			this->AnimCallback("base");
	} else {
		if (this->_forwardFlag == true)
			return;
		this->_forwardFlag = true;
		if (this->_wallsRight.size() == 0 && this->_canMove == true && this->_isRunning != 0 && this->_isDashing == false && this->buff.cur != "mdma")
			this->GetBody()->SetLinearVelocity(b2Vec2(this->_getAttr("force").asFloat() + this->buff.bonusSpeed + this->buff.drugSpeed, this->GetBody()->GetLinearVelocity().y));
		else if (this->_wallsLeft.size() == 0 && this->_canMove == true && this->_isRunning != 0 && this->_isDashing == false && this->buff.cur == "mdma")
			this->GetBody()->SetLinearVelocity(b2Vec2(this->_getAttr("force").asFloat() + this->buff.bonusSpeed + this->buff.drugSpeed, this->GetBody()->GetLinearVelocity().y));
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
		if (this->_backwardFlag == true)
			return;
		this->_backwardFlag = true;
		if (this->getAttribute("type") == "Hero") {
			this->_orientation = LEFT;
			this->_latOrientation = LEFT;
		}
		if (this->GetSpriteFrame() < this->_getAttr("beginFrame").asInt() &&
			!this->_isJump && !this->_isLoadingAttack && !this->_isWhirlwinding && !this->_isCharging) {
			this->changeSizeTo(Vector2(this->_getAttr("x").asInt(), this->_getAttr("y").asInt()));
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
									  this->_getAttr("beginFrame").asInt(),
									  this->_getAttr("endFrame").asInt());
		} else if (this->_isJump && !this->_isLoadingAttack && !this->_isWhirlwinding && !this->_isCharging) {
			this->_setCategory("jump");
			this->changeSizeTo(Vector2(this->_getAttr("x").asInt(), this->_getAttr("y").asInt()));
			if (this->GetSpriteFrame() >= this->_getAttr("beginFrame_right").asInt() && this->GetSpriteFrame() <= this->_getAttr("endFrame_right").asInt())
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
										  this->_getAttr("fallingFrame_left").asInt(),
										  this->_getAttr("fallingFrame_left").asInt(), "jump");
			this->_setCategory("backward");
		} else if (this->_isLoadingAttack) {
			this->_setCategory(this->_weapon->getType());
			this->changeSizeTo(Vector2(this->_getAttr("loadX").asInt(), this->_getAttr("loadY").asInt()));
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
					this->_getAttr("holdFrameLoadLeft").asInt() - 1,
					this->_getAttr("holdFrameLoadLeft").asInt(), "loadAttack_charge");
			this->_setCategory("backward");
		}
		Game::startRunning(this);
		if (this->_isRunning == 1 && this->_isDashing == false)
			this->GetBody()->SetLinearVelocity(b2Vec2(-this->_getAttr("force").asFloat() - this->buff.bonusSpeed - this->buff.drugSpeed, this->GetBody()->GetLinearVelocity().y));
		this->_isRunning = 2;
	} else if (status == 0 && this->_latOrientation == LEFT) {
	  if (this->_isDashing == false)
		this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
	  Game::stopRunning(this);
		this->_isRunning = 0;
		if (!this->_isJump && !this->_isAttacking && !this->_isDashing)
			this->AnimCallback("base");
	} else {
		if (this->_backwardFlag == true)
			return;
		this->_backwardFlag = true;
		if (this->_wallsLeft.size() == 0 && this->_canMove == true && this->_isRunning != 0 && this->_isDashing == false && this->buff.cur != "mdma")
			this->GetBody()->SetLinearVelocity(b2Vec2(-this->_getAttr("force").asFloat() - this->buff.bonusSpeed - this->buff.drugSpeed, this->GetBody()->GetLinearVelocity().y));
		else if (this->_wallsRight.size() == 0 && this->_canMove == true && this->_isRunning != 0 && this->_isDashing == false && this->buff.cur == "mdma")
			this->GetBody()->SetLinearVelocity(b2Vec2(-this->_getAttr("force").asFloat() - this->buff.bonusSpeed - this->buff.drugSpeed, this->GetBody()->GetLinearVelocity().y));
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
	if (_isFlying == 0) {
		if (status == 1 && this->_canJump == true) {
			this->_canJump = false;
			if (this->_speMove == "wallJump" && this->_grounds.size() == 0 &&
					(this->_wallsLeft.size() > 0 || this->_wallsRight.size() > 0)) {
				this->GetBody()->SetGravityScale(1);
				if (this->_wallsLeft.size() > 0 && this->_wallsRight.size() == 0) {
					this->GetBody()->SetLinearVelocity(b2Vec2(5, this->_getAttr("rejump").asFloat()));
					this->changeSizeTo(Vector2(this->_getAttr("x").asInt(), this->_getAttr("y").asInt()));
					this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
											  this->_getAttr("holdFrame_left").asInt(),
											  this->_getAttr("rejumpFrame_left").asInt(), "base");
				} else if (this->_wallsRight.size() > 0 && this->_wallsLeft.size() == 0) {
					this->GetBody()->SetLinearVelocity(b2Vec2(-5, this->_getAttr("rejump").asFloat()));
					this->changeSizeTo(Vector2(this->_getAttr("x").asInt(), this->_getAttr("y").asInt()));
					this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
											  this->_getAttr("holdFrame_right").asInt(),
											  this->_getAttr("rejumpFrame_right").asInt(), "base");
				} else
					this->GetBody()->SetLinearVelocity(b2Vec2(0, this->_getAttr("rejump").asFloat()));
				Game::stopRunning(this);
			} else if (this->_isJump == 0 || (this->_isJump < this->_getAttr("max").asInt())) {
				if (this->_isJump >= 1) {
					this->GetBody()->SetLinearVelocity(b2Vec2(this->GetBody()->GetLinearVelocity().x,
															  this->_getAttr("rejump").asFloat()));
					if (this->_isAttacking == false && this->_isLoadingAttack == 0 && !this->_isWhirlwinding && !this->_isCharging) {
						if (this->_isDashing == false)
							this->changeSizeTo(Vector2(this->_getAttr("jump", "x").asFloat(), this->_getAttr("jump", "y").asFloat()));
						if (this->_latOrientation == RIGHT) {
							this->changeSizeTo(Vector2(this->_getAttr("x").asInt(), this->_getAttr("y").asInt()));
							this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
													  this->_getAttr("beginFrame_right").asInt(),
													  this->_getAttr("fallingFrame_right").asInt(), "jump");
						}
					}
				} else {
					this->ApplyLinearImpulse(Vector2(0, this->_getAttr("force").asFloat()), Vector2(0, 0));
				}
				if (this->_isAttacking == false && this->_isLoadingAttack == 0 && !this->_isWhirlwinding && !this->_isCharging) {
					this->changeSizeTo(Vector2(this->_getAttr("jump", "x").asFloat(), this->_getAttr("jump", "y").asFloat()));
					if (this->_latOrientation == RIGHT) {
						this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
								this->_getAttr("beginFrame_right").asInt(),
								this->_getAttr("fallingFrame_right").asInt(), "jump");
					} else if (this->_latOrientation == LEFT) {
						this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
								this->_getAttr("beginFrame_left").asInt(),
								this->_getAttr("fallingFrame_left").asInt(), "jump");
					}
				}
				if (this->_grounds.size() == 0)
					this->_isJump++;
			}
		} else if (status == 0) {
			this->_canJump = true;
		}
	} else { //Hero is flying
		if (this->_doFlyFlag == true && status == 1)
			return ;
		this->_doFlyFlag = true;
		this->_flyTrigger = (status ? 1 : 0);
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
  if (this->_shopItem != "" && this->_gold >= this->_shopItemPrice) {
	if (this->_inventory->addItemToInventory(this->_shopItem) == 1) {
	  if (Game::wList->checkExists(this->_inventory->getCurrentFocus()))
		new Loot(this, Game::wList->getWeapon(this->_inventory->dropSelectedItem()));
	  else if (Game::aList->checkExists(this->_inventory->getCurrentFocus()))
		new Loot(this, Game::aList->getArmor(this->_inventory->dropSelectedItem()));
	  else if (Game::rList->checkExists(this->_inventory->getCurrentFocus()))
		new Loot(this, Game::rList->getRing(this->_inventory->dropSelectedItem()));
	  else
		Log::error("An error occured trying to drop " + this->_inventory->getCurrentFocus());
	  this->_inventory->addItemToInventory(this->_shopItem);
	}
	theSwitchboard.Broadcast(new Message("deleteShopItem" +
										 std::to_string(this->_shopItemNumber)));
	theSwitchboard.DeferredBroadcast(new Message("removeShopkeeperText"), 3);
	this->_shopItem = "";
	  this->_gold -= this->_shopItemPrice;
	  Game::getHUD()->updateGold(this->_gold);
	  this->_shopItemNumber = 0;
	  this->_shopItemPrice = 0;
  }
  else if (this->_item != nullptr) {
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
  else {
	  theSwitchboard.Broadcast(new Message("dropItem"));
  }
}

//! Executes special move
/**
 * Executes the characters special move, based on which one he picked up at the beginning
 * @todo should not be based off the json config
 */

void	Characters::_specialMove(int status) {
	if (status == 1 && this->_speMoveReady) {
		if (this->_speMove == "totem" && this->_totemDeletionSent == 0 && this->_totem != nullptr)
			this->_totemDeletionSent = 1;
			theSwitchboard.SubscribeTo(this, "removeTotem");
			theSwitchboard.DeferredBroadcast(new Message("removeTotem"), 3);
		if (this->_speMove == "dash")
			this->_eqMove->_dash();
		else if (this->_speMove == "charge")
			this->_eqMove->_charge();
		else if (this->_speMove == "stomp")
			this->_eqMove->_stomp();
		else if (this->_speMove == "blink")
			this->_eqMove->_blink();
		else if (this->_speMove == "fly")
			this->_eqMove->_fly();
		else if (this->_speMove == "shunpo")
			this->_eqMove->_shunpo();
		else if (this->_speMove == "disengage")
			this->_eqMove->_disengage();
		this->_speMoveReady = 0;
	}
	if (status == 0) {
		if (this->_speMove == "totem") {
			this->_totemDeletionSent = 0;
			theSwitchboard.UnsubscribeFrom(this, "removeTotem");
			if (this->_totemPlaced == 0)
				this->_eqMove->_totem();
			else
				this->_totemPlaced = 0;
		}
	}
}

void	Characters::_specialAttack(int status) {
	if (status == 0) {
		if (this->_speAtt == "whirlwind")
			this->_eqAtt->_whirlwind();
		else if (this->_speAtt == "shockwave")
			this->_eqAtt->_shockwave();
		if (this->_speAtt == "rapidFire")
			this->_eqAtt->_rapidFire();
	}
}

//! Equip a weapon
/**
 * Equip a new weapon to the Character, and update the HUD.
 * @param The Weapon object
 */
void	Characters::equipWeapon(Weapon* weapon) {
	if (weapon->getEquipable() == this->getAttribute("class")) {
		this->_weapon = new Weapon(weapon);
		Game::getHUD()->items(this->_weapon);
	}
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

	if (this->_armor->getAttribute("hpBuff") != "")
		this->_maxHp += std::stoi(this->_armor->getAttribute("hpBuff"));
	if (this->_armor->getAttribute("dmgReduc") != "")
		this->buff.dmgReduc += std::stoi(this->_armor->getAttribute("dmgReduc"));
	if (this->_armor->getAttribute("bonusSpeed") != "")
		this->buff.bonusSpeed += std::stoi(this->_armor->getAttribute("bonusSpeed"));
	if (this->_armor->getAttribute("bonusDmg") != "")
		this->buff.bonusDmg += std::stoi(this->_armor->getAttribute("bonusDmg"));
	Game::getHUD()->items(this->_armor);
	Game::getHUD()->setMaxHP(this->_maxHp);
	Game::getHUD()->life(this->_hp);
}

//! Unequip a armor
/**
 * Unequip old armor to the Character, and update the HUD.
 * @param void
 */
void	Characters::unequipArmor(void) {
	this->_inventory->swapEquipmentAndInventory(this->_armor->getAttribute("name"));
	if (this->_armor->getAttribute("hpBuff") != "") {
		this->_maxHp -= std::stoi(this->_armor->getAttribute("hpBuff"));
		if (this->_hp > this->_maxHp)
			this->_hp = this->_maxHp;
	}
	if (this->_armor->getAttribute("dmgReduc") != "")
		this->buff.dmgReduc -= std::stoi(this->_armor->getAttribute("dmgReduc"));
	if (this->_armor->getAttribute("bonusSpeed") != "")
		this->buff.bonusSpeed -= std::stoi(this->_armor->getAttribute("bonusSpeed"));
	if (this->_armor->getAttribute("bonusDmg") != "")
		this->buff.bonusDmg -= std::stoi(this->_armor->getAttribute("bonusDmg"));
	Game::getHUD()->items(this->_armor);
	Game::getHUD()->setMaxHP(this->_maxHp);
	Game::getHUD()->life(this->_hp);
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
	if (this->_ring->getAttribute("dmgReduc") != "")
		this->buff.dmgReduc += std::stoi(this->_ring->getAttribute("dmgReduc"));
	if (this->_ring->getAttribute("bonusSpeed") != "")
		this->buff.bonusSpeed += std::stoi(this->_ring->getAttribute("bonusSpeed"));
	if (this->_ring->getAttribute("bonusDmg") != "")
		this->buff.bonusDmg += std::stoi(this->_ring->getAttribute("bonusDmg"));
	Game::getHUD()->items(this->_ring);
	Game::getHUD()->setMaxHP(this->_maxHp);
	Game::getHUD()->life(this->_hp);
}

//! Unequip a ring
/**
 * Unequip old ring to the Character, and update the HUD.
 * @param The ring object
 */
void	Characters::unequipRing(void) {
	this->_inventory->swapEquipmentAndInventory(this->_ring->getAttribute("name"));
	if (this->_ring->getAttribute("hpBuff") != "") {
		this->_maxHp -= std::stoi(this->_ring->getAttribute("hpBuff"));
		if (this->_hp > this->_maxHp)
			this->_hp = this->_maxHp;
	}
	if (this->_ring->getAttribute("dmgReduc") != "")
		this->buff.dmgReduc -= std::stoi(this->_ring->getAttribute("dmgReduc"));
	if (this->_ring->getAttribute("bonusSpeed") != "")
		this->buff.bonusSpeed -= std::stoi(this->_ring->getAttribute("bonusSpeed"));
	if (this->_ring->getAttribute("bonusDmg") != "")
		this->buff.bonusDmg -= std::stoi(this->_ring->getAttribute("bonusDmg"));
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
	Game::getHUD()->life(this->_hp);
};

//! Set invincibility
/**
 * Set invincibility to the Character.
 * @param invincibility 
 */
void						Characters::setInvincibility(bool invincibility) {
	this->_invincibility = invincibility;
};

//! Destroy an Enemy.
/**
 * Make theSwitchboard Unsubscribe from the object, and destroy it.
 */
void						Characters::_destroyEnemy(void) {
  theSwitchboard.UnsubscribeFrom(this, "destroyEnemy" + this->GetName());
	Game::addToDestroyList(this);
}

//! Destroy current target.
/**
 * Destroy target if hero have one.
 */
void						Characters::destroyTarget(void) {
	if (this->_target != nullptr) {
		Game::addToDestroyList(this->_target);
		this->_target = nullptr;
	}
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
	ghost->SetLayer(101);
	theWorld.Add(ghost);
	theSwitchboard.Broadcast(new Message("deleteBoss"));
	ghost->PlaySpriteAnimation(0.2f, SAT_OneShot, 0, 10, "ghost");
	this->_ghost = ghost;
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

void						Characters::UnsubscribeFromAll(void) {
	StringSet sub;

	sub = theSwitchboard.GetSubscriptionsFor(this);
	for (StringSet::iterator k = sub.begin(); k != sub.end(); k++) {
		theSwitchboard.UnsubscribeFrom(this, *k);
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

void						Characters::_callTrigger(std::string name, int s) {
	if (this->_triggers[name] != s) {
		this->_triggers[name] = s;
		this->trigger(name, s);
	}
}

/* GETTERS */
Characters::Orientation		Characters::getOrientation(void) { return this->_orientation; }
std::string					Characters::getLastAction(void) { return this->_lastAction; };
Characters::Orientation		Characters::getLatOrientation(void) { return this->_latOrientation; }
Elements*					Characters::getItem(void) { return this->_item; }
int							Characters::getHP(void) { return this->_hp; };
int							Characters::getMaxHP(void) { return this->_maxHp; };
int							Characters::getGold(void) { return this->_gold; };
Actor						*Characters::getGhost(void) { return this->_ghost; };
void						Characters::setGold(int n) { this->_gold = n; };
void						Characters::setLevel(int n) { this->_level = n; };
void						Characters::changeCanMove(void) { this->_canMove = (this->_canMove ? false : true); };
void						Characters::setDrug(std::string name) { this->_drug = name; };
Weapon						*Characters::getWeapon(void) { return this->_weapon; };
Armor						*Characters::getArmor(void) { return this->_armor; };
Ring						*Characters::getRing(void) { return this->_ring; };
bool						Characters::getCharging(void) { return this->_isCharging; }
std::list<std::string>		Characters::getSubscribes(void) { return this->_subsc; };
int							Characters::getLevel(void) { return this->_level; };
int							Characters::getMaxInventory(void) { return this->_inventory->getSlots(); };
Inventory					*Characters::getInventory(void) { return this->_inventory; };
std::string					Characters::getSpeMove(void) { return this->_speMove; };
std::string					Characters::getSpeAtt(void) { return this->_speAtt; };
