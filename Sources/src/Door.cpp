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
 * File: Door.cpp
 * Creation: 2015-08-27 04:44
 * Vincent Rey <vrey@student.42.fr>
 */

#include "Door.hpp"

Door::Door(std::string type) {
	this->SetLayer(10);
	this->SetDensity(0);
	this->SetIsSensor(1);
	this->addAttribute("type", "Door");
	if (type == "SecretReturn") {
		this->addAttribute("type2", "Secret");
		this->addAttribute("speType", "return");
	}
	else
		this->addAttribute("type2", type);
	this->addAttribute("chest", "1");
	this->addAttribute("physic", "1");
	this->SetSize(Vector2(3,3));
	if (type == "Secret") {
		this->addAttribute("sprite", "Resources/Images/Door/SecretDoor.png");
		this->setXStart(Game::spawnSecretDoor.X);
		this->setYStart(Game::spawnSecretDoor.Y);
	} else if (type == "SecretReturn") {
		this->addAttribute("sprite", "Resources/Images/Door/SecretDoor.png");
		this->setXStart(Game::spawnSecretReturnDoor.X);
		this->setYStart(Game::spawnSecretReturnDoor.Y);
	} else if (type == "Boss") {
		this->addAttribute("sprite", "Resources/Images/Door/BossDoor.png");
		this->setXStart(Game::spawnBossDoor.X);
		this->setYStart(Game::spawnBossDoor.Y);
	}
	this->display();
	return;
}

Door::~Door(void) {}

