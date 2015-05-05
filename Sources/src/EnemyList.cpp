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
 * File: EnemyList.cpp
 * Creation: 2015-02-18 14:00
 * Vincent Rey <vrey@student.42.fr>
 */

# include "EnemyList.hpp"

//!  Constructor called at the beginning of the game to load every enemy available
/**
 * Called once and stocked in a static for the whole game
 */
EnemyList::EnemyList(void) {
	DIR				*dir;
	struct dirent	*dirEntry;
	std::istringstream	iss;
	std::string		res;

	dir = opendir("./Resources/Elements/Enemies/");
	while (dirEntry = readdir(dir)) {
		if (dirEntry && strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0) {
			iss.str(dirEntry->d_name);
			std::getline(iss, res, '.');
			this->_allEnemies.push_back(new Enemy("Enemies/" + res));
		}
	}
}

//! Destructor
/**
 * Basic destructor
 */
EnemyList::~EnemyList(void) {
	return;
}

//! Returns a enemy in order to use it afterwards
/**
 * Get an enemy obj by name
 * @param name the name of the enemy
 * @return *it
 */
Enemy		*EnemyList::getEnemy(std::string name) {
	std::list<Enemy*>::iterator it;

	for (it = this->_allEnemies.begin(); it != this->_allEnemies.end(); it++) {
		if (name == (*it)->getAttribute("name")) {
			return ((*it));
		}
	}
}


//! Returns one of the existing enemies
/**
 * Returns a enemy, no matter its level
 */
Enemy		*EnemyList::getEnemyRandom(bool flying) {
	std::list<Enemy*>::iterator it;
	int	i = 0;
	int value = (rand() % this->_allEnemies.size());

	// for (it = this->_allEnemies.begin(); it != this->_allEnemies.end(); it++) {
	// 	if (i == value) {
	// 		return ((*it));
	// 	}
	// 	i++;
	// }
	// return (*this->_allEnemies.begin());

}

//! Returns one of the existing enemies of the required level
/**
 * Returns a random enemy, with the corresponding level
 * @param level the enemy level queried
 * @param flying a boolean that asks whether the enemy should be flying or not
 */
Enemy		*EnemyList::getEnemyRandom(int level, bool flying) {
	std::list<Enemy*>::iterator it;
	std::list<Enemy*> enemies;

	// for (it = this->_allEnemies.begin(); it != this->_allEnemies.end(); it++) {
	// 	if ((*it)->getLootLevel() == level) {
	// 		enemies.push_back(*it);
	// 	}
	// }
	// int	i = 0;
	// int value = (rand() % enemies.size());
	// for (it = enemies.begin(); it != enemies.end(); it++) {
	// 	if (i == value) {
	// 		return ((*it));
	// 	}
	// 	i++;
	// }
	// return (*enemies.begin());

}
