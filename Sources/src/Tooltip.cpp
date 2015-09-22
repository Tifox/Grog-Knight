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
 * File: Tooltip.cpp
 * Creation: 2015-06-03 14:57
 * Manon Budin <mbudin@student.42.fr>
 */

#include "Tooltip.hpp"

//! Constructor
/**
 * Basic constructor
 */

Tooltip::Tooltip() : _name("") , _flavor(""), _hp(""), _speed(""), _reduc(""), _dmg(""), _lastElem(nullptr), _talk("") {
	theSwitchboard.SubscribeTo(this, "deleteTip");
	theSwitchboard.SubscribeTo(this, "deleteTalk");
	return ;
}



//! Destructor
/**
 * Basic Destructor
 */

Tooltip::~Tooltip() {
	return ;
}
 
//! Parse the type of the loot
/**
 * Parse the type and set color and value
 * @param elem The Loot
 * @param c The Character
 */

void	Tooltip::tip(Elements *elem, Characters *c) {
	std::string type = elem->getAttribute("type3");
	HUDWindow *hud = Game::getHUD();
	std::string val;
	
	if (type == "gold") {
		val = elem->getAttribute("value");
		hud->setText("+" + val , c, Vector3(255, 204, 0), 1, 0);
	} else 	if (type == "HP") {
		val = elem->getAttribute("value");
		hud->setText("+" + val , c, Vector3(255, 0, 0), 1, 0);
	} /*else 	if (type == "mana") {*/
		//val = elem->getAttribute("value");
		//hud->setText("+" + val , c, Vector3(255, 51, 255), 1, 0);
	/*}*/
	this->_val = "+" + val;
	theSwitchboard.DeferredBroadcast(new Message("deleteTip"), 2);

}

void 	Tooltip::info(Elements *elem) { 
	std::string tmp;
	int i = 0;
	float	x = theCamera.GetWindowWidth() / 20 * 11.3;
	float	y = 30;

	clearInfo();
	if (this->_name == "" && this->_flavor == "" && this->_hp == "" && this->_reduc == "" && this->_speed == "" && this->_dmg == "") {
		HUDWindow *hud = Game::getHUD();
		this->_name = elem->getAttribute("displayName");
		this->_flavor = elem->getAttribute("flavor");
		int b;

		if (elem->getAttribute("type3") == "Armor") {
			b = atoi((elem->getAttribute("hpBuff").c_str())) - atoi(Game::currentGame->getHero()->getArmor()->getAttribute("hpBuff").c_str());
			this->_hp = std::to_string(b) + " HP";
			b = atoi((elem->getAttribute("dmgReduc").c_str())) - atoi(Game::currentGame->getHero()->getArmor()->getAttribute("dmgReduc").c_str());
			this->_reduc = std::to_string(b) + " Resist";
			b = atoi((elem->getAttribute("bonusSpeed").c_str())) - atoi(Game::currentGame->getHero()->getArmor()->getAttribute("bonusSpeed").c_str());
			this->_speed = std::to_string(b) + " Speed";
			b = atoi((elem->getAttribute("bonusDmg").c_str())) - atoi(Game::currentGame->getHero()->getArmor()->getAttribute("bonusDmg").c_str());
			this->_dmg = std::to_string(b) + " DMG";
			}
		if (elem->getAttribute("type3") == "Ring") {
			b = atoi((elem->getAttribute("hpBuff").c_str())) - atoi(Game::currentGame->getHero()->getRing()->getAttribute("hpBuff").c_str());
			this->_hp = std::to_string(b) + " HP";
			b = atoi((elem->getAttribute("dmgReduc").c_str())) - atoi(Game::currentGame->getHero()->getRing()->getAttribute("dmgReduc").c_str());
			this->_reduc = std::to_string(b) + " Resist";
			b = atoi((elem->getAttribute("bonusSpeed").c_str())) - atoi(Game::currentGame->getHero()->getRing()->getAttribute("bonusSpeed").c_str());
			this->_speed = std::to_string(b) + " Speed";
			b = atoi((elem->getAttribute("bonusDmg").c_str())) - atoi(Game::currentGame->getHero()->getRing()->getAttribute("bonusDmg").c_str());
			this->_dmg = std::to_string(b) + " DMG";
			}

		hud->setText(this->_name , x + theCamera.GetWindowWidth() / 40 * 2, y + theCamera.GetWindowHeight() / 20 * 0.15, Vector3(0, 0, 0), 1);
		
		for (y = 45; i < this->_flavor.size(); i++) {
			if (this->_flavor[i] == '\n') {
				hud->setText(tmp, x + theCamera.GetWindowWidth() / 40 * 2, y + theCamera.GetWindowHeight() / 20 * 0.15,  Vector3(0, 0, 0), 1);
				tmp.clear();
				y += 15;
			} else
				tmp = tmp + this->_flavor[i];
		}
		hud->setText(tmp, x + theCamera.GetWindowWidth() / 40 * 2, y + theCamera.GetWindowHeight() / 20 * 0.15,  Vector3(0, 0, 0), 1);
		
		if (this->_hp.find("-"))
			hud->setText(this->_hp , x + theCamera.GetWindowWidth() / 40 * 2, y + 15 + theCamera.GetWindowHeight() / 20 * 0.15,  Vector3(0, 1, 0), 1);
		else
			hud->setText(this->_hp , x + theCamera.GetWindowWidth() / 40 * 2, y + 15 + theCamera.GetWindowHeight() / 20 * 0.15,  Vector3(1, 0, 0), 1);
 		if (this->_reduc.find("-"))
			hud->setText(this->_reduc , x + 120 + theCamera.GetWindowWidth() / 40 * 2, y + 15 + theCamera.GetWindowHeight() / 20 * 0.15,  Vector3(0, 1, 0), 1);
		else
			hud->setText(this->_reduc , x + 120 + theCamera.GetWindowWidth() / 40 * 2, y + 15 + theCamera.GetWindowHeight() / 20 * 0.15,  Vector3(1, 0, 0), 1);
 		if (this->_speed.find("-"))
			hud->setText(this->_speed , x + theCamera.GetWindowWidth() / 40 * 2, y + 30 + theCamera.GetWindowHeight() / 20 * 0.15,  Vector3(0, 1, 0), 1);
		else
			hud->setText(this->_speed , x + theCamera.GetWindowWidth() / 40 * 2, y + 30 + theCamera.GetWindowHeight() / 20 * 0.15,  Vector3(1, 0, 0), 1);
 		if (this->_dmg.find("-"))
			hud->setText(this->_dmg , x + 120 + theCamera.GetWindowWidth() / 40 * 2, y + 30 + theCamera.GetWindowHeight() / 20 * 0.15,  Vector3(0, 1, 0), 1);
		else
			hud->setText(this->_dmg , x + 120 + theCamera.GetWindowWidth() / 40 * 2, y + 30 + theCamera.GetWindowHeight() / 20 * 0.15,  Vector3(1, 0, 0), 1);
	}
	return ;
}


void	Tooltip::talk(Elements *elem) {
	clearInfo();
	HUDWindow *hud = Game::getHUD();
	this->_name = elem->getAttribute("name");
	this->_talk = elem->getAttribute("talk");

	std::string tmp;
	int i = 0;
	float	x = theCamera.GetWindowWidth() / 20 * 11.3;
	float	y = 30;

	hud->setText(this->_name , x + theCamera.GetWindowWidth() / 40 * 2, y + theCamera.GetWindowHeight() / 20 * 0.15, Vector3(0, 0, 0), 1);
	
	for (y = 45; i < this->_talk.size(); i++) {
		if (this->_talk[i] == '\n') {
			hud->setText(tmp, x + theCamera.GetWindowWidth() / 40 * 2, y + theCamera.GetWindowHeight() / 20 * 0.15,  Vector3(0, 0, 0), 1);
			tmp.clear();
			y += 15;
		} else
			tmp = tmp + this->_talk[i];
	}
	hud->setText(tmp, x + theCamera.GetWindowWidth() / 40 * 2, y + theCamera.GetWindowHeight() / 20 * 0.15,  Vector3(0, 0, 0), 1);
	theSwitchboard.DeferredBroadcast(new Message("deleteTalk"), 5);
}

void	Tooltip::clearInfo(int clean) {
	std::string tmp;
	int i;


	Game::getHUD()->removeText(this->_name);

	for (i = 0; i < this->_flavor.size(); i++) {
		if (this->_flavor[i] == '\n') {
			Game::getHUD()->removeText(tmp);
			tmp.clear();
		} else
			tmp = tmp + this->_flavor[i];
	}

	for (i = 0; i < this->_talk.size(); i++) {
		if (this->_talk[i] == '\n') {
			Game::getHUD()->removeText(tmp);
			tmp.clear();
		} else
			tmp = tmp + this->_talk[i];
	}

	Game::getHUD()->removeText(tmp);
	Game::getHUD()->removeText(this->_hp);
	Game::getHUD()->removeText(this->_speed);
	Game::getHUD()->removeText(this->_reduc);
	Game::getHUD()->removeText(this->_dmg);
	this->_name = "";
	this->_flavor = "";
	this->_talk = "";
	this->_hp = "";
	this->_speed = "";
	this->_reduc = "";
	this->_dmg = "";

	if (clean == 0) {
		Elements	*w;
		tmp = Game::currentGame->getHero()->getInventory()->getCurrentFocus();
		if (tmp != "") {
			if (Game::wList->checkExists(tmp))
				w = new Weapon(Game::wList->getWeapon(tmp));
			else if (Game::aList->checkExists(tmp))
				w = new Armor(Game::aList->getArmor(tmp));
			else if (Game::rList->checkExists(tmp))
				w = new Ring(Game::rList->getRing(tmp));
			this->info(w);
		}
	}
}

void 	Tooltip::ReceiveMessage(Message *m) {
	if (m->GetMessageName() == "deleteTip") {
		Game::getHUD()->removeText(this->_val);
	}
	else if (m->GetMessageName() == "deleteTalk") {
		clearInfo(0);
	}
}
