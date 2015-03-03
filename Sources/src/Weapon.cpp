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
 * File: Weapon.cpp
 * Creation: 2015-02-18 14:00
 * Vincent Rey <vrey@student.42.fr>
 */

# include "Weapon.hpp"


/**
 * Default constructor, using the element that called the attack
 * @param: name (std::string)
 */
Weapon::Weapon(std::string name) : _name(name) {
	theSwitchboard.SubscribeTo(this, "deleteWeapon");
	theSwitchboard.SubscribeTo(this, "canAttack");
	this->_readFile(name);
	this->_canAttack = 1;
}

/**
 * Copy constructor
 * @param weapon (Weapon*)
 */

Weapon::Weapon(Weapon* weapon) {
	this->_name = weapon->getName();
	this->_flavor = weapon->getFlavor();
	this->_damage = weapon->getDamage();
	this->_recovery = weapon->getRecovery();
	this->_active = weapon->getActive();
	this->_size = weapon->getSize();
	this->_attack = weapon->getAttack();
}

/**
 * Hitbox area constructor
 * @param: weapon (Weapon *)
 */

Weapon::WeaponArea::WeaponArea(Weapon *w) {
	this->SetRestitution(0);
	this->SetDensity(1);
	this->SetFriction(1);
	this->SetFixedRotation(true);
	this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	this->SetPosition(1, 1);
	this->SetColor(1,1,1);
	this->SetDrawShape(ADS_Square);
	this->InitPhysics();
	this->GetBody()->SetLinearVelocity(b2Vec2(0,0));
//	theWorld.Add(this);
}

/**
 * Basic Destructor
 */

Weapon::WeaponArea::~WeaponArea(void) {
	return;
}

/**
 * Basic destructor
 */
Weapon::~Weapon(void) {
	return;
}

/**
 * Read a config file, base on the name of the class
 * @param: name (std::string)
 */
void	Weapon::_readFile(std::string name) {
	std::string			file;
	std::stringstream	buffer;
	std::ifstream		fd;

	file = "./Resources/Elements/Weapons/" + name + ".json";
	fd.open(file.c_str());
	if (!fd.is_open())
		Log::error("Can't open the file for the " +
				   name + " class. (Expected path is Resources/Elements/" + name +".json)");
	buffer << fd.rdbuf();
	this->_parseJson(buffer.str());
}

/**
 * Parse, read and stock the info in the config file
 * @param: file (std::string)
 * @note: file is the whole file content
 */
void    Weapon::_parseJson(std::string file) {
	Json::Reader    read;
	Json::Value     json;
	Json::ValueIterator i, v;
	std::map<std::string, Json::Value>  tmp;

	if (!read.parse(file, json))
		Log::error("Error in json syntax :\n" + read.getFormattedErrorMessages());
	if (this->_name != json["infos"].get("name", "").asString())
		Log::warning("The weapon name is different with the name in the config file.");
	this->_name = json["infos"].get("name", "").asString();
	this->_flavor = json["infos"].get("flavor", "").asString();
	this->_active = json["infos"].get("active", "").asFloat();
	this->_recovery = json["infos"].get("recovery", "").asFloat();
	this->_size = json["infos"].get("size", "").asFloat();
	this->_damage = json["infos"].get("damage", "").asFloat();
	this->_attack = json["infos"].get("damage", "").asString();
}

/**
 * Get a Json::Value of a key in the config file
 * @param: category (std::string)
 * @param: key (std::string)
 * @note: See the docs for the utilisation of Json::Value
 */
Json::Value     Weapon::_getAttr(std::string category, std::string key) {
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
 * Called by the Character to attack with the currently equipped weapon
 * @param: x (Int)
 * @param: y (Int)
 * @param: orientationX (int)
 * @param: orientationY (int)
 * @param: linearVelocity (b2Vec2)
 */
void	Weapon::attack(int x, int y, int orientationX, int orientationY, b2Vec2 linearVelocity) {
	this->_attackBox = new WeaponArea(this);
}

void	Weapon::ReceiveMessage(Message *m) {
}

/* GETTERS */
std::string		Weapon::getName(void) { return this->_name; }
std::string		Weapon::getFlavor(void) { return this->_flavor; }
std::string		Weapon::getAttack(void) { return this->_attack; }
int				Weapon::getActive(void) { return this->_active; }
int				Weapon::getSize(void) { return this->_size; }
int				Weapon::getDamage(void) { return this->_damage; }
int				Weapon::getRecovery(void) { return this->_recovery; }

void	Weapon::BeginContact(Elements *elem, b2Contact *contact) {
}

void	Weapon::EndContact(Elements *elem, b2Contact *contact) {
}

