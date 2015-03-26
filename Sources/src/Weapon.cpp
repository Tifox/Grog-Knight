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
	this->_pushback = weapon->getPushback();
	this->_canAttack = 1;

	theSwitchboard.SubscribeTo(this, "canAttack");
}


/**
 * Constructor for creating the weapon area-of-effect
 * @param: w (Weapon*)
 * @param: c (Characters*)
 */

Weapon::Weapon(Weapon* w, Characters* c) {
	this->_name = w->getName();
	this->_flavor = w->getFlavor();
	this->_damage = w->getDamage();
	this->_recovery = w->getRecovery();
	this->_active = w->getActive();
	this->_size = w->getSize();
	this->_attack = w->getAttack();
	this->_pushback = w->getPushback();
	this->_canAttack = 1;
	this->SetSize(w->getSize());
	this->SetName("HeroWeaponHitbox");
	if (c->getAttributes()["type"] == "Hero")
		this->addAttribute("type", "HeroWeaponHitBox");
	else
		this->addAttribute("type", "WeaponHitBox");
	this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	this->SetDrawShape(ADS_Square);
	this->SetColor(1,1,1);
	this->SetDensity(0.0001);
	this->SetFriction(0);
	this->SetRestitution(0.0f);
	this->SetFixedRotation(true);
	this->Tag("weaponhitbox");
	this->SetIsSensor(true);
	theSwitchboard.SubscribeTo(this, "deleteWeapon");
	theSwitchboard.DeferredBroadcast(new Message("deleteWeapon"), w->getActive());
	theSwitchboard.DeferredBroadcast(new Message("canAttack"), w->getRecovery());
	this->_initDirection(w, c);
	theWorld.Add(this);
}

void	Weapon::_initDirection(Weapon* w, Characters* c) {
	float xDecal = 0;
	float yDecal = 0;
	float xjoint1 = 0;
	float xjoint2 = 0;
	float yjoint1 = 0;
	float yjoint2 = 0;
	if (c->getOrientation() == Characters::RIGHT) {
		xDecal = 1;
		yDecal = 0;
		xjoint1 = 0.5;
		yjoint1 = 0.5;
		xjoint2 = 0.5;
		yjoint2 = -0.5;
	} else if (c->getOrientation() == Characters::LEFT) {
		xDecal = -1;
		yDecal = 0;
		xjoint1 = -0.5;
		yjoint1 = 0.5;
		xjoint2 = -0.5;
		yjoint2 = -0.5;
	} else if (c->getOrientation() == Characters::UP) {
		xDecal = 0;
		yDecal = 1;
		xjoint1 = 0.5;
		yjoint1 = 0.5;
		xjoint2 = -0.5;
		yjoint2 = 0.5;
	} else if (c->getOrientation() == Characters::DOWN) {
		xDecal = 0;
		yDecal = -1;
		xjoint1 = -0.5;
		yjoint1 = -0.5;
		xjoint2 = 0.5;
		yjoint2 = -0.5;
	}
	this->SetPosition(c->GetBody()->GetWorldCenter().x + xDecal, c->GetBody()->GetWorldCenter().y + yDecal);
	this->InitPhysics();
	this->GetBody()->SetBullet(true);
	b2DistanceJointDef jointDef1;
	jointDef1.Initialize(c->GetBody(), this->GetBody(), b2Vec2(c->GetBody()->GetWorldCenter().x + xjoint1, c->GetBody()->GetWorldCenter().y + yjoint1),
						 this->GetBody()->GetWorldCenter());
	jointDef1.collideConnected = false;
	b2DistanceJointDef jointDef2;
	jointDef2.Initialize(c->GetBody(), this->GetBody(), b2Vec2(c->GetBody()->GetWorldCenter().x + xjoint2, c->GetBody()->GetWorldCenter().y + yjoint2),
						 this->GetBody()->GetWorldCenter());
	jointDef2.collideConnected = false;
	b2DistanceJoint *joint1 = (b2DistanceJoint*)theWorld.GetPhysicsWorld().CreateJoint(&jointDef1);
	b2DistanceJoint *joint2 = (b2DistanceJoint*)theWorld.GetPhysicsWorld().CreateJoint(&jointDef2);
}

/**
 * Basic destructor
 */
Weapon::~Weapon(void) {
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
	this->_pushback = json["infos"].get("pushback", "").asFloat();
	this->_attack = json["infos"].get("attack", "").asString();
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
void	Weapon::attack(Characters *c) {
	this->_canAttack = 0;
	if (this->_attack == "melee")
		new Weapon(this, c);
	else if (this->_attack == "ranged")
		new Projectile(this, c);
}

void	Weapon::ReceiveMessage(Message *m) {
	if (m->GetMessageName() == "deleteWeapon") {
		Game::addToDestroyList(this);
		theSwitchboard.UnsubscribeFrom(this, "deleteWeapon");
	}
	if (m->GetMessageName() == "canAttack")
		this->_canAttack = 1;
}

/* GETTERS */
std::string		Weapon::getName(void) { return this->_name; }
std::string		Weapon::getFlavor(void) { return this->_flavor; }
std::string		Weapon::getAttack(void) { return this->_attack; }
float			Weapon::getActive(void) { return this->_active; }
int				Weapon::getSize(void) { return this->_size; }
int				Weapon::getDamage(void) { return this->_damage; }
int				Weapon::getPushback(void) { return this->_pushback; }
float			Weapon::getRecovery(void) { return this->_recovery; }
int				Weapon::attackReady(void) { return this->_canAttack; }

void	Weapon::BeginContact(Elements *elem, b2Contact *contact) {
}

void	Weapon::EndContact(Elements *elem, b2Contact *contact) {
}

