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
 * File: Menu.cpp
 * Creation: 2015-04-07 21:11
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "Menu.hpp"

//! Basic constructor
Menu::Menu(void) {
	theSwitchboard.SubscribeTo(this, "enterPressed");
	theSwitchboard.SubscribeTo(this, "downPressed");
	theSwitchboard.SubscribeTo(this, "upPressed");
	theSwitchboard.SubscribeTo(this, "PauseGame");
// Rebind key
// Show text Menu
// While make a choice
// Launch game
}

//! Basic destructor
Menu::~Menu(void) {

}

//! The init menu function
/**
 * Init the world, show the menu, and set the game for the future.
 * @param game The Main game class
 * @sa Game
 */
void	Menu::showMenu(Game *game) {
	HUDWindow		*w = new HUDWindow();

	this->_game = game;
	w->setText("grog \\grog\\: spirits (originally rum) mixed with water.", theCamera.GetWindowWidth() / 2 - 100, theCamera.GetWindowHeight() / 2 - 20, Vector3(255.0f, 255.0f, 255.0f), 1);
	w->setText("drunk  \\drungk\\ v. Past participle of drink. adj. 1. a. Intoxicated with alcoholic liquor to the point of impairment of physical and mental faculties.", theCamera.GetWindowWidth() / 2 - 450, theCamera.GetWindowHeight() / 2, Vector3(255.0f, 255.0f, 255.0f), 1);
	w->setText("Press <ENTER> to start", theCamera.GetWindowWidth() / 2 - 40, theCamera.GetWindowHeight() / 2 + 40, 
		Vector3(255.0f, 255.0f, 255.0f), 1);
	theWorld.SetBackgroundColor(*(new Color(0, 0, 0)));
	theCamera.SetPosition(0, 0);
	this->_window = w;
	theWorld.Add(w);
	Game::addHUDWindow(w);
	theWorld.StartGame();
}

//! Broadcast message function
/**
 * The intern Broadcast function.
 * See Angel docs for more information
 * @param m The Message Object
 */
void	Menu::ReceiveMessage(Message *m) {
	if (m->GetMessageName() == "enterPressed") {
		theSwitchboard.UnsubscribeFrom(this, "enterPressed");
		theSwitchboard.UnsubscribeFrom(this, "upPressed");
		theSwitchboard.UnsubscribeFrom(this, "downPressed");
		Game::removeHUDWindow(this->_window);
		this->_game->start();
	} else if (m->GetMessageName() == "PauseGame") {
		//theWorld.PauseSimulation();
	}
}
