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
	theSwitchboard.SubscribeTo(this, "escapePressed");
	theSwitchboard.SubscribeTo(this, "moveMenu");
	theSwitchboard.SubscribeTo(this, "PauseGame");
	theSwitchboard.SubscribeTo(this, "bossEnd");
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
	Maps			*maps = new Maps("Maps/background/");
	maps->readMaps();
	maps->_maps[3]->setXStart(0);
	maps->_maps[3]->setYStart(-40);
	maps->_maps[3]->display();
	this->_background_map = maps->_maps[3];
	delete(maps);

	this->_game = game;
	theCamera.SetPosition(27, -60, 8.502);
	this->_menuChoices.push_back("Start Game");
	this->_menuChoices.push_back("Settings");
	this->_menuChoices.push_back("Bindings");
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

	if (m->GetMessageName() == "PauseGame")
		Game::currentGame->endingGame();
	if (m->GetMessageName() == "enterPressed" && Game::deadWaiting) {
		if (!Game::lvlDone) {
			Game::currentGame->menuInGame();
		} else {
			Game::lvlDone = 0;
			Game::World++;
			Game::menuCharacter = nullptr;
			Game::isInMenu = 0;
			Game::currentGame->start();
			Game::started = 1;
		}
	}
	if (Game::toggleMenu == false)
		return;
	if (m->GetMessageName() == "moveMenu")
		this->_background_map->destroyMap();
	if (m->GetMessageName() == "bossEnd") {
		Game::endGame = true;
		Game::lvlDone = 1;
		Game::destroyAllBodies("LEVEL CLEARED");
	}
	if (this->_inMenu == 1) {
		if (m->GetMessageName() == "enterPressed") {
			if (this->_currentChoice == "Start Game") {
				Game::isInMenu = 0;
				Game::removeHUDWindow(this->_window);
				this->_game->menuInGame();
				this->_window = Game::getHUD();
				this->_inMenu = 0;
			} else if (this->_currentChoice == "Settings") {
				this->removeBaseMenu();
				this->_currentChoice = "Anti-Aliasing";
				this->_lastMenu = 1;
				this->settings();
			} else if (this->_currentChoice == "Exit") {
				Quit::quitGame();
			} else if (this->_currentChoice == "Bindings") {
				this->removeBaseMenu();
				this->_lastMenu = 1;
				this->_inMenu = 4;
				this->bindingMenu();
			}
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

			for (it = this->_pauseMenuText.begin(); it != this->_pauseMenuText.end() && (*it) != this->_currentChoice; it++);
			if ((++it) != this->_pauseMenuText.end()) {
				this->_currentChoice = *it;
				this->pauseMenu();
			}
		} else if (m->GetMessageName() == "upPressed") {
			std::list<std::string>::iterator	it;

			for (it = this->_pauseMenuText.begin(); it != this->_pauseMenuText.end() && (*it) != this->_currentChoice; it++);
			if (it != this->_pauseMenuText.begin()) {
				--it;
				this->_currentChoice = *it;
				this->pauseMenu();
			}
		} else if (m->GetMessageName() == "enterPressed") {
			if (this->_currentChoice == "Quit") {
				Quit::quitGame();
			} else if (this->_currentChoice == "Settings") {
				std::list<std::string>::iterator		it;

				for (it = this->_pauseMenuText.begin(); it != this->_pauseMenuText.end(); it++)
					this->_window->removeText(*it);
				this->_window->removeText("PAUSE");
				this->_lastMenu = 3;
				this->_inMenu = 2;
				this->_currentChoice = "Anti-Aliasing";
				this->settings(theCamera.GetWindowHeight() / 2 - 100);
			} else if (this->_currentChoice == "Bindings") {
				std::list<std::string>::iterator		it;

				for (it = this->_pauseMenuText.begin(); it != this->_pauseMenuText.end(); it++)
					this->_window->removeText(*it);
				this->_window->removeText("PAUSE");
				this->_lastMenu = 3;
				this->_inMenu = 4;
				this->bindingMenu(theCamera.GetWindowHeight() / 2 - 175);
			}
		}
	} else if (this->_inMenu == 4) {
		if (m->GetMessageName() == "downPressed") {
			std::map<std::string, std::list<t_bind *> >::iterator	it = this->_bindingIterator;
			std::list<t_bind *>::iterator	it2;

			for (it2 = it->second.begin(); it2 != it->second.end() && (*it2)->name != this->_currentChoice; it2++);
			if (++it2 == it->second.end()) {
				if (++it != this->_bindingMenu.end() && it->first != "General") {
					this->_bindingIterator = it;
					this->_currentChoice = (*it->second.begin())->name;
				}
			} else {
				this->_currentChoice = (*it2)->name;
			}
			if (this->_lastMenu == 3)
				this->bindingMenu(theCamera.GetWindowHeight() / 2 - 175);
			else
				this->bindingMenu();
		} else if (m->GetMessageName() == "upPressed") {
			std::map<std::string, std::list<t_bind *> >::iterator	it = this->_bindingIterator;
			std::list<t_bind *>::iterator	it2;

			for (it2 = it->second.begin(); it2 != it->second.end() && (*it2)->name != this->_currentChoice; it2++);
			if (it2 == it->second.begin()) {
				if (it != this->_bindingMenu.begin()) {
					--it;
					if (it->first != "General") {
						this->_bindingIterator = it;
						it2 = it->second.end();
						it2--;
						this->_currentChoice = (*it2)->name;
					}
				}
			} else {
				it2--;
				this->_currentChoice = (*it2)->name;
			}
			if (this->_lastMenu == 3)
				this->bindingMenu(theCamera.GetWindowHeight() / 2 - 175);
			else
				this->bindingMenu();
		} else if (m->GetMessageName() == "enterPressed") {
			std::map<std::string, std::list<t_bind *> >::iterator	it = this->_bindingIterator;
			std::list<t_bind *>::iterator	it2;

			for (it2 = it->second.begin(); it2 != it->second.end() && (*it2)->name != this->_currentChoice; it2++);
			this->_window->removeText((*it2)->realKey);
			(*it2)->oldKey = (*it2)->realKey;
			(*it2)->realKey = "<Press a Key>";
			if (this->_lastMenu == 3)
				this->bindingMenu(theCamera.GetWindowHeight() / 2 - 175);
			else
				this->bindingMenu();
			Game::isWaitingForBind = 1;
		} else if (m->GetMessageName() == "deletePressed") {
			std::map<std::string, std::list<t_bind *> >::iterator	it;
			std::list<t_bind *>::iterator							it2;

			if (this->applyBindings()) {
				for (it = this->_bindingMenu.begin(); it != this->_bindingMenu.end(); it++) {
					this->_window->removeText(it->first);
					for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
						this->_window->removeText((*it2)->name);
						this->_window->removeText((*it2)->realKey);
					}
				}
				this->_window->removeText("Bindings");
				if (this->_lastMenu == 1) {
					this->_inMenu = 1;
					this->_currentChoice = "Start Game";
					this->listMenu();
				} else if (this->_lastMenu == 3) {
					this->_inMenu = 3;
					this->_currentChoice = "Settings";
					this->pauseMenu();
				}
			}
		}
	}

	if (m->GetMessageName() == "escapePressed") {
		if (this->_inMenu == 0) {
			std::list<std::string>				subs = Game::currentGame->getHero()->getSubscribes();
			std::list<std::string>::iterator	it;

			for (it = subs.begin(); it != subs.end(); it++)
				theSwitchboard.SubscribeTo(this, *it);
				Game::currentGame->getHero()->unsubscribeFromAll();
				theWorld.PausePhysics();
				this->_inMenu = 3;
				Game::isPaused = 1;
			this->pauseMenu();
		} else if (this->_inMenu == 1) {
			Quit::quitGame();
		} else if (this->_inMenu == 2 || this->_inMenu == 4) {
			this->ReceiveMessage(new Message("deletePressed"));
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
			Game::isPaused = 0;
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
	this->_window->setText("Grog Knight", (theCamera.GetWindowWidth() / 2) - 200, (theCamera.GetWindowHeight() / 2) - 100,
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

void	Menu::parseBindings(void) {
	std::stringstream 	buffer;
	std::ifstream		fd;
	Json::Reader	read;
	Json::Value		json;
	Json::ValueIterator i, v, j;
	t_bind				*tmp;

	fd.open("Config/Bindings.json");
	if (!fd.is_open())
		Log::error("Can't find Config/Settings.json");
	buffer << fd.rdbuf();

	if (!read.parse(buffer, json))
		Log::error("Error in json syntax :\n" + read.getFormattedErrorMessages());

	for (i = json.begin(); i != json.end(); i++) {
		this->_bindingMenu[i.key().asString()] = std::list<t_bind *>();
		for (v = (*i).begin(); v != (*i).end(); v++) {
			tmp = new t_bind();
			tmp->name = v.key().asString();
			for (j = (*v).begin(); j != (*v).end(); j++) {
				if (j.key().asString() == "broadcast")
					tmp->broadcast = (*j).asString();
				else if (j.key().asString() == "key") {
					tmp->key = theInput.GetHashFromKeyName((*j).asString());
					tmp->realKey = (*j).asString();
				} else if (j.key().asString() == "controller")
					tmp->controller = (*j).asString();
			}
			this->_bindingMenu[i.key().asString()].push_back(tmp);
		}
	}
	this->_bindingIterator = this->_bindingMenu.begin();
	this->_bindingIterator++;
	this->_currentChoice = (*(this->_bindingIterator)->second.begin())->name;
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
						theCamera.ResizeCallback(theWorld.GetMainWindow(), 1024, 720);
					} else if (it2->first == "1524x1020") {
						glfwSetWindowSize(theWorld.GetMainWindow(), 1524, 1020);
						theCamera.ResizeCallback(theWorld.GetMainWindow(), 1524, 1020);
					} else if (it2->first == "FullScreen") {
						const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
						glfwSetWindowSize(theWorld.GetMainWindow(), mode->width, mode->height);
						theCamera.ResizeCallback(theWorld.GetMainWindow(), mode->width, mode->height);
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
	if (Game::started == 1) {
		Game::reloadHUD = 1;
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
		this->_pauseMenuText.push_back("Bindings");
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

void	Menu::getBind(int key) {
	std::map<std::string, std::list<t_bind *> >::iterator	it = this->_bindingIterator;
	std::list<t_bind *>::iterator	it2, tmp;

	for (it2 = it->second.begin(); it2 != it->second.end() && (*it2)->name != this->_currentChoice; it2++);
	this->_window->removeText((*it2)->realKey);
	if (key <= 90 && key >= 65)
		key += 32;
	(*it2)->key = key;
	(*it2)->realKey = theInput.GetKeyNameFromHash(key);
	tmp = it2;

	for (it = this->_bindingMenu.begin(); it != this->_bindingMenu.end(); it++) {
		for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			if ((*it2)->key == (*tmp)->key && it2 != tmp) {
				(*it2)->key = 0;
				(*it2)->realKey = "<None>";
			}
		}
	}
	if (this->_lastMenu == 3)
		this->bindingMenu(theCamera.GetWindowHeight() / 2 - 175);
	else
		this->bindingMenu();
	Game::isWaitingForBind = 0;
}

void	Menu::bindingMenu(int y) {
	int		x = theCamera.GetWindowWidth() / 2;
	y = (y ? y : theCamera.GetWindowHeight() / 2 - 250);
	std::map<std::string, std::list<t_bind *> >::iterator	it;
	std::list<t_bind *>::iterator							it2;

	if (!this->_bindingMenu.size()) {
		this->parseBindings();
	}

	for (it = this->_bindingMenu.begin(); it != this->_bindingMenu.end(); it++) {
		this->_window->removeText(it->first);
		for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			this->_window->removeText((*it2)->name);
			this->_window->removeText((*it2)->realKey);
		}
	}
	this->_window->removeText("Bindings");

	this->_window->setText("Bindings", (x - 180), y, Vector3(255, 255, 255), 1, "title");
	y += 70;
	for (it = this->_bindingMenu.begin(); it != this->_bindingMenu.end(); it++) {
		if (it->first != "General") {
			this->_window->setText(it->first, x - ((it->first).length() / 2 * 16) - 50, y, Vector3(255, 255, 255), 1, "smallTitle");
			y += 20;
			for (it2 = it->second.begin(); it2 != it->second.end(); it2++, y += 20) {
				if (this->_currentChoice == (*it2)->name)
					this->_window->setText((*it2)->name, x - 150, y, Vector3(255, 0, 0), 1);
				else
					this->_window->setText((*it2)->name, x - 150, y, Vector3(255, 255, 255), 1);
				this->_window->setText((*it2)->realKey, x + 50, y, Vector3(255, 255, 255), 1);
			}
			y += 20;
		}
	}
}

int		Menu::applyBindings(void) {
	std::map<std::string, std::list<t_bind *> >::iterator	it;
	std::list<t_bind *>::iterator							it2;

	for (it = this->_bindingMenu.begin(); it != this->_bindingMenu.end(); it++) {
		for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			if ((*it2)->key == 0)
				return 0;
		}
	}

	for (it = this->_bindingMenu.begin(); it != this->_bindingMenu.end(); it++) {
		for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			theInput.UnbindKey((*it2)->oldKey);
			if (this->_isUpper((*it2)->broadcast)) {
				theInput.BindKey((*it2)->realKey, (*it2)->broadcast);
			} else {
				theInput.BindKey((*it2)->realKey, "+" + (*it2)->broadcast + "Pressed");
				theInput.BindKey((*it2)->realKey, "-" + (*it2)->broadcast + "Released");
			}
		}
	}
	return 1;
}

int		Menu::_isUpper(std::string s) {
	int		i;

	for (i = 0; i < s.size(); i++) {
		if (s[i] >= 'A' && s[i] <= 'Z')
			return 1;
	}
	return 0;
}

std::map<std::string, std::list<t_bind *> >		Menu::getBindings(void) { return this->_bindingMenu; };
