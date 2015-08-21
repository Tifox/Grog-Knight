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
 * File: Dealer`.cpp
 * Creation: 2015-08-07 15:09
 * Manon Budin <mbudin@student.42.fr> 
 */

# include "Shopkeeper.hpp"

//! Constructor
/**
 * This constructor is making some subscribtions for himself.
 */
Shopkeeper::Shopkeeper(std::string name) : Characters(name) {
	this->addAttribute("type", "Shopkeeper");
	this->SetLayer(10);
	this->_shop = new Shop(0,0,2,3);
	return ;
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

void	Shopkeeper::ReceiveMessage(Message *m) {
}
