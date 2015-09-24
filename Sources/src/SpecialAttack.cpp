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
 * File: SpecialMoves.cpp
 * Creation: 2015-06-30 09:57
 * Matthieu Maudet<arkhane84@gmail.com>
 */

# include "SpecialAttack.hpp"
# include <cstdlib>

//! Base constructor
SpecialAttack::SpecialAttack(void) {

}

SpecialAttack::~SpecialAttack(void) {

}

SpecialAttack::SpecialAttack(Characters* charac) {
	std::string		file;
	std::stringstream 	buffer;
	std::ifstream		fd;
	Json::Reader		read;
	Json::Value		json;
	Json::ValueIterator i, v;
	std::map<std::string, Json::Value>	tmp;
	this->character = charac;
	file = "Resources/Elements/SpecialAttack.json";
	fd.open(file.c_str());
	if (!fd.is_open())
		Log::error("Can't open the file for the specialmoves class.");
	buffer << fd.rdbuf();
	if (!read.parse(buffer, json))
		Log::error("Error in json syntax :\n" + read.getFormattedErrorMessages());
	for (i = json.begin(); i != json.end(); i++) {
		for (v = (*i).begin(); v != (*i).end(); v++) {
			tmp[v.key().asString()] = (*v);
			this->character->_attr[i.key().asString()] = tmp;
		}
	}
	theSwitchboard.SubscribeTo(this, "SpecialAttackEnd");
}

void	SpecialAttack::ReceiveMessage(Message *m) {
	if (m->GetMessageName() == "SpecialAttackEnd") {
		if (this->_currentAttack == "whirlwind") {
			Game::currentGame->getHero()->buff.bonusSpeed = this->_previousSpeed;
			Game::currentGame->getHero()->_isWhirlwinding = false;
			this->_currentAttack = "";
			this->character->_speAttReady = 0;
		}
		else if (this->_currentAttack == "shockwave") {
			this->_currentAttack = "";
			std::cout << "message" << std::endl;
			this->character->_speAttReady = 1;
		}
	}
}
void	SpecialAttack::_whirlwind(void) {
	this->character->_setCategory("whirlwind");
	Weapon *currentWeapon = Game::currentGame->getHero()->getWeapon();
	Characters *hero = Game::currentGame->getHero();
	if (this->character->_isAttacking == 0 && this->character->_canMove == 1 && this->character->_speAttReady == 1 && currentWeapon->getType() == "Sword") {
		this->character->_isWhirlwinding = true;
		currentWeapon->setActive(this->character->_getAttr("whirlwind", "uptime").asFloat());
		new Weapon (Game::currentGame->getHero()->_weapon, Game::currentGame->getHero(), 1);
		new Weapon (Game::currentGame->getHero()->_weapon, Game::currentGame->getHero(), -1);
		this->_previousSpeed = hero->buff.bonusSpeed;
		hero->buff.bonusSpeed = -(hero->_getAttr("forward", "force").asInt() / 2);
		theSwitchboard.DeferredBroadcast(new Message("speAttReady"),
				this->character->_getAttr("cooldown").asFloat());
		theSwitchboard.DeferredBroadcast(new Message("SpecialAttackEnd"),
				this->character->_getAttr("whirlwind", "uptime").asFloat());
		this->_currentAttack = "whirlwind";
	}
}

void	SpecialAttack::_shockwave(void) {
	this->character->_setCategory("shockwave");
	Weapon *currentWeapon = Game::currentGame->getHero()->getWeapon();
	Characters *hero = Game::currentGame->getHero();
	if (this->character->_isAttacking == 0 && this->character->_canMove == 1 && this->character->_speAttReady == 1 && currentWeapon->getType() == "Axe") {
		this->character->_speAttReady = 0;
		this->character->_isShockWaving = true;
		theSwitchboard.DeferredBroadcast(new Message("speAttReady"),
				this->character->_getAttr("cooldown").asFloat());
		int dmg = this->character->_getAttr("shockwave", "damage").asInt();
		Projectile *wave = new Projectile(currentWeapon, dmg);
		this->_currentAttack = "shockwave";
		theSwitchboard.DeferredBroadcast(new Message("speAttReady"),
				this->character->_getAttr("cooldown").asFloat());
	}
}
