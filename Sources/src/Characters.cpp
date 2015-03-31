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

/**
 * Base constructor
 */
Characters::Characters(void) {
	return ;
}

/**
 * Main constructor
 * @param: name (std::string)
 */
Characters::Characters(std::string name) : _name(name), _isRunning(0), _isJump(0) {
	this->addAttribute("physic", "1");
	this->addAttribute("type", name);
	this->SetDensity(1.0f);
	this->SetFriction(1);
	this->SetRestitution(0.0f);
	this->SetFixedRotation(true);
	this->_orientation = RIGHT;
	this->_readFile(name);
	this->_canMove = true;
	this->_canJump = true;
	this->_invincibility = false;
	this->_grounds.clear();
	this->_walls.clear();
	this->_item = nullptr;
	this->_isAttacking = 0;
}

/**
 * Basic destructor
 */
Characters::~Characters(void) {
	return ;
}

/**
 * Read a config file, base on the name of the class
 * @param: name (std::string)
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

/**
 * Parse, read and stock the info in the config file
 * @param: file (std::string)
 * @note: file is the whole file content
 */
void	Characters::_parseJson(std::string file) {
	Json::Reader	read;
	Json::Value		json;
	Json::ValueIterator i, v;
	std::map<std::string, Json::Value>	tmp;

	if (!read.parse(file, json))
		Log::error("Error in json syntax :\n" + read.getFormattedErrorMessages());
	if (json["infos"].get("name", "").asString() != this->_name)
		Log::warning("The class name is different with the name in the config file.");
	this->_name = json["infos"].get("name", "").asString();
	this->_id = json["infos"].get("id", "").asInt();
	this->_size = json["infos"].get("size", "").asFloat();
	this->_maxSpeed = json["infos"].get("maxSpeed", "").asFloat();
	this->_hp = json["infos"].get("HP", "").asInt();
	this->_maxHp = json["infos"].get("HP", "").asInt();
	this->_hitboxType = json["infos"].get("hitboxType", "").asString();
	this->_hitbox = json["infos"].get("hitbox", "").asString();
	this->addAttribute("spritesFrame", json["infos"].get("sprites", "").asString());

	for (i = json["Actions"].begin(); i != json["Actions"].end(); i++) {
		for (v = (*i).begin(); v != (*i).end(); v++) {
			tmp[v.key().asString()] = (*v);
			this->_attr[i.key().asString()] = tmp;
			// Subcribe to the broadcasts
			if (v.key().asString() == "subscribe") {
//				Log::info("SubscribeTo " + (*v).asString());
				theSwitchboard.SubscribeTo(this, (*v).asString() + "Pressed");
				theSwitchboard.SubscribeTo(this, (*v).asString() + "Released");
			}
		}
	}
}

/**
 * Get a Json::Value of a key in the config file
 * @param: category (std::string)
 * @param: key (std::string)
 * @note: See the docs for the utilisation of Json::Value
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

/**
 * Set the current working category
 * @param: category (std::string)
 */
void	Characters::_setCategory(std::string category) {
	this->_category = category;
}

/**
 * Get a Json::Value of a key in the config file, with a pre-set category
 * @param: key (std::string)
 */
Json::Value		Characters::_getAttr(std::string key) {
	if (this->_attr[this->_category].find(key) != this->_attr[this->_category].end())
		return this->_attr[this->_category][key];
	Log::warning("The key " + key + " not in the category " + this->_category);
	return nullptr;
}

/**
 * Receive and redistribute broadcasts messages
 * @param: m (Message *)
 */
void	Characters::ReceiveMessage(Message *m) {
	std::map<std::string, std::map<std::string, Json::Value> >::iterator	i;
	std::string		name, attrName;
	int				status;

	if (m->GetMessageName() == "canMove") {
		this->changeCanMove();
	}
	else if (m->GetMessageName() == "endInvincibility") {
		theSwitchboard.UnsubscribeFrom(this, "colorDamageBlink1");
		theSwitchboard.UnsubscribeFrom(this, "colorDamageBlink2");
		this->_invincibility = false;
		this->SetColor(1,1,1,1);
	}
	else if (m->GetMessageName() == "colorDamageBlink1") {
		theSwitchboard.DeferredBroadcast(new Message("colorDamageBlink2"), 0.1f);
		this->SetColor(1,1,1,1);
	}
	else if (m->GetMessageName() == "colorDamageBlink2") {
		theSwitchboard.DeferredBroadcast(new Message("colorDamageBlink1"), 0.1f);
		this->SetColor(1,0,0,0.9f);
	}
	else if (m->GetMessageName() == "startPathing") {
		if (this->_grounds.size() > 0) {
			if (this->_wallsLeft.size() > 0)
				this->_orientation = RIGHT;
			else if (this->_wallsRight.size() > 0)
				this->_orientation = LEFT;
			if (this->_orientation == RIGHT)
				this->GetBody()->SetLinearVelocity(b2Vec2(5,0));
			if (this->_orientation == LEFT)
				this->GetBody()->SetLinearVelocity(b2Vec2(-5,0));
		}
		theSwitchboard.DeferredBroadcast(new Message("startPathing"), 0.2f);
		return;
	}
	else if (m->GetMessageName() == "destroyEnemy") {
		this->_destroyEnemy();
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

/**
 * The animation callback
 * @param: s (String)
 * @note: String is the std::string object used in Angel2d.
 */
void	Characters::AnimCallback(String s) {
	this->_setCategory("breath");
	if (s == "base") {
		this->changeSizeTo(Vector2(1, 1));
		this->_isAttacking = 0;
		if (this->_isRunning == 0) {
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
		} else {
			if (this->_isRunning == 1) {
				this->_setCategory("forward");
			} else if (this->_isRunning == 2) {
				this->_setCategory("backward");
			}
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
									  this->_getAttr("beginFrame").asInt(),
									  this->_getAttr("endFrame").asInt());
		}
	}
}

/**
 * Collision begin callback
 * @param: elem (Elements *)
 * @param: contact (b2Contact *)
 * @note: This function is called just before a collision
 */
void	Characters::BeginContact(Elements *elem, b2Contact *contact) {
	if (elem->getAttributes()["type"] == "ground") {
		if (this->GetBody()->GetWorldCenter().y - 0.905 >= elem->GetBody()->GetWorldCenter().y) {
			if (this->_grounds.size() > 0)
				contact->SetEnabled(false);
			else
				this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
			if (this->_isJump > 0) {
				this->_isJump = 0;
				if (this->_latOrientation == RIGHT)
				this->PlaySpriteAnimation(0.1f, SAT_OneShot,
										  this->_getAttr("jump", "endFrame_right").asInt() - 2,
										  this->_getAttr("jump", "endFrame_right").asInt(), "base");
				if (this->_latOrientation == LEFT)
				this->PlaySpriteAnimation(0.1f, SAT_OneShot,
										  this->_getAttr("jump", "endFrame_left").asInt() - 2,
										  this->_getAttr("jump", "endFrame_left").asInt(), "base");
			}
			this->_grounds.push_back(elem);
		} else if (this->GetBody()->GetWorldCenter().x >= elem->GetBody()->GetWorldCenter().x) {
			if (this->_wallsLeft.size() > 0)
				contact->SetEnabled(false);
			this->_wallsLeft.push_back(elem);
		} else if (this->GetBody()->GetWorldCenter().x < elem->GetBody()->GetWorldCenter().x) {
			if (this->_wallsRight.size() > 0)
				contact->SetEnabled(false);
			this->_wallsRight.push_back(elem);
		}
	}
}

/**
 * Collision end callback
 * @param: elem (Elements *)
 * @param: contact (b2Contact *)
 * @note: This function is called just after the elements leave another
 */
void	Characters::EndContact(Elements *elem, b2Contact *contact) {
	if (elem->getAttributes()["type"] == "ground") {
		this->_wallsLeft.remove(elem);
		this->_wallsRight.remove(elem);
		if (this->_grounds.size() == 1) {
			this->_grounds.remove(elem);
			if (this->_grounds.size() == 0) {
				this->_isJump++;
				if (this->_lastAction == "forward" && this->_canMove)
					this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
										  this->_getAttr("jump", "fallingFrame_right").asInt(),
										  this->_getAttr("jump", "endFrame_right").asInt() - 3, "jump");
				else if (this->_lastAction == "backward" && this->_canMove)
					this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
										  this->_getAttr("jump", "fallingFrame_left").asInt(),
										  this->_getAttr("jump", "endFrame_left").asInt() - 3, "jump");
			}
		}
		else
			this->_grounds.remove(elem);
	}
}

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

/**
 * Forward action
 * @param: status (int)
 */
void	Characters::_forward(int status) {
	this->_setCategory("forward");
	if (status == 1) {
		this->_orientation = RIGHT;
		this->_latOrientation = RIGHT;
		if ((this->GetSpriteFrame() < this->_getAttr("beginFrame").asInt() ||
			 (this->GetSpriteFrame() >= this->_getAttr("backward", "beginFrame").asInt() &&
			  this->GetSpriteFrame() <= this->_getAttr("backward", "endFrame").asInt()))  && !this->_isJump)
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
				this->_getAttr("beginFrame").asInt(), this->_getAttr("endFrame").asInt());
		else if (this->_isJump) {
			this->_setCategory("jump");
			if (this->GetSpriteFrame() >= this->_getAttr("beginFrame_left").asInt() && this->GetSpriteFrame() <= this->_getAttr("endFrame_left").asInt())
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
										  this->GetSpriteFrame() - 8,
										  this->_getAttr("endFrame_right").asInt() - 3, "jump");
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
		if (this->GetBody()->GetLinearVelocity().x < this->_maxSpeed) {
			if (this->_wallsRight.size() == 0 && this->_canMove == true)
				this->GetBody()->SetLinearVelocity(b2Vec2(this->_getAttr("force").asFloat(), this->GetBody()->GetLinearVelocity().y));
		}
	}
	return ;
}

/**
 * Backward action
 * @param: status (int)
 */
void	Characters::_backward(int status) {
   this->_setCategory("backward");
	if (status == 1) {
		this->_orientation = LEFT;
		this->_latOrientation = LEFT;
		if (this->GetSpriteFrame() < this->_getAttr("beginFrame").asInt() && !this->_isJump)
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
				this->_getAttr("beginFrame").asInt(),
				this->_getAttr("endFrame").asInt());
		else if (this->_isJump) {
			this->_setCategory("jump");
			if (this->GetSpriteFrame() >= this->_getAttr("beginFrame_right").asInt() && this->GetSpriteFrame() <= this->_getAttr("endFrame_right").asInt())
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
										  this->GetSpriteFrame() + 8,
										  this->_getAttr("endFrame_left").asInt() - 3, "jump");
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
		if (this->GetBody()->GetLinearVelocity().x > -(this->_maxSpeed)) {
			if (this->_wallsLeft.size() == 0 && this->_canMove == true)
				this->GetBody()->SetLinearVelocity(b2Vec2(-this->_getAttr("force").asFloat(), this->GetBody()->GetLinearVelocity().y));
		}
	}
	return ;
}

/**
 * Jump Action
 * @param: status (int)
 */
void	Characters::_jump(int status) {
	this->_setCategory("jump");

	if (status == 1 && this->_canJump == true) {
		this->_canJump = false;
		if (this->_isJump == 0 || (this->_isJump <= 1 && this->_getAttr("double").asInt() == 1)) {
			if (this->_isJump >= 1) {
				this->GetBody()->SetLinearVelocity(b2Vec2(this->GetBody()->GetLinearVelocity().x, this->_getAttr("rejump").asFloat()));
			}
			else {
				this->ApplyLinearImpulse(Vector2(0, this->_getAttr("force").asFloat()), Vector2(0, 0));
			}
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
			if (this->_grounds.size() == 0)
				this->_isJump++;
		}
	}
	else if (status == 0) {
		this->_canJump = true;
	}
	return ;
}

/**
 * Called when pressing the UP button
 * @param: status (int)
 */

void	Characters::_up(int status) {
	if (status == 1)
		this->_orientation = UP;
}

/**
 * Called when pressing the DOWN button
 * @param: status (int)
 */

void	Characters::_down(int status) {
	if (status == 1)
		this->_orientation = DOWN;
}

/**
 * Attack action
 * @param: status (int)
 */
void	Characters::_attack(int status) {
	this->_setCategory("attack");

	if (status == 1 && this->_weapon->attackReady() == 1) {
		this->_isAttacking = 1;
		this->_weapon->attack(this);
	}
}

void	Characters::_pickupItem(int status) {
	if (this->_item == nullptr)
		return;
	this->equipWeapon(static_cast<Equipment*>(this->_item)->getWeapon());
	this->_item = nullptr;
	theSwitchboard.Broadcast(new Message("DeleteEquipment"));
}

void	Characters::equipWeapon(Weapon* weapon) {
		this->_weapon = new Weapon(weapon);
}

void						Characters::setHP(int hp) {
	if (hp > this->_maxHp)
		this->_hp = this->_maxHp;
	else
		this->_hp = hp;
};

void						Characters::_destroyEnemy(void) {
	theSwitchboard.UnsubscribeFrom(this, "destroyEnemy");
	Game::addToDestroyList(this);
}

Characters::Orientation		Characters::getOrientation(void) { return this->_orientation; }
std::string					Characters::getLastAction(void) { return this->_lastAction; };
int							Characters::getHP(void) { return this->_hp; };
void						Characters::changeCanMove(void) { this->_canMove = (this->_canMove ? false : true); };
