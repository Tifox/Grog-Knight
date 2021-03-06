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
 * File: Equipment.cpp
 * Creation: 2015-03-06 15:51
 * Manon Budin <mbudin@student.42.fr>
 */

#include "Equipment.hpp"


//! Basic constructor
/**
 * The basic constructor.
 * In this constructor, we get the sprite and InitPhysics.
 * @todo The position is actually in the hard-way. Config file needed.
 */
Equipment::Equipment(void): Object() {
	this->addAttribute("type2", "Equipment");
	this->SetPosition(5, -12);
	this->_name = "TBD";
	this->_weapon = new Weapon(Game::wList->getWeapon("Bow"));
	this->SetSprite(this->_weapon->getSprite());
	theSwitchboard.SubscribeTo(this, "DeleteEquipment");
	this->InitPhysics();
	theWorld.Add(this);
}

//! Constructor called when an enemy dies
/**
 * This constructor positions the object based on the enemy
 * @param c (A Characters object)
 */
Equipment::Equipment(Characters* c): Object() {
	this->addAttribute("type2", "Equipment");
	this->SetPosition(c->GetBody()->GetWorldCenter().x, c->GetBody()->GetWorldCenter().y);
	this->_weapon = new Weapon(Game::wList->getWeaponRandom());
	this->SetSprite(this->_weapon->getSprite());
	this->SetName("loot");
	theSwitchboard.SubscribeTo(this, "DeleteEquipment" + this->GetName());
	this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	Game::bodiesToCreate.push_back(this);
}

Equipment::Equipment(Weapon *w, Characters* c): Object() {
	this->addAttribute("type2", "Equipment");
	this->addAttribute("type3", "Weapon");
	this->addAttribute("hitbox", "heroHitbox");
	this->addAttribute("hitboxType", "special");
	this->SetPosition(c->GetBody()->GetWorldCenter().x, c->GetBody()->GetWorldCenter().y);
	this->_weapon = new Weapon(w);
	this->SetLayer(15);
	this->_ring = nullptr;
	this->_armor = nullptr;
	this->_name = w->getName();
	this->addAttribute("name", w->getName());
	this->_displayName = w->getDisplayName();
	this->addAttribute("displayName", w->getDisplayName());
	this->_flavor = w->getFlavor();
	this->addAttribute("flavor", w->getFlavor());
	this->_equipable = w->getEquipable();
	this->addAttribute("equipable", w->getEquipable());
	this->SetSprite(this->_weapon->getSprite());
	this->SetName("loot");
	theSwitchboard.SubscribeTo(this, "DeleteEquipment" + this->GetName());
	this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	Game::bodiesToCreate.push_back(this);
}


Equipment::Equipment(Armor *w, Characters* c): Object() {
  	this->addAttribute("type2", "Equipment");
	this->addAttribute("type3", "Armor");
	this->addAttribute("hitbox", "heroHitbox");
	this->addAttribute("hitboxType", "special");
	this->SetPosition(c->GetBody()->GetWorldCenter().x, c->GetBody()->GetWorldCenter().y);
	this->_armor = new Armor(w);
	this->SetLayer(15);
	this->_weapon = nullptr;
	this->_ring = nullptr;
	this->_name = w->getName();
	this->addAttribute("name", w->getName());
	this->_displayName = w->getDisplayName();
	this->addAttribute("displayName", w->getDisplayName());
	this->_flavor = w->getFlavor();
	this->addAttribute("flavor", w->getFlavor());
	this->SetSprite(this->_armor->getSprite());
	this->SetName("loot");
	if (w->getAttribute("hpBuff") != "")
		this->addAttribute("hpBuff", w->getAttribute("hpBuff"));
	theSwitchboard.SubscribeTo(this, "DeleteEquipment" + this->GetName());
	this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	Game::bodiesToCreate.push_back(this);
}

Equipment::Equipment(Ring *w, Characters* c): Object() {
	this->addAttribute("type2", "Equipment");
	this->addAttribute("type3", "Ring");
	this->addAttribute("hitbox", "heroHitbox");
	this->addAttribute("hitboxType", "special");
	this->SetPosition(c->GetBody()->GetWorldCenter().x, c->GetBody()->GetWorldCenter().y);
	this->_ring = new Ring(w);
	this->SetLayer(15);
	this->_armor = nullptr;
	this->_weapon = nullptr;
	this->_name = w->getName();
	this->addAttribute("name", w->getName());
	this->_displayName = w->getDisplayName();
	this->addAttribute("displayName", w->getDisplayName());
	this->_flavor = w->getFlavor();
	this->addAttribute("flavor", w->getFlavor());
	this->SetSprite(this->_ring->getSprite());
	this->SetName("loot");
	if (w->getAttribute("hpBuff") != "")
		this->addAttribute("hpBuff", w->getAttribute("hpBuff"));
	theSwitchboard.SubscribeTo(this, "DeleteEquipment" + this->GetName());
	this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	Game::bodiesToCreate.push_back(this);
}

//! Destructor
/*
 * Basic Destructor
 */
Equipment::~Equipment(void) {
}

//! Collision begin callback
/**
 * The contact begin function
 * /!\ This function is actually called just BEFORE a collision
 * @param elem The Elements who collide with.
 * @param contact The Box2D contact object.
 */
void	Equipment::BeginContact(Elements *elem, b2Contact *contact) {
	if (!elem)
		return ;
	if (elem->getAttribute("type") != "ground" || elem->getAttribute("speType") == "spikes") {
		contact->SetEnabled(false);
		contact->enableContact = false;
	} else if (elem->getAttribute("physic") != "") {
		if (this->GetBody()->GetWorldCenter().y - 1 > elem->GetBody()->GetWorldCenter().y) {
			theSwitchboard.SubscribeTo(this, "setToStatic" + this->GetName());
			theSwitchboard.Broadcast(new Message("setToStatic" + this->GetName()));
			this->GetBody()->GetFixtureList()->SetDensity(0);
			this->GetBody()->ResetMassData();
		}
	}
}

//! End of collision callback
/**
 * End of contact function
 * /!\ This function is called just after the collision is over,
 * @param elem The other element
 * @param contact The Box2D contact object
 */
void	Equipment::EndContact(Elements *elem, b2Contact *contact) {
}

/*GETTERS*/
Weapon*		Equipment::getWeapon(void) { return this->_weapon; }

Armor*		Equipment::getArmor(void) { return this->_armor; }

Ring*		Equipment::getRing(void) { return this->_ring; }

std::string	Equipment::getName(void) { return this->_name; }

int			Equipment::getPrice(void) { return this->_price; }

//! Intern broadcasts function.
/**
 * The Receive Message function.
 * @param m The Broadcasted message. (See Angel2D docs for more info.)
 */
void		Equipment::ReceiveMessage(Message *m) {
	if (m->GetMessageName() == "DeleteEquipment" + this->GetName()) {
		if (this->_ring != nullptr) {
			Game::addToDestroyList(this->_ring);
		} else if (this->_armor != nullptr) {
			Game::addToDestroyList(this->_armor);
		} else if (this->_weapon != nullptr) {
			Game::addToDestroyList(this->_weapon);
		}
		this->ChangeColorTo(Color(1,1,1,0), 0);
		Game::addToDestroyList(this);
	}
	if (m->GetMessageName() == "setToStatic" + this->GetName()) {
		this->GetBody()->SetType(b2_staticBody);
	}
}
