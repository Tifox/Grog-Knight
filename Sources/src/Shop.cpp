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
	int rant;
	int rant2;
	this->_items = std::vector<std::string>(nb);
	for (i = 0; i < nb; i++) {
		std::string item;
		theSwitchboard.SubscribeTo(this, "deleteShopItem" + std::to_string(i));
		rant = rand() % 3;
		if (rant == 0) {
			rant2 = rand() % 3;
			if (rant2 == 1)
				lvl--;
			else if (rant2 == 3)
				lvl++;
			if (lvl < 1)
				lvl = 1;
			item = Game::wList->getWeaponRandom(lvl)->getName();
		}
		if (rant == 1) {
			rant2 = rand() % 3;
			if (rant2 == 1)
				lvl--;
			else if (rant2 == 3)
				lvl++;
			if (lvl < 1)
				lvl = 1;
			item = Game::aList->getArmorRandom(lvl)->getName();
		}
		if (rant == 2) {
			rant2 = rand() % 3;
			if (rant2 == 1)
				lvl--;
			else if (rant2 == 3)
				lvl++;
			if (lvl < 1)
				lvl = 1;
			item = Game::rList->getRingRandom(lvl)->getName();
		}
		if (i != 0) {
			if (std::find(this->_items.begin(), this->_items.end(), item) == this->_items.end())
				this->_items[i] = item;
			else
				i--;
		}
		else {
			this->_items[i] = item;
		}
	}
}

//! Deletes the merchant when changing floor
Shop::~Shop(void) {

}

//! Reveal the merchant when you enter the correct room
void	Shop::revealShop(int x, int y) {
	this->_shopItems = std::vector<ShopItem*>(this->_items.size());
  int i = 0;
  for (i = 0, x -= 2; i < this->_items.size(); i++, x += 3) {
	if (this->_items[i] != "bought") {
		this->_shopItems[i] = new ShopItem(this->_items[i], x, y - 7, i);
	}
	else
		this->_shopItems[i] = nullptr;
  }
}

Shop::ShopItem::ShopItem(std::string name, int x, int y, int num): Elements() {
	this->SetPosition(x, y);
	this->SetLayer(99);
	this->addAttribute("type", "shopItem");
	this->addAttribute("price", "10");
	this->addAttribute("name", name);
	this->addAttribute("number", std::to_string(num));
	this->addAttribute("shopPosition", std::to_string(num));
	if (Game::wList->checkExists(name) == 1) {
		this->SetSprite(Game::wList->getWeapon(name)->getSprite());
		this->addAttribute("flavor", Game::wList->getWeapon(name)->getFlavor());
		this->addAttribute("type3", "Weapon");
		this->addAttribute("displayName",Game::wList->getWeapon(name)->getDisplayName());
		if (Game::wList->getWeapon(name)->getAttribute("hpBuff") != "")
			this->addAttribute("hpBuff", Game::wList->getWeapon(name)->getAttribute("hpBuff"));
	}
	else if (Game::aList->checkExists(name) == 1) {
		this->SetSprite(Game::aList->getArmor(name)->getSprite());
		this->addAttribute("type3", "Armor");
		this->addAttribute("flavor", Game::aList->getArmor(name)->getFlavor());
		this->addAttribute("displayName",Game::aList->getArmor(name)->getDisplayName());
		if (Game::aList->getArmor(name)->getAttribute("hpBuff") != "")
			this->addAttribute("hpBuff", Game::aList->getArmor(name)->getAttribute("hpBuff"));
  }
	else if (Game::rList->checkExists(name) == 1) {
		this->addAttribute("flavor", Game::rList->getRing(name)->getFlavor());
		this->addAttribute("type3", "Ring");
		this->SetSprite(Game::rList->getRing(name)->getSprite());
		if (Game::rList->getRing(name)->getAttribute("hpBuff") != "")
			this->addAttribute("hpBuff", Game::rList->getRing(name)->getAttribute("hpBuff"));
	}
	this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	this->SetIsSensor(true);
	this->SetDensity(0);
	Game::bodiesToCreate.push_back(this);
}

//! Hide the merchant when you leave the roomz`
/**
 * Not currently used
 */
void	Shop::hideShop(void) {
	int i;
	for (i = 0; i < this->_shopItems.size(); i++) {
		if (this->_shopItems[i] != nullptr)
			this->_shopItems[i]->GetBody()->SetActive(false);
			theWorld.Remove(this->_shopItems[i]);
			this->_shopItems[i] = nullptr;
	}
}

void	Shop::ReceiveMessage(Message *m) {
	int i;
	for (i = 0; i < this->_items.size(); i++) {
		if (m->GetMessageName() == "deleteShopItem" + std::to_string(i)) {
			this->_items[i] = "bought";
			this->_shopItems[i]->GetBody()->SetActive(false);
			this->_shopItems[i]->ChangeColorTo(Color(0, 0, 0, 0), 0);
			theWorld.Remove(this->_shopItems[i]);
			this->_shopItems[i] = nullptr;
			Game::currentGame->getShopkeeper()->displayText("A wise choice.");
		}
	}
}
