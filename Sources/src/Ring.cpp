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
 * File: Ring.cpp
 * Creation: 2015-05-07 17:43
 * Manon Budin <mbudin@student.42.fr>
 */

# include "Ring.hpp"

//!Constructor called by the Ringlist class to parse all Rings
/**
 * Default constructor, using the element that called the attack
 * @param name (std::string)
 */
Ring::Ring(std::string name) : _name(name) {
	this->_readFile(name);
}

//!Constructor called by hero/equipment class, to copy a parsed version from Ringlist
/**
 * Copy constructor
 * @param Ring (Ring*)
 */

Ring::Ring(Ring* Ring) {
	this->_name = Ring->getName();
	this->_displayName = Ring->getDisplayName();
	this->_flavor = Ring->getFlavor();
	this->_sprite = Ring->getSprite();
	this->SetLayer(70);
	this->addAttribute("type3", "Ring");
	this->addAttribute("name", this->_name);
	this->addAttribute("displayName", this->_displayName);
	this->addAttribute("flavor", this->_flavor);
	this->addAttribute("sprite", this->_sprite);
	this->_lootLevel = Ring->getLootLevel();
	if (Ring->getAttribute("hpBuff") != "")
		this->addAttribute("hpBuff", Ring->getAttribute("hpBuff"));
	if (Ring->getAttribute("dmgReduc") != "")
		this->addAttribute("dmgReduc", Ring->getAttribute("dmgReduc"));
	if (Ring->getAttribute("bonusSpeed") != "")
		this->addAttribute("bonusSpeed", Ring->getAttribute("bonusSpeed"));
	if (Ring->getAttribute("bonusDmg") != "")
		this->addAttribute("bonusDmg", Ring->getAttribute("bonusDmg"));
}


//! Basic destructor

Ring::~Ring(void) {
}

//!Json parsing for the Ring files (differs a bit from the Characters, that's why we have both)
/**
 * Read a config file, base on the name of the class
 * @param: name (std::string)
 */
void	Ring::_readFile(std::string name) {
	std::string			file;
	std::stringstream	buffer;
	std::ifstream		fd;

	file = "./Resources/Elements/Rings/" + name + ".json";
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
void    Ring::_parseJson(std::string file) {
	Json::Reader    read;
	Json::Value     json;
	Json::ValueIterator i, v;
	std::map<std::string, Json::Value>  tmp;

	if (!read.parse(file, json))
		Log::error("Error in json syntax :\n" + read.getFormattedErrorMessages());
	if (this->_name != json["infos"].get("name", "").asString())
		Log::warning("The class name is different with the name in the config file: " + this->_name + "/" + json["infos"].get("name", "").asString());
	this->_displayName = json["infos"].get("displayName", "").asString();
	this->_name = json["infos"].get("name", "").asString();
	this->_flavor = json["infos"].get("flavor", "").asString();
	this->_lootLevel = json["infos"].get("lootLevel", "").asInt();
	this->_price = json["infos"].get("price", "").asInt();
	this->_sprite = json["infos"].get("sprites", "").asString();
	for (i = json["bonus"].begin(); i != json["bonus"].end(); i++)
		this->addAttribute( i.key().asString(), (*i).asString());
	this->addAttribute("type3", "Armor");
	this->addAttribute("type3", "Ring");
	this->addAttribute("sprite", this->_sprite);
}

//! Function called to get an attr value from the parsed json
/**
 * Get a Json::Value of a key in the config file
 * @param: category (std::string)
 * @param: key (std::string)
 * @note: See the docs for the utilisation of Json::Value
 */
Json::Value     Ring::_getAttr(std::string category, std::string key) {
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

void	Ring::ReceiveMessage(Message *m) {
	if (m->GetMessageName() == "DeleteEquipment" + this->GetName()) {
		this->SetLayer(-1);
		Game::addToDestroyList(this);
	}
	if (m->GetMessageName() == "setToStatic" + this->GetName()) {
		this->GetBody()->SetType(b2_staticBody);
	}
}

/* GETTERS */
std::string		Ring::getName(void) { return this->_name; }
std::string		Ring::getFlavor(void) { return this->_flavor; }
std::string		Ring::getSprite(void) { return this->_sprite; }
std::string		Ring::getDisplayName(void) { return this->_displayName; }
int				Ring::getLootLevel(void) { return this->_lootLevel; }
int				Ring::getPrice(void) { return this->_price; }

/* SETTERS */

void	Ring::BeginContact(Elements *elem, b2Contact *contact) {
}

void	Ring::EndContact(Elements *elem, b2Contact *contact) {
}

