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

//!Constructor called by the weaponlist class to parse all weapons
/**
 * Default constructor, using the element that called the attack
 * @param name (std::string)
 */
Weapon::Weapon(std::string name) : _name(name) {
	this->_readFile(name);
}

//!Constructor called by hero/equipment class, to copy a parsed version from weaponlist
/**
 * Copy constructor
 * @param weapon (Weapon*)
 */

Weapon::Weapon(Weapon* weapon) {
	this->_name = weapon->getName();
	this->addAttribute("name", this->_name);
	this->_displayName = weapon->getDisplayName();
	this->addAttribute("displayName", this->_displayName);
	this->SetLayer(70);
	this->_flavor = weapon->getFlavor();
	this->addAttribute("flavor", this->_flavor);
	this->_equipable = weapon->getEquipable();
	this->addAttribute("equipable", this->_equipable);
	this->_damage = weapon->getDamage();
	this->_recovery = weapon->getRecovery();
	this->_active = weapon->getActive();
	this->_size = weapon->getSize();
	this->_attack = weapon->getAttack();
	this->_pushback = weapon->getPushback();
	this->_sprite = weapon->getSprite();
	this->_lootLevel = weapon->getLootLevel();
	this->_type = weapon->getType();
	this->addAttribute("sprite", this->_sprite);
	this->addAttribute("type3", "Weapon");
	this->_critRate = weapon->getCritRate();
}

//! Constructor called when stomping, create a hitbox on each side of the char
/**
 * Hitbox for the stomp
 * @param w the weapon currently used to deal the damage
 * @param c the character for the localisation
 * @param i left or right spawn
 */

Weapon::Weapon(Weapon* w, Characters* c, int i, std::string str) {
	this->_name = w->getName();
	this->_displayName = w->getDisplayName();
	this->_flavor = w->getFlavor();
	this->_damage = w->getDamage();
	this->_recovery = w->getRecovery();
	this->_active = w->getActive();
	this->_size = w->getSize();
	this->_attack = w->getAttack();
	this->_pushback = w->getPushback();
	this->_equipable = w->getEquipable();
	this->SetSize(1);
	this->_critRate = w->getCritRate();
	this->SetName("HeroWeaponHitbox");
	if (c->getAttributes()["type"] == "Hero")
		this->addAttribute("type", "HeroWeaponHitBox");
	else
		this->addAttribute("type", "WeaponHitBox");
	this->SetDrawShape(ADS_Square);
	this->SetColor(1, 1, 1, 1);
	this->addAttribute("physic", "1");
	this->_hitboxType = "special";
	this->_hitbox = "octogonHitbox";
	this->SetDensity(0);
	this->SetFixedRotation(true);
	this->Tag("weaponhitbox");
	this->SetIsSensor(true);
	theSwitchboard.SubscribeTo(this, "deleteWeapon" + this->GetName());
	theSwitchboard.DeferredBroadcast(new Message("deleteWeapon" + this->GetName()), w->getActive());
	this->SetPosition(c->GetBody()->GetWorldCenter().x + i, c->GetBody()->GetWorldCenter().y);
	Game::bodiesToCreate.push_back(this);
}

//! Constructor called when stomping, create a hitbox on each side of the char
/**
 * Hitbox for the stomp
 * @param w the weapon currently used to deal the damage
 * @param c the character for the localisation
 * @param i left or right spawn
 */

Weapon::Weapon(Weapon* w, Characters* c, int i) {
	this->_name = w->getName();
	this->_displayName = w->getDisplayName();
	this->_flavor = w->getFlavor();
	this->_damage = w->getDamage();
	this->_recovery = w->getRecovery();
	this->_active = w->getActive();
	this->_size = w->getSize();
	this->_attack = w->getAttack();
	this->_pushback = w->getPushback();
	this->_equipable = w->getEquipable();
	this->SetSize(1);
	this->_critRate = w->getCritRate();
	this->SetName("HeroWeaponHitbox");
	if (c->getAttributes()["type"] == "Hero")
		this->addAttribute("type", "HeroWeaponHitBox");
	else
		this->addAttribute("type", "WeaponHitBox");
	this->SetDrawShape(ADS_Square);
	this->SetColor(1, 1, 1, 1);
	this->addAttribute("physic", "1");
	this->_hitboxType = "special";
	this->_hitbox = "octogonHitbox";
	this->SetDensity(0.0001);
	this->SetFixedRotation(true);
	this->Tag("weaponhitbox");
	this->SetIsSensor(true);
	theSwitchboard.SubscribeTo(this, "deleteWeapon" + this->GetName());
	theSwitchboard.DeferredBroadcast(new Message("deleteWeapon" + this->GetName()), w->getActive());
	this->SetPosition(c->GetBody()->GetWorldCenter().x + i, c->GetBody()->GetWorldCenter().y);
	this->_initDirection2(w, c, i);
	theWorld.Add(this);
}

void	Weapon::_initDirection2(Weapon* w, Characters* c, int i) {
	float xDecal = 0;
	float yDecal = 0;
	float xjoint1 = 0;
	float xjoint2 = 0;
	float yjoint1 = 0;
	float yjoint2 = 0;
	if (i == 1 || i == -1) {
		yjoint1 = 0.4f;
		yjoint2 = -0.4f;
		xDecal = -0.5f;
		if (i == 1) {
			xDecal = 0.5f;
		}
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

//! Constructor called when attacking, creates the defined hitbox
/**
 * Constructor for creating the weapon area-of-effect
 * @param w Weapon*
 * @param c (Characters*)
 */

Weapon::Weapon(Weapon* w, Characters* c) {
	this->_name = w->getName();
	this->_displayName = w->getDisplayName();
	this->_flavor = w->getFlavor();
	this->_damage = w->getDamage();
	this->_recovery = w->getRecovery();
	this->_active = w->getActive();
	this->_size = w->getSize();
	this->_attack = w->getAttack();
	this->_pushback = w->getPushback();
	this->_equipable = w->getEquipable();
	this->SetSize(1);
	this->_critRate = w->getCritRate();
	this->SetName("HeroWeaponHitbox");
	if (c->getAttributes()["type"] == "Hero")
		this->addAttribute("type", "HeroWeaponHitBox");
	else
		this->addAttribute("type", "WeaponHitBox");
	this->SetDrawShape(ADS_Square);
	this->SetColor(1, 1, 1, 0);
	this->addAttribute("physic", "1");
	this->_hitboxType = "special";
	this->_hitbox = "octogonHitbox";
	this->SetDensity(0.001f);
	this->SetFixedRotation(true);
	this->Tag("weaponhitbox");
	this->SetIsSensor(true);
	theSwitchboard.SubscribeTo(this, "deleteWeapon" + this->GetName());
	theSwitchboard.DeferredBroadcast(new Message("deleteWeapon" + this->GetName()), w->getActive());
	theSwitchboard.DeferredBroadcast(new Message("attackReady"), w->getRecovery());
	this->_initDirection(w, c);
	theWorld.Add(this);
}

//!Function containing the details related to the attack direction
/**
 * This function is called when an attack starts, to attach the hitbox body to the character
 * @param w Weapon* (the current weapon)
 * @param c Characters* (the character manipulating the weapon)
 */

void	Weapon::_initDirection(Weapon* w, Characters* c) {
	float xDecal = 0;
	float yDecal = 0;
	float xjoint1 = 0;
	float xjoint2 = 0;
	float yjoint1 = 0;
	float yjoint2 = 0;
	if (c->getOrientation() == Characters::RIGHT ||
		c->getOrientation() == Characters::LEFT) {
		yjoint1 = 0.4f;
		yjoint2 = -0.4f;
		xDecal = -0.5f;
		if (c->getOrientation() == Characters::RIGHT) {
			xDecal = 0.5f;
		}
	} else if (c->getOrientation() == Characters::UP ||
			   c->getOrientation() == Characters::DOWN) {
		xjoint1 = 0.4f;
		xjoint2 = -0.4f;
		yDecal = 0.5f;
		if (c->getOrientation() == Characters::DOWN) {
			yDecal = -0.5f;
		}
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
//! Setter

void	Weapon::setActive(float f) {
	this->_active = f;
}


//! Basic destructor

Weapon::~Weapon(void) {
}

//!Json parsing for the weapon files (differs a bit from the Characters, that's why we have both)
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

//!Follow from the json parser, stocks value
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
		Log::warning("The class name is different with the name in the config file: " + this->_name + "/" + json["infos"].get("name", "").asString());
	this->_name = json["infos"].get("name", "").asString();
	this->_displayName = json["infos"].get("displayName", "").asString();
	this->_price = json["infos"].get("price", "").asInt();
	this->_flavor = json["infos"].get("flavor", "").asString();
	this->_active = json["infos"].get("active", "").asFloat();
	this->_recovery = json["infos"].get("recovery", "").asFloat();
	this->_lootLevel = json["infos"].get("lootLevel", "").asInt();
	this->_size = json["infos"].get("size", "").asFloat();
	this->_damage = json["infos"].get("damage", "").asFloat();
	this->_pushback = json["infos"].get("pushback", "").asFloat();
	this->_attack = json["infos"].get("attack", "").asString();
	this->_sprite = json["infos"].get("sprites", "").asString();
	this->_critRate = json["infos"].get("critRate", "").asInt();
	this->_type = json["infos"].get("type", "").asString();
	this->_equipable = json["infos"].get("equipable", "").asString();
	if (json["infos"].get("starterLevel", "").isConvertibleTo(Json::ValueType::intValue))
		this->_starterLevel = json["infos"].get("starterLevel", "").asInt();
	else
		this->_starterLevel = -1;
	this->addAttribute("sprite", this->_sprite);
	this->addAttribute("type3", "Weapon");
}

//! Function called to get an attr value from the parsed json
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

//! Function called when a character attacks, to create the hitbox depending if its ranged or melee
/**
 * Called by the Character to attack with the currently equipped weapon
 * @param: x (Int)
 * @param: y (Int)
 * @param: orientationX (int)
 * @param: orientationY (int)
 * @param: linearVelocity (b2Vec2)
 */
void	Weapon::attack(Characters *c) {
	if (this->_attack == "melee") {
		new Weapon(this, c);
	}
	else if (this->_attack == "ranged")
		new Projectile(this, c);
}

void	Weapon::ReceiveMessage(Message *m) {
	if (m->GetMessageName() == "DeleteEquipment" + this->GetName()) {
		Game::addToDestroyList(this);
	}
	if (m->GetMessageName() == "deleteWeapon" + this->GetName()) {
		Game::addToDestroyList(this);
		theSwitchboard.UnsubscribeFrom(this, "deleteWeapon");
		theSwitchboard.Broadcast(new Message("disableAttackHitbox"));
	}
	if (m->GetMessageName() == "setToStatic" + this->GetName()) {
		this->GetBody()->SetType(b2_staticBody);
	}
}

/* GETTERS */
std::string		Weapon::getName(void)      { return this->_name; }
std::string		Weapon::getDisplayName(void)      { return this->_displayName; }
std::string		Weapon::getFlavor(void)    { return this->_flavor; }
std::string		Weapon::getSprite(void)    { return this->_sprite; }
std::string		Weapon::getAttack(void)    { return this->_attack; }
std::string 	Weapon::getType(void)	   { return this->_type; }
std::string 	Weapon::getEquipable(void)	   { return this->_equipable; }
int				Weapon::getLootLevel(void) { return this->_lootLevel; }
int				Weapon::getPrice(void) 	   { return this->_price; }
float			Weapon::getActive(void)    { return this->_active; }
int				Weapon::getSize(void)      { return this->_size; }
int				Weapon::getDamage(void)	   { return this->_damage; }
int				Weapon::getPushback(void)  { return this->_pushback; }
float			Weapon::getRecovery(void)  { return this->_recovery; }
int				Weapon::getCritRate(void)  { return this->_critRate; }
int				Weapon::getStarterLevel(void)  { return this->_starterLevel; }

/* SETTERS */


void	Weapon::BeginContact(Elements *elem, b2Contact *contact) {
}

void	Weapon::EndContact(Elements *elem, b2Contact *contact) {
}

