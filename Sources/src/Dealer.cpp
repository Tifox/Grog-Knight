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
 * File: Dealer.cpp
 * Creation: 2015-08-07 15:09
 * Manon Budin <mbudin@student.42.fr> 
 */

# include "Dealer.hpp"

//! Constructor
/**
 * This constructor is making some subscribtions for himself.
 */
Dealer::Dealer(std::string name) : Characters(name) {
	theSwitchboard.SubscribeTo(this, "enterPressed");
	this->addAttribute("type", "Dealer");
	this->SetLayer(10);
	return ;
}

//! Destructor
/**
 * Basic Destructor
 */
Dealer::~Dealer(void) {
	return ;
}

//! Init Animation function
/**
 * This function making the first animation call.
 */
void	Dealer::init(void) {
	this->AnimCallback("base");
}


//! Begin collision function
/**
 * Collision begin callback
 * /!\ This function is called just before a collision
 * @param elem The other Element
 * @param contact The Box2D contact object
 */
void	Dealer::BeginContact(Elements* elem, b2Contact *contact) {
	HUDWindow *hud = Game::getHUD();
	if (elem->getAttribute("type") != "ground") {
		contact->SetEnabled(false);
		contact->enableContact = false;
	}
	if (elem->getAttribute("type") == "Hero") {
		this->PlaySpriteAnimation(this->_getAttr("give", "time").asFloat(), SAT_Loop,
								  this->_getAttr("give", "beginFrame").asInt(),
								  this->_getAttr("give", "endFrame").asInt(), "base");
		//	hud->setText("HI BUD YOU WANNA SOME DRUGS?", Game::currentGame->getHero(), Vector3(255, 51, 255), 0, 1);
		return; 
	}
}

//! End collision function
/**
 * End contact callback.
 * /!\ This function is actually called after a collision.
 * @param elem The other Element.
 * @param contact The Box2D contact object
 */
void	Dealer::EndContact(Elements *elem, b2Contact *contact) {
		HUDWindow *hud = Game::getHUD();
		if (elem->getAttribute("type") == "Hero") {
			this->AnimCallback("base");
			hud->removeText("HI BUD YOU WANNA SOME DRUGS?");
			hud->removeText("Here you go buddy. See ya.");
			return;
		}
}

void	Dealer::ReceiveMessage(Message *m) {
///	std::list<std::string>::iterator	it;
		HUDWindow *hud = Game::getHUD();

//	if (this->_inMenu == 1) {
		if (m->GetMessageName() == "enterPressed") {
			this->AnimCallback("base");
			hud->removeText("HI BUD YOU WANNA SOME DRUGS?");
			Drug *drug = new Drug(Game::dList->getDrugRandom());
		//	hud->setText("Here you go buddy. See ya.", Game::currentGame->getHero(), Vector3(255, 51, 255), 0, 1);
		}
	//}
}