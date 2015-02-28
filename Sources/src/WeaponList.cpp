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


/*
** Default constructor, using the element that called the attack
** @param: Elements *
*/
WeaponList::WeaponList(void) {
	DIR *Dir;
	struct dirent *DirEntry;
	Dir = opendir("./Resources/Elements/Weapons/");
	while(DirEntry=readdir(Dir))
	{
		if( strcmp(DirEntry->d_name, ".") != 0 && strcmp(DirEntry->d_name, "..") != 0 )
			this->_allWeapons.push_back(new Weapon(std::string("./Resources/Elements/Weapons/") + std::string(DirEntry->d_name), 1));
	}
}
WeaponList::~WeaponList(void) {
	return;
}

void		WeaponList::statWeapon(std::string name) {
	std::list<Weapon*>::iterator it;

	for (it = this->_allWeapons.begin(); it != this->_allWeapons.end(); it++) {
		if (name == (*it)->getName()) {
			std::cout << (*it)->getName() << std::endl;
			std::cout << (*it)->getFlavor() << std::endl;
		}
	}
}

Weapon		*WeaponList::equipWeapon(std::string name) {
	std::list<Weapon*>::iterator it;

	for (it = this->_allWeapons.begin(); it != this->_allWeapons.end(); it++) {
		if (name == (*it)->getName()) {
			return ((*it));
		}
	}
}
