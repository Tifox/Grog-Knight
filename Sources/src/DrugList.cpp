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
 * File: DrugList.cpp
 * Creation: 2015-08-11 17:43
 * Manon Budin <mbudin@student.42.fr>
 */

# include "DrugList.hpp"

//!  Constructor called at the beginning of the game to load every Drug available
/**
 * Called once and stocked in a static for the whole game
 */
DrugList::DrugList(void) {
	DIR				*dir;
	struct dirent	*dirEntry;
	std::istringstream	iss;
	std::string		res;

	Log::info("Reading Drug list...");
	dir = opendir("./Resources/Elements/Drugs/");
	while (dirEntry = readdir(dir)) {
		if (dirEntry && strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0) {
			iss.str(dirEntry->d_name);
			std::getline(iss, res, '.');
			this->_allDrugs.push_back(new Drug(res));
		}
	}
}

//! Destructor
/**
 * Basic destructor
 */
DrugList::~DrugList(void) {
	return;
}

int			DrugList::checkExists(std::string name) {
	std::list<Drug*>::iterator it;

	for (it = this->_allDrugs.begin(); it != this->_allDrugs.end(); it++) {
		if (name == (*it)->getName()) {
			return 1;
		}
	}
	return 0;
}

//! Returns a Drug in order to use it afterwards
/**
 * Get a Drug obj by name
 * @param name (std::string)
 * @return *it
 */
Drug		*DrugList::getDrug(std::string name) {
	std::list<Drug*>::iterator it;

	for (it = this->_allDrugs.begin(); it != this->_allDrugs.end(); it++) {
		if (name == (*it)->getName()) {
			return ((*it));
		}
	}
}


//! Returns one of the existing Drugs
/**
 * Returns a Drug, no matter its level
 */
Drug		*DrugList::getDrugRandom(void) {
	std::list<Drug*>::iterator it;
	int	i = 0;
	int value = (rand() % this->_allDrugs.size());

	for (it = this->_allDrugs.begin(); it != this->_allDrugs.end(); it++) {
		if (i == value) {
			Game::getHUD()->addImage("Resources/Images/HUD/exta.png", theCamera.GetWindowWidth() / 20 * 1.8, theCamera.GetWindowHeight() / 20 * 0.8, theCamera.GetWindowWidth() / 32, 5);
			return ((*it));
		}
		i++;
	}
	Game::getHUD()->addImage("Resources/Images/HUD/exta.png", theCamera.GetWindowWidth() / 20 * 1.8, theCamera.GetWindowHeight() / 20 * 0.8, theCamera.GetWindowWidth() / 32, 5);
	return (*this->_allDrugs.begin());
}
