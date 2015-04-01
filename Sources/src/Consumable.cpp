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
 * File: Consumable.cpp
 * Creation: 2015-03-06 15:37
 * Manon Budin <mbudin@student.42.fr>
 */

#include "Consumable.hpp"


//! Basic Constructor
/**
 * Basic Constructor
 * This constructor sets a lots of attributes, like the sprite, the position. etc...
 * That also init the physics.
 * @todo: This function setting attributes the hard-way. Maybe a name based config, like Characters,
 * would be better ?
 */
Consumable::Consumable(void) {
	this->addAttribute("type2", "Consumable");
	this->addAttribute("type3", "HP");
	this->addAttribute("value", "50");
	this->SetSprite("Resources/Images/HUD/heart.png");
	this->SetPosition(9, -4);
	this->InitPhysics();
	theWorld.Add(this);
}

//! Collision begin callback
/**
 * Collision begin function
 * /!\ This function is called just before a collision
 * @param: elem The Elements who collide.
 * @param: contact The Box2D contact object. See Box2D docs for more information.
 * @todo: This function is actually doing nothing.
 */
void	Consumable::BeginContact(Elements *elem, b2Contact *contact) {
	// if (elem->getattributes()["type"] == "hero"){
	// 	game::bodiestodestroy.push_back(this);
	// 	contact->setenabled(false);
	// }
}

//! Destructor
/*
 * Basic Destructor
 */
Consumable::~Consumable(void) {
	return;
}
