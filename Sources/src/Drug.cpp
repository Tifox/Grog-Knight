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
 * File: Drug.cpp
 * Creation: 2015-05-07 17:43
 * Manon Budin <mbudin@student.42.fr>
 */

# include "Drug.hpp"

//!Constructor called by the Druglist class to parse all Drugs
/**
 * Default constructor, using the element that called the attack
 * @param name (std::string)
 */
Drug::Drug(std::string name) : _name(name) {
	this->_readFile(name);
	this->_name = getName();
	this->_flavor = getFlavor();
	this->_effect = getEffect();
	this->addAttribute("type3", "Drug");
	this->addAttribute("name", this->_name);
	this->addAttribute("flavor", this->_flavor);
}

//!Constructor called by hero/equipment class, to copy a parsed version from Druglist
/**
 * Copy constructor
 * @param Drug (Drug*)
 */

Drug::Drug(Drug* Drug) {
	this->_name = Drug->getName();
	this->_flavor = Drug->getFlavor();
	this->_effect = Drug->getEffect();
	this->addAttribute("type3", "Drug");
	this->addAttribute("name", this->_name);
	this->addAttribute("flavor", this->_flavor);
}


//! Basic destructor

Drug::~Drug(void) {
}

//!Json parsing for the Drug files (differs a bit from the Characters, that's why we have both)
/**
 * Read a config file, base on the name of the class
 * @param: name (std::string)
 */
void	Drug::_readFile(std::string name) {
	std::string			file;
	std::stringstream	buffer;
	std::ifstream		fd;

	file = "./Resources/Elements/Drugs/" + name + ".json";
	fd.open(file.c_str());
	if (!fd.is_open())
		Log::error("Can't open the file for the " +
				   name + " class. (Expected path is Resources/Elements/" + name +".json)");
	buffer << fd.rdbuf();
	this->_parseJson(buffer.str());
}

//!Follow from the json parser, stocks value
/**
 * Parse, read and stock the info in the config file
 * @param: file (std::string)
 * @note: file is the whole file content
 */
void    Drug::_parseJson(std::string file) {
	Json::Reader    read;
	Json::Value     json;
	Json::ValueIterator i, v;
	std::map<std::string, Json::Value>  tmp;

	if (!read.parse(file, json))
		Log::error("Error in json syntax :\n" + read.getFormattedErrorMessages());
	if (this->_name != json["infos"].get("name", "").asString())
		Log::warning("The class name is different with the name in the config file: " + this->_name + "/" + json["infos"].get("name", "").asString());
	this->_name = json["infos"].get("name", "").asString();
	this->_flavor = json["infos"].get("flavor", "").asString();
	this->_sprite = json["infos"].get("sprites", "").asString();
	this->_effect = json["infos"].get("effect", 0).asInt();
		std::cout << this->getEffect() << std::endl;

	this->addAttribute("type3", "Drug");
	this->addAttribute("sprite", this->_sprite);
}

//! Function called to get an attr value from the parsed json
/**
 * Get a Json::Value of a key in the config file
 * @param: category (std::string)
 * @param: key (std::string)
 * @note: See the docs for the utilisation of Json::Value
 */
Json::Value     Drug::_getAttr(std::string category, std::string key) {
	if (this->_attr.find(category) != this->_attr.end()) {
		if (this->_attr[category].find(key) != this->_attr[category].end())
			return this->_attr[category][key];
		else
			Log::warning("The key " + key + " not in the category " + category);
	} else {
		Log::warning("The category " + category + " not in the config");
	}
	return nullptr;
}

void	Drug::ReceiveMessage(Message *m) {
	if (m->GetMessageName() == "endBonus") {
		if (this->_curDrug == "pot")
			this->pot(0);
		if (this->_curDrug == "cocaine")
			this->cocaine(0);
		if (this->_curDrug == "morphine")
			this->morphine(0);
	}
	else if (m->GetMessageName() == "endMalus") {
		if (this->_curDrug == "pot")
			this->pot(2);
		if (this->_curDrug == "cocaine")
			this->cocaine(2);
		if (this->_curDrug == "morphine")
			this->morphine(2);
	}
}

/* GETTERS */
std::string		Drug::getName(void) { return this->_name; }
std::string		Drug::getFlavor(void) { return this->_flavor; }
int 			Drug::getEffect(void) { return this->_effect; }


/* EFFECTS */


void			Drug::pot(int status) {
	Characters *hero = Game::currentGame->getHero();

	if (status == 1) {
		Game::getHUD()->setText(":)", hero, Vector3(0, 255, 0), 1, 0);
		hero->buff.bonusDmg = hero->getWeapon()->getDamage();
		this->_curDrug = "pot";
		theSwitchboard.SubscribeTo(this, "endBonus");
		theSwitchboard.SubscribeTo(this, "endMalus");
		theSwitchboard.DeferredBroadcast(new Message("endBonus"), 15);
		theSwitchboard.DeferredBroadcast(new Message("endMalus"), 30);
	} else if(status == 0) {
		Game::getHUD()->setText(":(", hero, Vector3(255, 0, 0), 1, 0);
		hero->buff.bonusDmg = -(hero->getWeapon()->getDamage() / getEffect());
	}
	else if(status == 2) {
		hero->buff.bonusDmg = 0;
	}
}

void			Drug::cocaine(int status) {
	Characters *hero = Game::currentGame->getHero();

	if (status == 1) {
		Game::getHUD()->setText(":)", hero, Vector3(0, 255, 0), 1, 0);
		hero->buff.bonusSpeed = hero->_getAttr("forward", "force").asInt();
		this->_curDrug = "cocaine";
		theSwitchboard.SubscribeTo(this, "endBonus");
		theSwitchboard.SubscribeTo(this, "endMalus");
		theSwitchboard.DeferredBroadcast(new Message("endBonus"), 20);
		theSwitchboard.DeferredBroadcast(new Message("endMalus"), 30);
	} else if(status == 0) {
		Game::getHUD()->setText(":(", hero, Vector3(255, 0, 0), 1, 0);
		hero->buff.bonusSpeed = -(hero->_getAttr("forward", "force").asInt() / getEffect());
		}
	else if(status == 2) {
		hero->buff.bonusSpeed = 0;
	}
}


void			Drug::morphine(int status) {
	Characters *hero = Game::currentGame->getHero();
	std::cout << "EFFECT ==== > " << this->getEffect() << std::endl;

	if (status == 1) {
		Game::getHUD()->setText(":)", hero, Vector3(0, 255, 0), 1, 0);
		hero->setInvincibility(true);
	std::cout << "EFFECT ==== > " << this->getEffect() << std::endl;
		this->_curDrug = "morphine";
		theSwitchboard.SubscribeTo(this, "endBonus");
		theSwitchboard.SubscribeTo(this, "endMalus");
		theSwitchboard.DeferredBroadcast(new Message("endBonus"), 10);
		theSwitchboard.DeferredBroadcast(new Message("endMalus"), 30);
	} else if(status == 0) {
		Game::getHUD()->setText(":(", hero, Vector3(255, 0, 0), 1, 0);
		hero->setHP(getEffect());
		}
	else if(status == 2) {
		hero->setInvincibility(false);
	}
}