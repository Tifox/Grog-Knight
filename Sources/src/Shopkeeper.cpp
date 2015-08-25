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
 * File: Shopkeeper.cpp
 * Creation: 2015-08-07 15:09
 * Vincent Rey <vrey@student.42.fr>
 */

# include "Shopkeeper.hpp"

//! Constructor
/**
 * This constructor is making some subscribtions for himself.
 */
Shopkeeper::Shopkeeper(std::string name) : Characters(name) {
	this->addAttribute("type", "Shopkeeper");
	theSwitchboard.SubscribeTo(this, "removeShopkeeperText");
	this->SetLayer(10);
	this->SetDensity(0);
	this->_shop = new Shop(0,0,2,3);
}

//! Destructor
/**
 * Basic Destructor
 */
Shopkeeper::~Shopkeeper(void) {
	return ;
}

//! Init Animation function
/**
 * This function making the first animation call.
 */
void	Shopkeeper::init(void) {
}

//! Spawn shopkeeper
/**
 * Spawns the shopkeeper when you enter the correct room
 *
 */
void	Shopkeeper::spawn(void) {
	//Here starts the game - parse the 1st map coordinates and hero start
	this->setXStart(Game::currentGame->maps->getMapXY()[Game::currentY][Game::currentX].getXMid() - 1);
	this->setYStart(Game::currentGame->maps->getMapXY()[Game::currentY][Game::currentX].getYMid() + 3);
	this->addAttribute("shopkeeper", "1");
	this->display();
	this->_shop->revealShop(Game::currentGame->maps->getMapXY()[Game::currentY][Game::currentX].getXMid() - 1, Game::currentGame->maps->getMapXY()[Game::currentY][Game::currentX].getYMid() + 3);
	this->_currentPhrase = "Greetings, traveler.";
	theSwitchboard.DeferredBroadcast(new Message("removeShopkeeperText"), 3);
	Game::getHUD()->setText(this->_currentPhrase, this, Vector3(255, 51, 255), 0, 0);
}

//! Begin collision function
/**
 * Collision begin callback
 * /!\ This function is called just before a collision
 * @param elem The other Element
 * @param contact The Box2D contact object
 */
void	Shopkeeper::BeginContact(Elements* elem, b2Contact *contact) {
	HUDWindow *hud = Game::getHUD();
	if (elem->getAttribute("type") != "ground") {
		contact->SetEnabled(false);
		contact->enableContact = false;
	}
}

//! End collision function
/**
 * End contact callback.
 * /!\ This function is actually called after a collision.
 * @param elem The other Element.
 * @param contact The Box2D contact object
 */
void	Shopkeeper::EndContact(Elements *elem, b2Contact *contact) {
}

void	Shopkeeper::displayText(std::string say) {
	HUDWindow* hud = Game::getHUD();
		hud->removeText(this->_currentPhrase);
		this->_currentPhrase = say;
		hud->setText(this->_currentPhrase, this, Vector3(255, 51, 255), 0, 0);
}

void	Shopkeeper::removeText(void) {
	Game::getHUD()->removeText(this->_currentPhrase);
}

void	Shopkeeper::ReceiveMessage(Message *m) {
	if (m->GetMessageName() == "removeShopkeeperText") {
		if (this->_currentPhrase == "A wise choice." || this->_currentPhrase == "Greetings, traveler.")
			this->removeText();
	}
}
