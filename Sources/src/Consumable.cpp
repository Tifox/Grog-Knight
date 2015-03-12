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


/**
 * Basic Constructor
 */
Consumable::Consumable(void) {
	this->addAttribute("type2", "Consumable");
	this->SetColor(1,1,1,1);
	this->SetPosition(6, -11);
	this->InitPhysics();
	theWorld.Add(this);
	Log::info("wjorwbgurw");
}

void	Consumable::BeginContact(Elements *elem, b2Contact *contact) {
	if (elem->getAttributes()["type"] == "Hero"){
		Game::bodiesToDestroy.push_back(this);
		contact->SetEnabled(false);
	}
}

void	Consumable::init(void) {
}

/*
 * Basic Destructor
 */
Consumable::~Consumable(void) {
	return;
}
