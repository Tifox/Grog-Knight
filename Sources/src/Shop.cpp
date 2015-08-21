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
  for (i = 0; i < nb; i++) {
	int rant = rand() % 3;
	if (rant == 0)
	  this->_items[i] = Game::wList->getWeaponRandom(lvl)->getName(); 
	if (rant == 1)
	  this->_items[i] = Game::aList->getArmorRandom(lvl)->getName(); 
	if (rant == 2)
	  this->_items[i] = Game::rList->getRingRandom(lvl)->getName(); 
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
	  new ShopItem(this->_items[i], x + i - 1, y, i);
	}
  }
  
}

Shop::ShopItem::ShopItem(std::string name, int x, int y, int num): Elements() {
  this->SetPosition(x, y);
  this->addAttribute("type", "shopItem");
  this->addAttribute("price", "10");
  this->addAttribute("name", name);
  this->addAttribute("shopPosition", std::to_string(num));
  if (Game::wList->checkExists(name) == 1) {
	this->SetSprite(Game::wList->getWeapon(name)->getSprite());
  }
  else if (Game::aList->checkExists(name) == 1) {
	this->SetSprite(Game::aList->getArmor(name)->getSprite());
  }
  else if (Game::rList->checkExists(name) == 1) {
	this->SetSprite(Game::rList->getRing(name)->getSprite());
  }
  this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
  this->SetIsSensor(true);
  this->SetDensity(0);
  Game::bodiesToCreate.push_back(this);
}

//! Hide the merchant when you leave the roomz`
void	Shop::hideShop(void) {
}
