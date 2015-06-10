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
 * File: Inventory.cpp
 * Creation: 2015-05-27 04:44
 * Vincent Rey <vrey@student.42.fr>
 */

# include "Inventory.hpp"

//! Main constructor
/**
 * Constructor that initiates the inventory for a given character
 * @param slots the number of slots you wanna give the inventory
 */

Inventory::Inventory(int slots): _slots(slots), _focused(0), _inInventory(0) {
}

//! change the currently selected item
/**
 * Called by character when pressing the item button,
 * to target another item for dropping/equipping
 */
void			Inventory::changeItemFocus(void) {
	if (this->_slots == 1)
		return;
	else if (this->_focused < this->_slots - 1) {
		this->_focused++;
	} else
		  this->_focused = 0;
	std::cout << this->_focused << ": " << this->_items[this->_focused] << std::endl;


}


//! Called by char when picking up an item
/**
 * When inventory is full, returns 1 and the game automatically drops the selected item through
 * dropSelectedItem
 * @param item the name of the item to add in the inventory
 * @sa Characters::pickupItem
 */

int		Inventory::addItemToInventory(std::string item) {
	if (!this->_items.empty() && this->_inInventory > 0 &&
		this->_items[this->_focused] != "")
		return 1;
	else {
		this->_items[this->_focused] = item;
		this->_inInventory++;
	}
	Game::getHUD()->consumable(this->_items);
}


std::string Inventory::getCurrentFocus(void) {
	return this->_items[this->_focused];
}

void		Inventory::swapEquipmentAndInventory(std::string item) {
	this->_items[this->_focused] = item;
}

std::string		Inventory::dropSelectedItem(void) {

	std::string itemName;

	itemName = this->_items[this->_focused];
	this->_items[this->_focused].clear();
	this->_inInventory--;
	return itemName;
}

std::string 	Inventory::equipSelectedItem(void) {
	if (this->_items[this->_focused].empty()) {
		return "";
	}
	return this->_items[this->_focused];
}

/* GETTERS */
std::map<int, std::string>		Inventory::getItems(void) { return this->_items; };
