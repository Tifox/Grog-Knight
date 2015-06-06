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

Tooltip::Tooltip() : _name("") {
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
	if (this->_name == "") {
		HUDActor *equip = new HUDActor();
		HUDWindow *hud = Game::getHUD();
		this->_equip = equip;
		this->_name = elem->getAttribute("name");

		equip->SetSize(200, 100);
		equip->SetPosition(theCamera.GetWindowWidth() - 100, 50);
		equip->SetColor(0, 0, 0);
		equip->SetDrawShape(ADS_Square);
		theWorld.Add(equip);	
		hud->setText(this->_name,theCamera.GetWindowWidth() - 100, 50);
	}
	return ;
}

void	Tooltip::clearInfo(void) {
	theWorld.Remove(this->_equip);
	Game::getHUD()->removeText(this->_name);
}

void 	Tooltip::ReceiveMessage(Message *m) {
	if (m->GetMessageName() == "deleteTip") {
		Game::getHUD()->removeText(this->_val);
	}
}
