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
Menu::Menu(void) : _currentChoice("Start Game") {
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
	w->setText("Grog Like", (theCamera.GetWindowWidth() / 2) - 200, (theCamera.GetWindowHeight() / 2) - 100,
		Vector3(255.0f, 255.0f, 255.0f), 1, "title");
	theWorld.SetBackgroundColor(*(new Color(0, 0, 0)));
	theCamera.SetPosition(0, 0);
	this->_menuChoices.push_back("Start Game");
	this->_menuChoices.push_back("Exit");
	this->_window = w;
	this->listMenu();
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
	std::list<std::string>::iterator	it;

	if (Game::started != 1) {
		if (m->GetMessageName() == "enterPressed") {
			theSwitchboard.UnsubscribeFrom(this, "enterPressed");
			theSwitchboard.UnsubscribeFrom(this, "upPressed");
			theSwitchboard.UnsubscribeFrom(this, "downPressed");
			Game::removeHUDWindow(this->_window);
			if (this->_currentChoice == "Start Game")
				this->_game->start();
			else if (this->_currentChoice == "Exit")
				exit(0);
		} else if (m->GetMessageName() == "downPressed") {
			it = std::find(this->_menuChoices.begin(), this->_menuChoices.end(), this->_currentChoice);
			this->_currentChoice = *(++it);
			this->listMenu();
		} else if (m->GetMessageName() == "upPressed") {
			it = std::find(this->_menuChoices.begin(), this->_menuChoices.end(), this->_currentChoice);
			this->_currentChoice = *(--it);
			this->listMenu();
		}
	}
}

void	Menu::listMenu(void) {
	std::list<std::string>::iterator	it;
	int		x = (theCamera.GetWindowWidth() / 2), y = (theCamera.GetWindowHeight() / 2);

	for (it = this->_menuChoices.begin(); it != this->_menuChoices.end(); it++)
		this->_window->removeText(*it);
	for (it = this->_menuChoices.begin(); it != this->_menuChoices.end(); it++, y += 20) {
		if (this->_currentChoice == *it) {
			this->_window->setText(*it, x - ((*it).length() / 2 * 6), y, Vector3(255, 0, 0), 1);
		} else {
			this->_window->setText(*it, x - ((*it).length() / 2 * 6), y, Vector3(255, 255, 255), 1);
		}
	}
	
}
