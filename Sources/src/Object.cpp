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
 * File: Object.cpp
 * Creation: 2015-03-03 13:08
 * Manon Budin <mbudin@student.42.fr>
 */

#include "Object.hpp"

//! Constructor
/**
 * Basic Constructor
 */
Object::Object(void) {
	this->addAttribute("physic", "1");
	this->addAttribute("type", "Object");
	this->SetDensity(1);
	this->SetFriction(1.0f);
	this->SetRestitution(0.5f);
	this->SetFixedRotation(true);
}

//! Overload from b2Body's BeginContact
/**
 * Collision begin callback
 * @param elem (Elements *)
 * @param contact (b2Contact *)
 * @note This function is called just before a collision
 */

void	Object::BeginContact(Elements *elem, b2Contact *contact) {
	contact->SetEnabled(false);
	contact->enableContact = false;
}

//! Destructor
/**
 * Basic Destructor
 */
Object::~Object(void) {
	return;
}
