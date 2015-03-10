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
	this->SetFriction(1.0f);
	this->SetRestitution(0.0f);
	this->SetFixedRotation(true);
	this->_orientation = RIGHT;
	this->_readFile(name);
	this->_canMove = true;
	this->_invincibility = false;
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
	this->addAttribute("spritesFrame", json["infos"].get("sprites", "").asString());

	for (i = json["Actions"].begin(); i != json["Actions"].end(); i++) {
		for (v = (*i).begin(); v != (*i).end(); v++) {
			tmp[v.key().asString()] = (*v);
			this->_attr[i.key().asString()] = tmp;
			// Subcribe to the broadcasts
			if (v.key().asString() == "subscribe") {
				Log::info("SubscribeTo " + (*v).asString());
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

	Log::info(m->GetMessageName());
	if (m->GetMessageName() == "canMove") {
		this->_canMove = true;
		return;
	}
	else if (m->GetMessageName() == "endInvincibility") {
		this->_invincibility = false;
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
		if (this->_isRunning == 0) {
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
				this->_getAttr("beginFrame").asInt(),
				this->_getAttr("endFrame").asInt(), "base");
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
	if (this->getAttributes()["type"] != "Hero")
		return;
	if (elem->getAttributes()["type"] == "ground") {
		if (this->GetBody()->GetWorldCenter().y - 1 >= elem->GetBody()->GetWorldCenter().y) {
			if (this->_grounds.size() > 0)
				contact->SetEnabled(false);
			if (this->_isJump > 0) {
				this->_isJump = 0;
				this->PlaySpriteAnimation(0.1f, SAT_OneShot, this->_getAttr("jump", "endFrame").asInt(),
										  this->_getAttr("jump", "endFrame").asInt(), "base");
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
	this->_grounds.remove(elem);
	this->_wallsLeft.remove(elem);
	this->_wallsRight.remove(elem);
	if (elem->getAttributes()["type"] == "ground") {
		if (this->GetBody()->GetWorldCenter().y >= elem->GetBody()->GetWorldCenter().y + 1) {
			if (this->_grounds.size() == 0) {
				this->_isJump++;
// 				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
// 										  this->_getAttr("jump", "fallingFrame").asInt(),
// 										  this->_getAttr("jump", "endFallingFrame").asInt() - 1, "jump");
			}
		}
		else {
		}
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
	if (status == 1){
		this->_orientation = RIGHT;
		if (this->GetSpriteFrame() < this->_getAttr("beginFrame").asInt() && !this->_isJump)
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
				this->_getAttr("beginFrame").asInt(),
				this->_getAttr("endFrame").asInt());
		Game::startRunning(this);
		if (this->_isRunning == 2)
			this->GetBody()->SetLinearVelocity(b2Vec2(this->_getAttr("force").asFloat(), this->GetBody()->GetLinearVelocity().y));
		this->_isRunning = 1;
	} else if (status == 0) {
		this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
		Game::stopRunning(this);
		this->_isRunning = 0;
		if (!this->_isJump)
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
		if (this->GetSpriteFrame() < this->_getAttr("beginFrame").asInt() && !this->_isJump)
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
				this->_getAttr("beginFrame").asInt(),
				this->_getAttr("endFrame").asInt());
		Game::startRunning(this);
		if (this->_isRunning == 1)
			this->GetBody()->SetLinearVelocity(b2Vec2(-this->_getAttr("force").asFloat(), this->GetBody()->GetLinearVelocity().y));
		this->_isRunning = 2;
	} else if (status == 0) {
		this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
		if (!this->_isJump)
			this->AnimCallback("base");
		this->_isRunning = 0;
		Game::stopRunning(this);
	} else {
		if (this->GetBody()->GetLinearVelocity().x > -(this->_maxSpeed)) {
			std::cout << "backward launched" << std::endl;
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
	std::cout << this->_isJump << std::endl;

	if (status == 1) {
		if (this->_isJump == 0 || (this->_isJump <= 1 && this->_getAttr("double").asInt() == 1)) {
			if (this->_isJump >= 1) {
				this->GetBody()->SetLinearVelocity(b2Vec2(this->GetBody()->GetLinearVelocity().x, this->_getAttr("rejump").asFloat()));
			}
			else {
				this->ApplyLinearImpulse(Vector2(0, this->_getAttr("force").asFloat()), Vector2(0, 0));
			}
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
				this->_getAttr("beginFrame").asInt(),
				this->_getAttr("endFrame").asInt() - 1, "jump");
			if (this->_grounds.size() == 0)
				this->_isJump++;
		}
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
	if (status == 1 && this->_weapon->attackReady() == 1) {
		this->_weapon->attack(this);
	}
}

void	Characters::equipWeapon(Weapon* weapon) {
		this->_weapon = new Weapon(weapon);
}

Characters::Orientation		Characters::getOrientation(void) { return this->_orientation; }
std::string					Characters::getLastAction(void) { return this->_lastAction; };
