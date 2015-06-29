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

Tooltip::Tooltip() : _name("") , _flavor(""){
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


	if (this->_name == "" && this->_flavor == "") {
		HUDActor *equip = new HUDActor();
		HUDWindow *hud = Game::getHUD();
		this->_equip = equip;
		this->_name = elem->getAttribute("name");
		this->_flavor = elem->getAttribute("flavor");

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

		equip->SetSize(200, 100);
		equip->SetPosition(theCamera.GetWindowWidth() - 100, 50);
		equip->SetColor(0, 0, 0);
		equip->SetDrawShape(ADS_Square);
		theWorld.Add(equip);
	}

	return ;
}

void	Tooltip::clearInfo(void) {
	std::string tmp;
	int i;

	theWorld.Remove(this->_equip);

	Game::getHUD()->removeText(this->_name);
	for (i = 0; i < this->_flavor.size(); i++) {
		if (this->_flavor[i] == '\n') {
			Game::getHUD()->removeText(tmp);
			tmp.clear();
		} else
			tmp = tmp + this->_flavor[i];
	}
	Game::getHUD()->removeText(tmp);

	this->_name = "";
	this->_flavor = "";

}

void 	Tooltip::ReceiveMessage(Message *m) {
	if (m->GetMessageName() == "deleteTip") {
		Game::getHUD()->removeText(this->_val);
	}
}
