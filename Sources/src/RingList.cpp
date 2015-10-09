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
 * File: RingList.cpp
 * Creation: 2015-05-07 17:47
 * Manon Budin <mbudin@student.42.fr>
 */

# include "RingList.hpp"

//!  Constructor called at the beginning of the game to load every Ring available
/**
 * Called once and stocked in a static for the whole game
 */
RingList::RingList(void) {
	DIR				*dir;
	struct dirent	*dirEntry;
	std::istringstream	iss;
	std::string		res;

	Log::info("Reading Ring list...");
	dir = opendir("./Resources/Elements/Rings/");
	while (dirEntry = readdir(dir)) {
		if (dirEntry && strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0) {
			iss.str(dirEntry->d_name);
			std::getline(iss, res, '.');
			this->_allRings.push_back(new Ring(res));
		}
	}
}

//! Destructor
/**
 * Basic destructor
 */
RingList::~RingList(void) {
	return;
}

int			RingList::checkExists(std::string name) {
	std::list<Ring*>::iterator it;

	for (it = this->_allRings.begin(); it != this->_allRings.end(); it++) {
		if (name == (*it)->getName()) {
			return 1;
		}
	}
	return 0;
}

//! Returns a Ring in order to use it afterwards
/**
 * Get a Ring obj by name
 * @param name (std::string)
 * @return *it
 */
Ring		*RingList::getRing(std::string name) {
	std::list<Ring*>::iterator it;

	for (it = this->_allRings.begin(); it != this->_allRings.end(); it++) {
		if (name == (*it)->getName()) {
			return ((*it));
		}
	}
}


//! Returns one of the existing Rings
/**
 * Returns a Ring, no matter its level
 */
Ring		*RingList::getRingRandom(void) {
	std::list<Ring*>::iterator it;
	int	i = 0;
	int value = (rand() % this->_allRings.size());

	for (it = this->_allRings.begin(); it != this->_allRings.end(); it++) {
		if (i == value) {
			return ((*it));
		}
		i++;
	}
	return (*this->_allRings.begin());
}

//! Returns one of the existing Rings of the required level
/**
 * Returns a random Ring, with the corresponding item level
 */
Ring		*RingList::getRingRandom(int level) {
	std::list<Ring*>::iterator it;
	std::list<Ring*> Rings;

	for (it = this->_allRings.begin(); it != this->_allRings.end(); it++) {
		if ((*it)->getLootLevel() == level) {
			Rings.push_back(*it);
		}
	}
	int	i = 0;
	int value = (rand() % Rings.size());
	for (it = Rings.begin(); it != Rings.end(); it++) {
		if (i == value) {
			return ((*it));
		}
		i++;
	}
	return (*Rings.begin());

}

std::list<Ring *>		RingList::get3Starters(int level, std::string hero) {
	std::list<Ring *>		list;
	Ring					*tmp;
	std::list<Ring*>::iterator it;

	for (it = this->_allRings.begin(); it != this->_allRings.end(); it++) {
		if ((*it)->getStarterLevel() == level) {
			list.push_back((*it));
			if (list.size() > 3)
				break ;
		}
	}
	return list;
}
