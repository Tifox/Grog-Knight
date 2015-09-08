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
	this->_target = nullptr;
	this->display();
	this->_chestItems[0] = this->_chestItems[1] = this->_chestItems[2] = "";
	return;
}

Chest::~Chest(void) {}

void	Chest::spawn(void) {
	if (this->isUsed == 0) {
		this->PlaySpriteAnimation(0.4f, SAT_OneShot, 0, 4);
	}
}

void	Chest::displayInterface(void) {
	HUDWindow		*w = Game::getHUD();
	int				height = theCamera.GetWindowHeight(), width = theCamera.GetWindowWidth();

	// Subscribe
	theSwitchboard.SubscribeTo(this, "upPressed");
	theSwitchboard.SubscribeTo(this, "downPressed");
	theSwitchboard.SubscribeTo(this, "forwardPressed");
	theSwitchboard.SubscribeTo(this, "backwardPressed");
	theSwitchboard.SubscribeTo(this, "enterPressed");

	// Display Interface
	this->_interfaceElem.push_back(w->addImage("Resources/Images/bag.png", width / 2, 
		height - ((height / 3) * 2),Vector2(width - (width / 2), height / 6), 101));
	this->displayChestContent();
}

void	Chest::displayChestContent(void) {
	int		height = theCamera.GetWindowHeight(), width = theCamera.GetWindowWidth();
	HUDWindow		*w = Game::getHUD();
	HUDActor		*tmp;
	int		x = width / 2 - width / 6, y = height - ((height / 3) * 2), i;

	for (i = 0; i < 3; i++, x += width / 6) {
		tmp = w->addImage("Resources/Images/bag_slot.png", x, y, 80, 102);
		this->_interfaceElem.push_back(tmp);
		this->_choices.push_back(tmp);
		if (i == 0)
			this->_choicePointer = tmp;
	}
	this->makeChoices();
	this->updateItems();
}

void	Chest::removeInterface(void) {
	std::list<HUDActor *>::iterator		it;

	// Unsubscribe
	theSwitchboard.UnsubscribeFrom(this, "upPressed");
	theSwitchboard.UnsubscribeFrom(this, "downPressed");
	theSwitchboard.UnsubscribeFrom(this, "forwardPressed");
	theSwitchboard.UnsubscribeFrom(this, "backwardPressed");
	theSwitchboard.UnsubscribeFrom(this, "enterPressed");

	for (it = this->_interfaceElem.begin(); it != this->_interfaceElem.end(); it++)
		theWorld.Remove(*it);
	theWorld.Remove(this->_target);
}

void	Chest::ReceiveMessage(Message *m) {
	std::list<HUDActor *>::iterator		it = std::find(this->_choices.begin(), this->_choices.end(), this->_choicePointer);

	if (m->GetMessageName() == "forwardPressed") {
		if (++it != this->_choices.end())
			this->_choicePointer = *it;
	} else if (m->GetMessageName() == "backwardPressed") {
		if (it != this->_choices.begin()) {
			this->_choicePointer = *(--it);
		}
	}
	this->makeChoices();
	if (m->GetMessageName() == "enterPressed") {
		int		i, j;

		for (i = 0, it = this->_choices.begin(); this->_choicePointer != *it; i++, it++);
		std::cout << "CI: " << this->_chestItems[i] << ", Inv: " << Game::currentGame->getHero()->getInventory()->getCurrentFocus();
		if (this->_chestItems[i] == "") {
			this->_chestItems[i] = Game::currentGame->getHero()->getInventory()->getCurrentFocus();
			Game::currentGame->getHero()->getInventory()->dropSelectedItem();
		} else if (this->_chestItems[i] != "" 
						&& Game::currentGame->getHero()->getInventory()->getCurrentFocus() != "") {
			std::string tmp =  Game::currentGame->getHero()->getInventory()->getCurrentFocus();
			std::string tmp2 = this->_chestItems[i];
			int			focus = Game::currentGame->getHero()->getInventory()->getNumFocus();

			theWorld.Remove(this->_img[i]);
			Game::currentGame->getHero()->getInventory()->dropSelectedItem();
			Game::currentGame->getHero()->getInventory()->addItemToInventory(tmp2);
			this->_chestItems[i] = tmp;
		} else {
			Game::currentGame->getHero()->getInventory()->addItemToInventory(this->_chestItems[i]);
			this->_chestItems[i] = "";
			theWorld.Remove(this->_img[i]);
		}
		this->updateItems();
	}
}

void	Chest::makeChoices(void) {
	if (this->_target != nullptr)
		theWorld.Remove(this->_target);
	this->_target = Game::getHUD()->addImage("Resources/Images/HUD/weapon_background.png", this->_choicePointer->GetPosition().X, this->_choicePointer->GetPosition().Y, 81, 103);
}

void	Chest::updateItems(void) {
	std::map<int, std::string>::iterator	it;
	int		height = theCamera.GetWindowHeight(), width = theCamera.GetWindowWidth();
	HUDWindow		*w = Game::getHUD();
	int		x = width / 2 - width / 6, y = height - ((height / 3) * 2);
	std::string		path;
	HUDActor		*tmp;

	for (it = this->_chestItems.begin(); it != this->_chestItems.end(); it++) {
		if (it->second != "") {
			if (Game::wList->checkExists(it->second)) {
				path = Game::wList->getWeapon(it->second)->getSprite();
			} else if (Game::aList->checkExists(it->second)) {
				path = Game::aList->getArmor(it->second)->getSprite();
			} else if (Game::rList->checkExists(it->second)) {
				path = Game::rList->getRing(it->second)->getSprite();
			}
			tmp = w->addImage(path, x + (width / 6 * it->first), y, 80, 104);
			this->_interfaceElem.push_back(tmp);
			this->_img[it->first] = tmp;
		}
	}
	// Swap, take items, gold
}
