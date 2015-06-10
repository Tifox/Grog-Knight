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
 * File: WeaponList.cpp
 * Creation: 2015-02-18 14:00
 * Vincent Rey <vrey@student.42.fr>
 */

# include "WeaponList.hpp"

//!  Constructor called at the beginning of the game to load every weapon available
/**
 * Called once and stocked in a static for the whole game
 */
WeaponList::WeaponList(void) {
	DIR				*dir;
	struct dirent	*dirEntry;
	std::istringstream	iss;
	std::string		res;

	Log::info("Reading weapon list...");
	dir = opendir("./Resources/Elements/Weapons/");
	while (dirEntry = readdir(dir)) {
		if (dirEntry && strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0) {
			iss.str(dirEntry->d_name);
			std::getline(iss, res, '.');
			this->_allWeapons.push_back(new Weapon(res));
		}
	}
}

//! Destructor
/**
 * Basic destructor
 */
WeaponList::~WeaponList(void) {
	return;
}

int			WeaponList::checkExists(std::string name) {
	std::list<Weapon*>::iterator it;

	for (it = this->_allWeapons.begin(); it != this->_allWeapons.end(); it++) {
		if (name == (*it)->getName()) {
			return 1;
		}
	}
	return 0;
}

//! Returns a weapon in order to use it afterwards
/**
 * Get a weapon obj by name
 * @param name (std::string)
 * @return *it
 */
Weapon		*WeaponList::getWeapon(std::string name) {
	std::list<Weapon*>::iterator it;

	for (it = this->_allWeapons.begin(); it != this->_allWeapons.end(); it++) {
		if (name == (*it)->getName()) {
			return ((*it));
		}
	}
}


//! Returns one of the existing weapons
/**
 * Returns a weapon, no matter its level
 */
Weapon		*WeaponList::getWeaponRandom(void) {
	std::list<Weapon*>::iterator it;
	int	i = 0;
	int value = (rand() % this->_allWeapons.size());

	for (it = this->_allWeapons.begin(); it != this->_allWeapons.end(); it++) {
		if (i == value) {
			return ((*it));
		}
		i++;
	}
	return (*this->_allWeapons.begin());

}

//! Returns one of the existing weapons of the required level
/**
 * Returns a random weapon, with the corresponding item level
 */
Weapon		*WeaponList::getWeaponRandom(int level) {
	std::list<Weapon*>::iterator it;
	std::list<Weapon*> weapons;

	for (it = this->_allWeapons.begin(); it != this->_allWeapons.end(); it++) {
		if ((*it)->getLootLevel() == level) {
			weapons.push_back(*it);
		}
	}
	int	i = 0;
	int value = (rand() % weapons.size());
	for (it = weapons.begin(); it != weapons.end(); it++) {
		if (i == value) {
			return ((*it));
		}
		i++;
	}
	return (*weapons.begin());

}
