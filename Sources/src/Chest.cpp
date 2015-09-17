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

Chest::Chest(void) : isSpawn(0) {
	//	this->_contains = nullptr;
	this->SetLayer(10);
	this->SetDensity(0);
	this->SetIsSensor(1);
	this->addAttribute("class", "Chest");
	this->addAttribute("type", "Chest");
	this->addAttribute("chest", "1");
	this->addAttribute("physic", "1");
	this->addAttribute("sprite", "");
	this->addAttribute("spritesFrame", "Resources/Images/Chest/chest_000.png");
	this->_target = nullptr;
	this->_chestItems[0] = this->_chestItems[1] = this->_chestItems[2] = "";
	this->_isUsed = this->_gold = 0;
	return;
}

Chest::~Chest(void) {}

void	Chest::spawn(void) {
	this->setXStart(Game::spawnChest.X);
	this->setYStart(Game::spawnChest.Y);
	this->display();
	this->isSpawn = 1;
	if (this->_isUsed == 0) {
		if (this->_gold)
			this->PlaySpriteAnimation(0.4f, SAT_OneShot, 0, 2);
		else
			this->PlaySpriteAnimation(0.4f, SAT_OneShot, 3, 4);
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
				height - ((height / 3) * 2), Vector2(width - (width / 4), height / 5), 101));
	this->displayChestContent();
}

void	Chest::displayChestContent(void) {
	int		height = theCamera.GetWindowHeight(), width = theCamera.GetWindowWidth();
	HUDWindow		*w = Game::getHUD();
	HUDActor		*tmp;
	int		x = width / 2 - width / 4, y = height - ((height / 3) * 2), i;

	for (i = 0; i < 3; i++, x += width / 6) {
		tmp = w->addImage("Resources/Images/bag_slot.png", x, y, 80, 102);
		this->_interfaceElem.push_back(tmp);
		this->_choices.push_back(tmp);
		if (i == 0)
			this->_choicePointer = tmp;
	}
	tmp = w->addImage("Resources/Images/gold.png", x, y, 80, 150);
	w->setText(std::to_string(this->_gold), x - (std::to_string(this->_gold).length() / 2) * 10, y + 50, Vector3(1, 1, 0), 1);
	this->_choices.push_back(tmp);
	this->_interfaceElem.push_back(tmp);
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
	theCamera.MoveTo(Vector3(Game::currentGame->getCurrentMap().getXMid(),
				Game::currentGame->getCurrentMap().getYMid() + 1.8, 9.001), true);
	Game::toggleMenu = true;
	Game::currentGame->getHero()->subscribeToAll();
	if (this->_isUsed == 1)
		this->PlaySpriteAnimation(0.4f, SAT_OneShot, 2, 0);
	Game::getHUD()->removeText(std::to_string(this->_gold));
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
		if (i < 3) {
			if (this->_chestItems[i] == "" && Game::currentGame->getHero()->getInventory()->getCurrentFocus() == "")
				return ;
			if (this->_chestItems[i] == "" && Game::currentGame->getHero()->getInventory()->getCurrentFocus() != "") {
				this->_chestItems[i] = Game::currentGame->getHero()->getInventory()->getCurrentFocus();
				Game::currentGame->getHero()->getInventory()->dropSelectedItem();
				this->_isUsed = 1;
			} else if (this->_chestItems[i] != ""
					&& Game::currentGame->getHero()->getInventory()->getCurrentFocus() != "") {
				std::string tmp =  Game::currentGame->getHero()->getInventory()->getCurrentFocus();
				std::string tmp2 = this->_chestItems[i];
				int			focus = Game::currentGame->getHero()->getInventory()->getNumFocus();

				theWorld.Remove(this->_img[i]);
				Game::currentGame->getHero()->getInventory()->dropSelectedItem();
				Game::currentGame->getHero()->getInventory()->addItemToInventory(tmp2);
				this->_chestItems[i] = tmp;
				this->_isUsed = 1;
			} else {
				Game::currentGame->getHero()->getInventory()->addItemToInventory(this->_chestItems[i]);
				this->_chestItems[i] = "";
				theWorld.Remove(this->_img[i]);
				this->_isUsed = 1;
			}
			this->updateItems();
		} else {
			if (Game::currentGame->getHero()->getGold()) {
				j = this->_gold;
				std::cout << j << std::endl;
				this->_gold += Game::currentGame->getHero()->getGold();
				Game::getHUD()->updateText(std::to_string(j), std::to_string(this->_gold));
				Game::currentGame->getHero()->setGold(0);
				Game::getHUD()->updateGold(0);
				this->_isUsed = 1;
			}
		}
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
	int		x = width / 2 - width / 4, y = height - ((height / 3) * 2);
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
			std::cout << path << ", " << it->second << std::endl;
			tmp = w->addImage(path, x + (width / 6 * it->first), y, 80, 104);
			this->_interfaceElem.push_back(tmp);
			this->_img[it->first] = tmp;
		}
	}
}

void	Chest::_makeItUsed(void) {
	this->_isUsed = 1;
	this->removeInterface();
	this->PlaySpriteAnimation(0.4f, SAT_OneShot, 2, 0);
}

void		Chest::applySave(std::map<std::string, Json::Value> save) {
	Json::ValueIterator		it;

	for (it = save["chest"].begin(); it != save["chest"].end(); it++) {
		if (it.key().asString() != "gold") {
			this->_chestItems[atoi(it.key().asString().c_str())] = it->asString();
		} else {
			this->_gold = it->asInt();
		}
	}
}

int		Chest::isUsed(void) { return this->_isUsed; };
void	Chest::reset(void) { this->_isUsed = 0; };
int		Chest::getGold(void) { return this->_gold; };
std::map<int, std::string>	Chest::getItems(void) { return this->_chestItems; };
