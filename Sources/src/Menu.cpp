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
Menu::Menu(void) : _currentChoice("Start Game"), _fadeActor(nullptr) {
	theSwitchboard.SubscribeTo(this, "enterPressed");
	theSwitchboard.SubscribeTo(this, "downPressed");
	theSwitchboard.SubscribeTo(this, "upPressed");
	theSwitchboard.SubscribeTo(this, "specialMove");
	theSwitchboard.SubscribeTo(this, "PauseGame");
	theSwitchboard.SubscribeTo(this, "deletePressed");
	theSwitchboard.SubscribeTo(this, "escape");
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
	theWorld.SetBackgroundColor(*(new Color(.26f, .26f, .26f)));
	this->_menuChoices.push_back("Start Game");
	this->_menuChoices.push_back("Settings");
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

	if (this->_inMenu == 1) {
		if (m->GetMessageName() == "enterPressed") {
			if (this->_currentChoice == "Start Game") {
				theSwitchboard.UnsubscribeFrom(this, "upPressed");
				theSwitchboard.UnsubscribeFrom(this, "downPressed");
				Game::removeHUDWindow(this->_window);
				this->_game->start();
				this->_window = Game::getHUD();
				this->_inMenu = 0;
			} else if (this->_currentChoice == "Settings") {
				this->removeBaseMenu();
				this->_currentChoice = "Anti-Aliasing";
				this->_lastMenu = 1;
				this->settings();
			}	else if (this->_currentChoice == "Exit")
				exit(0);
		} else if (m->GetMessageName() == "downPressed") {
			it = std::find(this->_menuChoices.begin(), this->_menuChoices.end(), this->_currentChoice);
			if ((++it) != this->_menuChoices.end()) {
				this->_currentChoice = *(it);
				this->listMenu();
			}
		} else if (m->GetMessageName() == "upPressed") {
			it = std::find(this->_menuChoices.begin(), this->_menuChoices.end(), this->_currentChoice);
			if (it != this->_menuChoices.begin()) {
				this->_currentChoice = *(--it);
				this->listMenu();
			}
		}
	} else if (this->_inMenu == 2) {
		if (m->GetMessageName() == "enterPressed") {
			std::map<std::string, int>::iterator	it;

			for (it = this->_settingsValues[this->_currentChoice].begin(); it != this->_settingsValues[this->_currentChoice].end(); it++) {
				if (it->second == 1) {
					if (++it != this->_settingsValues[this->_currentChoice].end()) {
						--it;
						this->_settingsValues[this->_currentChoice][it->first] = 0;
						++it;
						this->_settingsValues[this->_currentChoice][it->first] = 1;
					} else {
						--it;
						this->_settingsValues[this->_currentChoice][it->first] = 0;
						this->_settingsValues[this->_currentChoice][this->_settingsValues[this->_currentChoice].begin()->first] = 1;
					}
				}
			}
			if (this->_lastMenu == 3)
					this->settings(theCamera.GetWindowHeight() / 2 - 100);
				else
					this->settings();
		} else if (m->GetMessageName() == "downPressed") {
			std::map<std::string, std::map<std::string, int> >::iterator	it;
			it = this->_settingsValues.find(this->_currentChoice);
			if ((++it) != this->_settingsValues.end()) {
				this->_currentChoice = it->first;
				if (this->_lastMenu == 3)
					this->settings(theCamera.GetWindowHeight() / 2 - 100);
				else
					this->settings();
			}
		} else if (m->GetMessageName() == "upPressed") {
			std::map<std::string, std::map<std::string, int> >::iterator	it;
			it = this->_settingsValues.find(this->_currentChoice);
			if (it->first != this->_settingsValues.begin()->first) {
				it--;
				this->_currentChoice = it->first;
				if (this->_lastMenu == 3)
					this->settings(theCamera.GetWindowHeight() / 2 - 100);
				else
					this->settings();
			}
		} else if (m->GetMessageName() == "deletePressed") {
			this->removeSettings();
			this->applySettings();
			if (this->_lastMenu == 1) {
				this->_currentChoice = "Start Game";
				this->listMenu();
			} else if (this->_lastMenu == 3) {
				this->_currentChoice = "Settings";
				this->_inMenu = 3;
				this->pauseMenu();
			}
		}
	} else if (this->_inMenu == 3) {
		if (m->GetMessageName() == "downPressed") {
			std::list<std::string>::iterator	it;

			for (it = this->_pauseMenuText.begin(); it != this->_pauseMenuText.end(); it++) {
				if ((++it) != this->_pauseMenuText.end()) {
					this->_currentChoice = *it;
					this->pauseMenu();
				}
			}
		} else if (m->GetMessageName() == "upPressed") {
			std::list<std::string>::iterator	it;

			for (it = this->_pauseMenuText.begin(); it != this->_pauseMenuText.end(); it++) {
				if (it != this->_pauseMenuText.begin()) {
					--it;
					this->_currentChoice = *it;
					this->pauseMenu();
					break ;
				}
			}
		} else if (m->GetMessageName() == "enterPressed") {
			if (this->_currentChoice == "Quit") {
				exit(0);
			} else if (this->_currentChoice == "Settings") {
				std::list<std::string>::iterator		it;

				for (it = this->_pauseMenuText.begin(); it != this->_pauseMenuText.end(); it++)
					this->_window->removeText(*it);
				this->_window->removeText("PAUSE");
				this->_lastMenu = 3;
				this->_inMenu = 2;
				this->_currentChoice = "Anti-Aliasing";
				this->settings(theCamera.GetWindowHeight() / 2 - 100);
			}
		}
	}

	if (m->GetMessageName() == "escape") {
		if (this->_inMenu == 0) {
			std::list<std::string>				subs = Game::currentGame->getHero()->getSubscribes();
			std::list<std::string>::iterator	it;

			for (it = subs.begin(); it != subs.end(); it++)
				theSwitchboard.SubscribeTo(this, *it);
			Game::currentGame->getHero()->unsubscribeFromAll();
			theWorld.PausePhysics();
			this->_inMenu = 3;
			this->pauseMenu();
		} else if (this->_inMenu == 1 || this->_inMenu == 2){
			exit(0);
		} else {
			std::list<std::string>::iterator		it;

			for (it = this->_pauseMenuText.begin(); it != this->_pauseMenuText.end(); it++)
				this->_window->removeText(*it);
			theWorld.Remove(this->_fadeActor);
			this->_window->removeText("PAUSE");
			std::list<std::string>				subs = Game::currentGame->getHero()->getSubscribes();

			for (it = subs.begin(); it != subs.end(); it++)
				theSwitchboard.UnsubscribeFrom(this, *it);

			Game::currentGame->getHero()->subscribeToAll();
			theWorld.ResumePhysics();
			this->_inMenu = 0;
			this->_fadeActor = nullptr;
			this->_pauseMenuText.clear();
		}
	}
}

void	Menu::listMenu(void) {
	std::list<std::string>::iterator	it;
	int		x = (theCamera.GetWindowWidth() / 2), y = (theCamera.GetWindowHeight() / 2);

	this->_inMenu = 1;
	for (it = this->_menuChoices.begin(); it != this->_menuChoices.end(); it++)
		this->_window->removeText(*it);
	this->_window->removeText("Grog Like");
	for (it = this->_menuChoices.begin(); it != this->_menuChoices.end(); it++, y += 20) {
		if (this->_currentChoice == *it) {
			this->_window->setText(*it, x - ((*it).length() / 2 * 6), y, Vector3(255, 0, 0), 1);
		} else {
			this->_window->setText(*it, x - ((*it).length() / 2 * 6), y, Vector3(255, 255, 255), 1);
		}
	}
	this->_window->setText("Grog Like", (theCamera.GetWindowWidth() / 2) - 200, (theCamera.GetWindowHeight() / 2) - 100,
			Vector3(255.0f, 255.0f, 255.0f), 1, "title");
}

void	Menu::removeBaseMenu(void) {
	std::list<std::string>::iterator	it;

	for (it = this->_menuChoices.begin(); it != this->_menuChoices.end(); it++)
		this->_window->removeText(*it);
	this->_window->removeText("Grog Like");
}

void	Menu::settings(int y) {
	int		x = theCamera.GetWindowWidth() / 2;
	y = (y ? y : theCamera.GetWindowHeight() / 2 - 250);
	std::map<std::string, std::map<std::string, int> >::iterator	it;
	std::map<std::string, int>::iterator	it2;

	if (this->_settingsValues.size() == 0)
		this->parseSettings();
	this->_window->removeText("Settings");
	for (it = this->_settingsValues.begin(); it != this->_settingsValues.end(); it++) {
		this->_window->removeText(it->first);
		for (it2 = it->second.begin(); it2 != it->second.end(); it2++) 
			this->_window->removeText(it2->first);
	}

	this->_inMenu = 2;
	this->_window->setText("Settings", (x - 180), y, Vector3(255, 255, 255), 1, "title");

	y += 80;
	for (it = this->_settingsValues.begin(); it != this->_settingsValues.end(); it++, y += 20) {
		if (this->_currentChoice == it->first)
			this->_window->setText(it->first, x - 200, y, Vector3(255, 0, 0), 1);
		else
			this->_window->setText(it->first, x - 200, y, Vector3(255, 255, 255), 1);
		for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			if (it2->second == 1) {
				this->_window->setText(it2->first, x + 100, y, Vector3(255, 255, 255), 1);
			}
		}
	}
}

void	Menu::removeSettings(void) {
	std::map<std::string, std::map<std::string, int> >::iterator	it;
	std::map<std::string, int>::iterator	it2;

	this->_window->removeText("Settings");
	for (it = this->_settingsValues.begin(); it != this->_settingsValues.end(); it++) {
		this->_window->removeText(it->first);
		for (it2 = it->second.begin(); it2 != it->second.end(); it2++) 
			this->_window->removeText(it2->first);
	}
}

void	Menu::parseSettings(void) {
	std::stringstream 	buffer;
	std::ifstream		fd;
	Json::Reader	read;
	Json::Value		json;
	Json::ValueIterator i, v;
	int					j;

	fd.open("Config/Settings.json");
	if (!fd.is_open())
		Log::error("Can't find Config/Settings.json");
	buffer << fd.rdbuf();

	if (!read.parse(buffer, json))
		Log::error("Error in json syntax :\n" + read.getFormattedErrorMessages());

	for (i = json.begin(); i != json.end(); i++) {
		this->_settingsValues[i.key().asString()] = std::map<std::string, int>();
		for (j = 0, v = (*i).begin(); v != (*i).end(); v++, j++) {
			if (!j)
				this->_settingsValues[i.key().asString()][v.key().asString()] = 1;
			else
				this->_settingsValues[i.key().asString()][v.key().asString()] = 0;
		}
	}
}

void	Menu::applySettings(void) {
	std::map<std::string, std::map<std::string, int> >::iterator	it;
	std::map<std::string, int>::iterator							it2;

	for (it = this->_settingsValues.begin(); it != this->_settingsValues.end(); it++) {
		for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			if (it2->second == 1) {
				if (it->first == "Resolution") {
					if (it2->first == "1024x720") {
						glfwSetWindowSize(theWorld.GetMainWindow(), 1024, 720);
					} else if (it2->first == "1524x1020") {
						glfwSetWindowSize(theWorld.GetMainWindow(), 1524, 1020);
					} else if (it2->first == "FullScreen") {
						const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
						glfwSetWindowSize(theWorld.GetMainWindow(), mode->width, mode->height);
					}
					glfwSetWindowPos(theWorld.GetMainWindow(), 0, 0);
				} else if (it->first == "Anti-Aliasing") {
					if (it2->first == "Yes") {
						glfwWindowHint(GLFW_SAMPLES, 4);
					} else {
						glfwWindowHint(GLFW_SAMPLES, 0);
					}
				}
			}
		}
	}
}

void		Menu::pauseMenu(void) {
	if (this->_fadeActor == nullptr) {
		HUDActor	*fade = new HUDActor();

		fade->SetSize(100000, 100000);
		fade->SetColor(0, 0, 0, 0.5f);
		fade->SetLayer(100000);
		theWorld.Add(fade);
		this->_fadeActor = fade;
		this->_pauseMenuText.push_back("Settings");
		this->_pauseMenuText.push_back("Quit");
		this->_currentChoice = "Settings";
		goto render;
	} else {
render:
		std::list<std::string>::iterator	it;
		for (it = this->_pauseMenuText.begin(); it != this->_pauseMenuText.end(); it++)
			this->_window->removeText(*it);
		this->_window->removeText("PAUSE");

		int			x = theCamera.GetWindowWidth() / 2, y = theCamera.GetWindowHeight() / 2;

		this->_window->setText("PAUSE", (theCamera.GetWindowWidth() / 2) - 100, (theCamera.GetWindowHeight() / 2) - 100,
				Vector3(255.0f, 255.0f, 255.0f), 1, "title");
		for (it = this->_pauseMenuText.begin(); it != this->_pauseMenuText.end(); it++, y += 20) {
			if (*it == this->_currentChoice)
				this->_window->setText(*it, x - ((*it).length() / 2 * 6), y, Vector3(255, 0, 0), 1);
			else
				this->_window->setText(*it, x - ((*it).length() / 2 * 6), y, Vector3(255, 255, 255), 1);
		}
	}
}
