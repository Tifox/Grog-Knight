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

Tooltip::Tooltip() : _name("") , _flavor(""), _hp("") , _mana(""), _lastElem(nullptr) {
	theSwitchboard.SubscribeTo(this, "deleteTip");
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
	} else 	if (type == "mana") {
		val = elem->getAttribute("value");
		hud->setText("+" + val , c, Vector3(255, 51, 255), 1, 0);
	}
	this->_val = "+" + val;
	theSwitchboard.DeferredBroadcast(new Message("deleteTip"), 2);

}

void 	Tooltip::info(Elements *elem) {
	std::string tmp;
	int i = 0;
	float	x = theCamera.GetWindowWidth() / 20 * 11.3;
	float	y = 30;

	if (this->_name == "" && this->_flavor == "" && this->_hp == "" && this->_mana == "") {
		HUDWindow *hud = Game::getHUD();
		this->_name = elem->getAttribute("name");
		this->_flavor = elem->getAttribute("flavor");
		int b;

		if (elem->getAttribute("type3") == "Armor") {
			b = atoi((elem->getAttribute("hpBuff").c_str())) - atoi(Game::currentGame->getHero()->getArmor()->getAttribute("hpBuff").c_str());
			this->_hp = std::to_string(b) + " HP";
			b = atoi((elem->getAttribute("manaBuff").c_str())) - atoi(Game::currentGame->getHero()->getArmor()->getAttribute("manaBuff").c_str());
			this->_mana = std::to_string(b) + " MP";
			}
		if (elem->getAttribute("type3") == "Ring") {
			b = atoi((elem->getAttribute("hpBuff").c_str())) - atoi(Game::currentGame->getHero()->getRing()->getAttribute("hpBuff").c_str());
			this->_hp = std::to_string(b) + " HP";
			b = atoi((elem->getAttribute("manaBuff").c_str())) - atoi(Game::currentGame->getHero()->getRing()->getAttribute("manaBuff").c_str());
			this->_mana = std::to_string(b) + " MP";
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
		if (this->_mana.find("-"))
			hud->setText(this->_mana , x + 60  + theCamera.GetWindowWidth() / 40 * 2, y + 15 + theCamera.GetWindowHeight() / 20 * 0.15,  Vector3(0, 1, 0), 1);
		else
			hud->setText(this->_mana , x + 60  + theCamera.GetWindowWidth() / 40 * 2, y + 15 + theCamera.GetWindowHeight() / 20 * 0.15,  Vector3(1, 0, 0), 1);
	}
	return ;
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

	Game::getHUD()->removeText(tmp);
	Game::getHUD()->removeText(this->_hp);
	Game::getHUD()->removeText(this->_mana);
	this->_name = "";
	this->_flavor = "";
	this->_hp = "";
	this->_mana = "";

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
}
