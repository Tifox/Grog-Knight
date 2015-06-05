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

Tooltip::Tooltip() {
	return ;
}

Tooltip::~Tooltip() {
	return ;
}

void	Tooltip::tip(Elements *elem, Characters *c) {
	std::string type = elem->getAttribute("type3");
	HUDWindow *hud = Game::getHUD();
	std::string val;
	
	if (type == "gold") {
		val = elem->getAttribute("value");
		/*hud->setText(val,
			(c->GetBody()->GetWorldCenter().x - Game::currentGame->maps->_XYMap[Game::currentY][Game::currentX].getXStart()),
			-(c->GetBody()->GetWorldCenter().y + Game::currentGame->maps->_XYMap[Game::currentY][Game::currentX].getYStart())+ 20,
			Vector3(246.0f, 255.0f, 0.0f), 1);*/
		TextActor	*t = new TextActor("Gamefont", "MDRLOL " + val);
		t->SetPosition(c->GetBody()->GetWorldCenter().x, c->GetBody()->GetWorldCenter().y + 20);
		t->SetSize(200);
		t->Render();
		theWorld.Add(t);

		std::cout << c->GetBody()->GetWorldCenter().x - Game::currentGame->maps->_XYMap[Game::currentY][Game::currentX].getXStart() << std::endl;
		std::cout << -(c->GetBody()->GetWorldCenter().y + Game::currentGame->maps->_XYMap[Game::currentY][Game::currentX].getYStart()+ 20) << std::endl;
	}
}
