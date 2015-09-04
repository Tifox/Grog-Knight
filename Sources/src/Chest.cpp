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
 * File: Chest.cpp
 * Creation: 2015-08-27 04:44
 * Vincent Rey <vrey@student.42.fr>
 */

#include "Chest.hpp"

Chest::Chest(void): isUsed(0) {
//	this->_contains = nullptr;
	this->SetLayer(10);
	this->SetDensity(0);
	this->SetIsSensor(1);
	this->addAttribute("class", "Chest");
	this->addAttribute("type", "Chest");
	this->setXStart(Game::spawnChest.X);
	this->setYStart(Game::spawnChest.Y);
	this->addAttribute("chest", "1");
	this->addAttribute("physic", "1");
	this->addAttribute("sprite", "");
	this->addAttribute("spritesFrame", "Resources/Images/Hero/hero_000.png");
	this->display();
	return;
}

Chest::~Chest(void) {}

void	Chest::spawn(void) {
	if (this->isUsed == 0) {
		this->PlaySpriteAnimation(0.4f, SAT_OneShot, 0, 4);
	}
}

void	Chest::displayInterface(void) {
	theSwitchboard.SubscribeTo(this, "upPressed");
	theSwitchboard.SubscribeTo(this, "downPressed");
	theSwitchboard.SubscribeTo(this, "forwardPressed");
	theSwitchboard.SubscribeTo(this, "backwardPressed");
	theSwitchboard.SubscribeTo(this, "enterPressed");
}

void	Chest::removeInterface(void) {
	theSwitchboard.UnsubscribeFrom(this, "upPressed");
	theSwitchboard.UnsubscribeFrom(this, "downPressed");
	theSwitchboard.UnsubscribeFrom(this, "forwardPressed");
	theSwitchboard.UnsubscribeFrom(this, "backwardPressed");
	theSwitchboard.UnsubscribeFrom(this, "enterPressed");
}

void	Chest::ReceiveMessage(Message *m) {

}
