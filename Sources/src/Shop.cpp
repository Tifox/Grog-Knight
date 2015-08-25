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
 * File: Shop.cpp
 * Creation: 2015-08-02 14:44
 * Vincent Rey <vrey@student.42.fr>
 */

# include "Shop.hpp"

//! Base constructor
Shop::Shop(void) {
}

//! Main constructor
/**
 * Takes a position, level of items spawned, number of items spawned
 * and get it ready to appear when necessary
 */
Shop::Shop(int x, int y, int lvl, int nb) {
	int i;
	int j;
	for (i = 0; i < nb; i++) {
		theSwitchboard.SubscribeTo(this, "deleteShopItem" + i);
		int rant = rand() % 3;
		if (rant == 0)
			this->_items[i] = Game::wList->getWeaponRandom(lvl)->getName();
		if (rant == 1)
			this->_items[i] = Game::aList->getArmorRandom(lvl)->getName();
		if (rant == 2)
			this->_items[i] = Game::rList->getRingRandom(lvl)->getName();
		// if (i != 0) {
		// 	for (j = i; j >= 0; j--) {
		// 		std::cout << j << std::endl;
		// 		if (this->_items[j] == this->_items[i]) {
		// 			i--;
		// 			break;
		// 		}
		// 	}
		// }
	}
	this->_items[i] = "end";
}
//! Deletes the merchant when changing floor
Shop::~Shop(void) {

}

//! Reveal the merchant when you enter the correct room
void	Shop::revealShop(int x, int y) {
  int i = 0;
  for (i = 0; this->_items[i] != "end"; i++) {
	if (this->_items[i] != "bought") {
		this->_shopItems[i] = new ShopItem(this->_items[i], x + i * 2 - 1, y, i);
	}
  }
}

Shop::ShopItem::ShopItem(std::string name, int x, int y, int num): Elements() {
	this->SetPosition(x, y);
	this->addAttribute("type", "shopItem");
	this->addAttribute("price", "10");
	this->addAttribute("name", name);
	this->addAttribute("number", std::to_string(num));
	this->addAttribute("shopPosition", std::to_string(num));
	if (Game::wList->checkExists(name) == 1) {
		this->SetSprite(Game::wList->getWeapon(name)->getSprite());
		this->addAttribute("flavor", Game::wList->getWeapon(name)->getFlavor());
		this->addAttribute("type3", "Weapon");
		if (Game::wList->getWeapon(name)->getAttribute("hpBuff") != "")
			this->addAttribute("hpBuff", Game::wList->getWeapon(name)->getAttribute("hpBuff"));
		if (Game::wList->getWeapon(name)->getAttribute("manaBuff") != "")
			this->addAttribute("manaBuff", Game::wList->getWeapon(name)->getAttribute("manaBuff"));
	}
	else if (Game::aList->checkExists(name) == 1) {
		this->SetSprite(Game::aList->getArmor(name)->getSprite());
		this->addAttribute("type3", "Armor");
		this->addAttribute("flavor", Game::aList->getArmor(name)->getFlavor());
		if (Game::aList->getArmor(name)->getAttribute("hpBuff") != "")
			this->addAttribute("hpBuff", Game::aList->getArmor(name)->getAttribute("hpBuff"));
		if (Game::aList->getArmor(name)->getAttribute("manaBuff") != "")
			this->addAttribute("manaBuff", Game::aList->getArmor(name)->getAttribute("manaBuff"));
  }
	else if (Game::rList->checkExists(name) == 1) {
		this->addAttribute("flavor", Game::rList->getRing(name)->getFlavor());
		this->addAttribute("type3", "Ring");
		this->SetSprite(Game::rList->getRing(name)->getSprite());
		if (Game::rList->getRing(name)->getAttribute("hpBuff") != "")
			this->addAttribute("hpBuff", Game::rList->getRing(name)->getAttribute("hpBuff"));
		if (Game::rList->getRing(name)->getAttribute("manaBuff") != "")
			this->addAttribute("manaBuff", Game::rList->getRing(name)->getAttribute("manaBuff"));
	}
	this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	this->SetIsSensor(true);
	this->SetDensity(0);
	Game::bodiesToCreate.push_back(this);
}

//! Hide the merchant when you leave the roomz`
void	Shop::hideShop(void) {
}

void	Shop::ReceiveMessage(Message *m) {
	int i;
	for (i = 0; this->_items[i] != "end"; i++) {
		if (m->GetMessageName() == "deleteShopItem" + i) {
			this->_items[i] = "bought";
			this->_shopItems[i]->GetBody()->SetActive(false);
			theWorld.Remove(this->_shopItems[i]);
			Game::currentGame->getShopkeeper()->displayText("A wise choice.");
		}
	}
}
