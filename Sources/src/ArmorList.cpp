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
 * File: ArmorList.cpp
 * Creation: 2015-05-07 17:33
 * Manon Budin <mbudin@student.42.fr>
 */

# include "ArmorList.hpp"

//!  Constructor called at the beginning of the game to load every Armor available
/**
 * Called once and stocked in a static for the whole game
 */
ArmorList::ArmorList(void) {
	DIR				*dir;
	struct dirent	*dirEntry;
	std::istringstream	iss;
	std::string		res;

	Log::info("Reading Armor list...");
	dir = opendir("./Resources/Elements/Armors/");
	while (dirEntry = readdir(dir)) {
		if (dirEntry && strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0) {
			iss.str(dirEntry->d_name);
			std::getline(iss, res, '.');
			this->_allArmors.push_back(new Armor(res));
		}
	}
}

//! Destructor
/**
 * Basic destructor
 */
ArmorList::~ArmorList(void) {
	return;
}

int			ArmorList::checkExists(std::string name) {
	std::list<Armor*>::iterator it;

	for (it = this->_allArmors.begin(); it != this->_allArmors.end(); it++) {
		if (name == (*it)->getName()) {
			return 1;
		}
	}
	return 0;
}

//! Returns a Armor in order to use it afterwards
/**
 * Get a Armor obj by name
 * @param name (std::string)
 * @return *it
 */
Armor		*ArmorList::getArmor(std::string name) {
	std::list<Armor*>::iterator it;

	for (it = this->_allArmors.begin(); it != this->_allArmors.end(); it++) {
		if (name == (*it)->getName()) {
			return ((*it));
		}
	}
}


//! Returns one of the existing Armors
/**
 * Returns a Armor, no matter its level
 */
Armor		*ArmorList::getArmorRandom(void) {
	std::list<Armor*>::iterator it;
	int	i = 0;
	int value = (rand() % this->_allArmors.size());

	for (it = this->_allArmors.begin(); it != this->_allArmors.end(); it++) {
		if (i == value) {
			return ((*it));
		}
		i++;
	}
	return (*this->_allArmors.begin());

}

//! Returns one of the existing Armors of the required level
/**
 * Returns a random Armor, with the corresponding item level
 */
Armor		*ArmorList::getArmorRandom(int level) {
	std::list<Armor*>::iterator it;
	std::list<Armor*> armors;

	for (it = this->_allArmors.begin(); it != this->_allArmors.end(); it++) {
		if ((*it)->getLootLevel() == level) {
			armors.push_back(*it);
		}
	}
	int	i = 0;
	if (armors.size() > 0) {
		int value = (rand() % armors.size());
		for (it = armors.begin(); it != armors.end(); it++) {
			if (i == value) {
				return ((*it));
			}
			i++;
		}
	}
	Log::info("No armor for requested level");
	return (*this->_allArmors.begin());

}

std::list<Armor *>		ArmorList::get3Starters(int level) {
	std::list<Armor *>		list;
	Armor					*tmp;

	while (list.size() < 3) {
		tmp = this->getArmorRandom(level);
		if (std::find(list.begin(), list.end(), tmp) == list.end())
			list.push_back(tmp);
	}
	return list;
}
