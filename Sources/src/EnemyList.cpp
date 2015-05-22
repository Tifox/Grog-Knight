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
			this->_allEnemies.push_back(new EnemyData(res));
		}
	}
}


//! Constructor for subclass EnemyData
/**
 * This class contains the infos on an enemy, without creating the object
 * @param res the name of the enemy
 */
EnemyList::EnemyData::EnemyData(std::string res) {
	this->_name = "Enemies/" + res;
	this->_readFile(this->_name);
}

std::string		EnemyList::EnemyData::getName() { return this->_name; }
int				EnemyList::EnemyData::getLevel() { return this->_level; }
bool	 		EnemyList::EnemyData::isFlying() { return this->_flying; }

//! Json parser to get the infos on the enemy
/**
 * @sa Characters::_readFile
 */
void	EnemyList::EnemyData::_readFile(std::string name) {
	std::string			file;
	std::stringstream 	buffer;
	std::ifstream		fd;

	file = "./Resources/Elements/" + name + ".json";

	fd.open(file.c_str());
	if (!fd.is_open())
		Log::error("Can't open the file for the " +
		name + " class. (Supposed path is Resources/Elements/" + name +".json)");
	buffer << fd.rdbuf();
	this->_parseJson(buffer.str());
}

//! Follow-up for the json parser
/**
 * @sa Characters::_parseJson
 */
void	EnemyList::EnemyData::_parseJson(std::string file) {
	Json::Reader	read;
	Json::Value		json;
	Json::ValueIterator i, v;
	std::map<std::string, Json::Value>	tmp;

	if (!read.parse(file, json))
		Log::error("Error in json syntax :\n" + read.getFormattedErrorMessages());
	if (json["infos"].get("name", "").asString() != this->_name)
		Log::warning("The class name is different with the name in the config file: " + this->_name + "/" + json["infos"].get("name", "").asString());
	this->_level = json["infos"].get("level", "").asInt();
	this->_flying = json["infos"].get("flying", "").asBool();
}

//! Destructor
/**
 * Basic destructor
 */
EnemyList::~EnemyList(void) {
	return;
}

//! Returns one of the existing enemies
/**
 * Returns a enemy, no matter its level - will check whether the enemy is flying
 * @param flying
 */
std::string		EnemyList::getEnemyRandom(bool flying) {
	std::list<EnemyData*>::iterator it;
	std::list<EnemyData*> enemies;

	for (it = this->_allEnemies.begin(); it != this->_allEnemies.end(); it++) {
		if ((*it)->isFlying() == flying) {
			enemies.push_back(*it);
		}
	}

	int	i = 0;
	int value = (rand() % enemies.size());

	for (it = this->_allEnemies.begin(); it != this->_allEnemies.end(); it++) {
		if (i == value) {
			return ((*it)->getName());
		}
		i++;
	}
	return ((*this->_allEnemies.begin())->getName());

}

//! Returns one of the existing enemies of the required level
/**
 * Returns a random enemy, with the corresponding level
 * @param level the enemy level queried
 * @param flying a boolean that asks whether the enemy should be flying or not
 */
std::string		EnemyList::getEnemyRandom(int level, bool flying) {
	std::list<EnemyData*>::iterator it;
	std::list<EnemyData*> enemies;

	for (it = this->_allEnemies.begin(); it != this->_allEnemies.end(); it++) {
		if ((*it)->getLevel() == level && (*it)->isFlying() == flying) {
			enemies.push_back(*it);
		}
	}
	int	i = 0;
	int value = (rand() % enemies.size());
	for (it = enemies.begin(); it != enemies.end(); it++) {
		if (i == value) {
			return ((*it)->getName());
		}
		i++;
	}
	return ((*this->_allEnemies.begin())->getName());

}
