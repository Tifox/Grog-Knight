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


/**
 * Basic Constructor
 */
Equipment::Equipment(void) {
	this->addAttribute("type2", "Equipment");
//	this->SetColor(0,1,1,1);
	this->SetPosition(5, -12);
	this->InitPhysics();
	theWorld.Add(this);
	this->_weapon = new Weapon(Game::wList->getWeapon("Bow"));
	std::cout << this->_weapon->getSprite() << std::endl;

	this->SetSprite(this->_weapon->getSprite());
	theSwitchboard.SubscribeTo(this, "DeleteEquipment");
}

/**
 * Collision begin callback
 * @param: elem (Elements *)
 * @param: contact (b2Contact *)
 * @note: This function is called just before a collision
 */
void	Equipment::BeginContact(Elements *elem, b2Contact *contact) {
	if (elem->getAttributes()["type"] == "Hero"){
		Game::getHUD()->setText(this->_weapon->getFlavor(), 450, 50);
	}
}

/**
 * End of contact
 * @param: elem (Elements *)
 * @param: contact (b2Contact *)
 */
void	Equipment::EndContact(Elements *elem, b2Contact *contact) {
	if (elem->getAttributes()["type"] == "Hero"){
		Game::getHUD()->removeText(this->_weapon->getFlavor());
	}
}

/*GETTERS*/
Weapon*		Equipment::getWeapon(void) { return this->_weapon; }

void		Equipment::ReceiveMessage(Message *m) {
	if (m->GetMessageName() == "DeleteEquipment")
		Game::addToDestroyList(this);
}

/*
 * Basic Destructor
 */
Equipment::~Equipment(void) {
	return;
}
