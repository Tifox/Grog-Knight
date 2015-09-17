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
 * File: Game.cpp
 * @date 2015-02-13 07:20
 * @author Louis Solofrizzo <louis@ne02ptzero.me>
 */

#include "Game.hpp"

//! Basic constructor, set the window to default value
/**
 * Initialize the world window, init Physics.
 * (http://docs.angel2d.com/class_world.html#ae5d7e8d20d3e6fc93785ab2014ac0c13)
 * Load the maps with Maps::Maps
 * @sa Maps
 */
Game::Game(void) : _hero((new Characters())) {
	#ifdef __APPLE__
		theWorld.Initialize(1024, 720, NAME, true, false);
	#else
		theWorld.Initialize(1024, 720, NAME, false, false);
	#endif
	theWorld.SetupPhysics(Vector2(0, -20));
	GameContactListener *gListen = new GameContactListener();
	theWorld.GetPhysicsWorld().SetContactListener(gListen);
	ContactFilter *gFilter = new ContactFilter();
	theWorld.GetPhysicsWorld().SetContactFilter(gFilter);
	this->maps = new Maps("Maps/");
	this->_controller = new ControllerInputManager();
	return ;
}

/**
 * Constructor with custom window width & height
 * @param width The width of the window
 * @param heigh The height of the window
 */
Game::Game(unsigned int width, unsigned int height) : _hero((new Characters())) {
	theWorld.Initialize(width, height, NAME);
	theWorld.SetupPhysics();
	GameContactListener *gListen = new GameContactListener();
	theWorld.GetPhysicsWorld().SetContactListener(gListen);
	this->maps = new Maps("Maps/");
}

/**
 * Destructor
 */
Game::~Game(void) {
	return ;
}

/**
 * Add a grid to the world (DEBUG)
 */
void	Game::grid(void) {
	theWorld.Add(new GridActor(), -1);
}

/**
 * Let's start the game
 */
void	Game::start(void) {
	theWorld.SetBackgroundColor(*(new Color(0, 0, 0)));
	Game::wList = new WeaponList();
	Game::eList = new EnemyList();
	Game::aList = new ArmorList();
	Game::rList = new RingList();
	Game::dList = new DrugList();
	Game::chest = new Chest();

	this->tooltip = new Tooltip();
	int i;
	for (i = 0; i < 3; i++) {
		this->maps->_XYMap[0][i].destroyMap();
	}

	delete(Game::currentGame->maps);
	this->maps = new Maps("Maps/");
	this->maps->readMaps();
	Game::currentGame = this;
	Hero			*hero = new Hero(Game::menuCharacter->getHeroType());

	theWorld.Remove(Game::menuCharacter);
	theWorld.GetPhysicsWorld().DestroyBody(Game::menuCharacter->GetBody());
	LevelGenerator *levelGenerator = new LevelGenerator(4, 3, 60);
	levelGenerator->execute();
	this->_tmpMap = levelGenerator->getLevel();
	this->maps->displayLevel(this->_tmpMap);

//	Log::info("NbMaps: " + std::to_string(levelGenerator->getNbMaps()));
//	Log::info("NbEnemy:" + trouver le nombre d ennemy);
//	Log::info("NbMaps: " + info sur la sauvegarde (nivau du hero, gold etc...));

	Game::currentX = levelGenerator->getStartX();
	Game::currentY = levelGenerator->getStartY();
	theCamera.SetPosition(this->maps->getMapXY()[Game::currentY][Game::currentX].getXMid(),
						  this->maps->getMapXY()[Game::currentY][Game::currentX].getYMid() + 1.8, 9.001);
	this->maps->_XYMap[Game::currentY][Game::currentX] = this->maps->getMapXY()[Game::currentY][Game::currentX].display();

	this->displayHero(*(hero));
	hero->init();
	hero->setGold(0);
	hero->setLevel(this->_save["level"].asInt());
	Game::chest->applySave(this->_save);
	this->setHero(hero);
	this->displayHUD();
	hero->setStartingValues();
	Game::started = 1;
	Game::currentGame = this;
}

void	Game::menuInGame(void) {
	theWorld.SetBackgroundColor(*(new Color(0, 0, 0)));

	InGameMenu	*menu = new InGameMenu();
	Game::endGame = false;
	if (Game::currentGame == nullptr)
		Game::currentGame = this;
	MenuCharacter	*charac = new MenuCharacter();

	this->_save = Quit::getSave();

	menu->showMaps();
	charac->setXStart(this->maps->getMapXY()[Game::currentY][Game::currentX].getXMid() - 10);
	charac->setYStart(this->maps->getMapXY()[Game::currentY][Game::currentX].getYMid() - 4);
	theCamera.MoveTo(Vector3(this->maps->getMapXY()[Game::currentY][Game::currentX].getXMid(),
						  this->maps->getMapXY()[Game::currentY][Game::currentX].getYMid() + 1.8, 18.502), 1, true, "moveMenu");
	this->maps->_XYMap[Game::currentY][Game::currentX] = this->maps->getMapXY()[Game::currentY][Game::currentX].display();
	charac->display();
	if (Game::getHUD() == nullptr)
		Game::addHUDWindow(new HUDWindow());
	else {
		Game::getHUD()->removeText("Press Enter to restart.");
		Game::getHUD()->removeText("YOU ARE DEAD");
		theWorld.Remove(this->getHero()->getGhost());
		delete this->getHero();
	}
	this->setHero(static_cast<Characters *>(charac));
	Game::started = 1;
	Game::isInMenu = 1;
}

Map		Game::getCurrentMap(void) {
	if (this->_hero->inSpecialMap == 0)
		return (this->maps->getMapXY()[Game::currentY][Game::currentX]);
	else if (this->_hero->inSpecialMap == 1)
		return *(this->maps->bossMap);
	else if (this->_hero->inSpecialMap == 2) {
		return *(this->maps->secretMap);
	}
}

//! Read the maps
/**
 * Launch Maps::readMaps
 * @sa Maps
 */
void	Game::readMaps(void) {
	this->maps->readMaps();
}

//! Display the first map
/**
 * Call the function Maps::firstOne
 * @sa Maps
 */
void	Game::showMap(void) {
	//this->maps->displayLevel();
}

//! Display the Hero
/**
 * We add the hero to the world, with an init position (x, y)
 * @param Hero The Hero Object
 */
void	Game::displayHero(Elements & Hero) {
	//Here starts the game - parse the 1st map coordinates and hero start
	Hero.setXStart(this->maps->getMapXY()[Game::currentY][Game::currentX].getXMid());
	Hero.setYStart(this->maps->getMapXY()[Game::currentY][Game::currentX].getYMid());
	Hero.addAttribute("hero", "1");
	Hero.display();
}

//! Display the Enemy
/**
 * Add an Enemy to the world, with an init position (x, y)
 * @param Enemy The enemy object
 */
void	Game::displayEnemy(Elements & Enemy) {
	Enemy.setXStart(5);
	Enemy.setYStart(-5);
	Enemy.addAttribute("enemy", "1");
	Enemy.display();
}

//! Display the object
/**
 * Display the Object ?!
 * @fixme Does this function have a real utility now ?
 * @param Object The Object
 */
void	Game::displayObject(Elements & Object) {
	Object.setXStart(6);
	Object.setYStart(6);
	Object.addAttribute("Object", "1");
	Object.display();
}

//! Get an id, in order.
/**
 * Get the current id, for the intern elements map
 * @return The next id
 */
int		Game::getNextId(void) {
	return Game::currentIds;
}

void	Game::checkHeroPosition(void) {
	Game::currentGame->_controller->flag = 0;
	Game::currentGame->_controller->tick();
	if (Game::started == 1 && Game::currentGame->getHero() != nullptr) {
		Game::currentGame->moveCamera();
		Game::currentGame->simulateHeroItemContact();
		Game::currentGame->getHero()->characterLoop();
		Game::currentGame->reloadingHUD();
		if (Game::asToStart == 1)
			Game::currentGame->start();
			Game::asToStart = 0;
	}
}

void	Game::simulateHeroItemContact(void) {
  if (this->_hero->getItem() != nullptr && this->_hero->getItem()->GetBody()) {
		if ((this->_hero->getItem()->GetBody()->GetWorldCenter().x >=
			 this->_hero->GetBody()->GetWorldCenter().x + 1) ||
			(this->_hero->getItem()->GetBody()->GetWorldCenter().x <=
   			 this->_hero->GetBody()->GetWorldCenter().x - 1) ||
			(this->_hero->getItem()->GetBody()->GetWorldCenter().y >=
 			 this->_hero->GetBody()->GetWorldCenter().y + 1) ||
			(this->_hero->getItem()->GetBody()->GetWorldCenter().y <=
			this->_hero->GetBody()->GetWorldCenter().y - 1))
		this->_hero->EndContact(this->_hero->getItem(), nullptr);
	}
}

void	Game::moveCamera(void) {
	bool	asChanged = false;
	Map		&tmp = this->maps->_XYMap[Game::currentY][Game::currentX];

	if (this->_hero->inSpecialMap == 0) {
		if (this->_hero->GetBody()->GetWorldCenter().x >= (tmp.getXStart() + tmp.getWidth() - 0.5)) {
			this->getCurrentMap().destroyMap();
			Game::currentX++;
			asChanged = true;
		} else if (this->_hero->GetBody()->GetWorldCenter().x <= (tmp.getXStart() - 1)) {
			this->getCurrentMap().destroyMap();
		Game::currentX--;
		asChanged = true;
		} else if (this->_hero->GetBody()->GetWorldCenter().y >= tmp.getYStart()) {
			this->getCurrentMap().destroyMap();
			Game::currentY--;
			asChanged = true;
		} else if (this->_hero->GetBody()->GetWorldCenter().y <= (tmp.getYStart() - tmp.getHeight())) {
			this->getCurrentMap().destroyMap();
			Game::currentY++;
			asChanged = true;
		}
		if (asChanged) {
			this->_hero->destroyTarget();
			this->maps->_XYMap[Game::currentY][Game::currentX] = this->maps->getMapXY()[Game::currentY][Game::currentX].display();
			theCamera.SetPosition(this->maps->getMapXY()[Game::currentY][Game::currentX].getXMid(),
								  this->maps->getMapXY()[Game::currentY][Game::currentX].getYMid() + 1.8);
			if (Game::isInMenu == 0) {
				Game::getHUD()->updateBigMap();
				Game::getHUD()->minimap();
			}
			asChanged = false;
		}
	}
}

//! Add an Element
/**
 * Add an element to the intern map, to the static elements map.
 * This map is usefull for cleaning an all level, or fading out at death, etc.
 * @param elem The Elements to add at the list
 */
void	Game::addElement(Elements & elem) {
	if (Game::elementMap[elem.getId()])
		Game::elementMap[elem.getId()] = &elem;
	else {
		Game::elementMap[Game::currentIds] = &elem;
		Game::currentIds += 1;
	}
}

//! Delete an element
/**
 * Delete an element from the intern map
 * @param elem The element to delete (The reference is important !)
 */
void	Game::delElement(Elements* elem) {
	for (int i = 0; i < Game::elementMap.size(); i++) {
		if (Game::elementMap[i]) {
			if (Game::elementMap[i]->getId() == elem->getId()) {
				Game::elementMap.erase(i);
			}
		}
	}
}

//! Collision intern callbacks
/**
 * Call the collision callbacks on two objects
 * @param a The ID of the first object
 * @param b The ID of the second object
 */
void	Game::callCallbacks(int a, int b) {
	if (a != -1 && b != -1) {
		Game::elementMap[a]->callback(Game::elementMap[b]);
		Game::elementMap[b]->callback(Game::elementMap[a]);
	}
}

//! Destroy an element at the right time.
/**
 * Call to add an element to the destroy list after the tick()
 * This function DO NOT remove an element itself. We stock in a list, and after the World::tick,
 * we call a function to destroying each element of the list.
 * @param m The Element to destroy
 * @todo how the fuck does StringSet works? Needed here for the UnsubscribeFromAll
 */

void	Game::addToDestroyList(Elements *m) {
	StringSet sub;

	for (std::list<Elements*>::iterator it = Game::bodiesToDestroy.begin(); it != Game::bodiesToDestroy.end(); it++) {
		if ((*it) == m)
			return;
	}
	sub = theSwitchboard.GetSubscriptionsFor(m);
	for (StringSet::iterator k = sub.begin(); k != sub.end(); k++) {
		theSwitchboard.UnsubscribeFrom(m, *k);
	}
	Game::bodiesToDestroy.push_back(m);
}

void	Game::endingGame(void) {
	int		i;
	for (i = 0; i < Game::elementMap.size(); i++) {
		if (Game::elementMap[i] && Game::elementMap[i]->getAttribute("type") != "Hero") {
			theWorld.Remove(Game::elementMap[i]);
		}
	}
	Game::elementMap.clear();
	Game::getHUD()->setText("Press Enter to restart.", 500, 500);
	theWorld.ResumePhysics();
	theWorld.GetPhysicsWorld().DestroyBody(Game::currentGame->getHero()->GetBody());
	theWorld.Remove(Game::currentGame->getHero());
	Game::ended = false;
	Game::endGame = false;
	Game::secretDoor = nullptr;
	Game::bossDoor = nullptr;
	Game::dealer = nullptr;
	Game::currentGame->setShopkeeper(nullptr);
	Game::chest->reset();
	Game::deadWaiting = true;
	// Chest, Shop Items
}

//! Intern callback for destroying an element.
/**
 * Called after each tick() in order to destroy all elements set to destroy.
 * This function destroy each element in Game::bodiesToDestroy.
 * So, call this function outisde of this goal is useless.
 */
bool	Game::destroyAllBodies(void) {
	if (Game::ended == true) {
		return true ;
	}
	if (Game::endGame == true) {
		Game::ended = true;
		theWorld.PausePhysics();
		int i, j = 0;
		Game::getHUD()->setText("YOU ARE DEAD", 400, 400, Vector3(1, 0, 0), 1, "dead");
		Game::getHUD()->clearHUD();
		for (i = 0; i < Game::elementMap.size(); i++) {
			if (Game::elementMap[i] && Game::elementMap[i]->getAttribute("type") != "Hero") {
				if (j++ == 0)
					theSwitchboard.DeferredBroadcast(new Message("PauseGame"), 1);
				else
					Game::elementMap[i]->ChangeColorTo(Color(0, 0, 0, 1), 1);
				if (Game::elementMap[i]->getAttribute("physic") != "") {
					if (Game::elementMap[i]->GetBody() != 0)
						theWorld.GetPhysicsWorld().DestroyBody((Game::elementMap[i])->GetBody());
				}
			}
		}
		return true;
	} else {
		for (std::list<Elements*>::iterator it = Game::bodiesToDestroy.begin(); it != Game::bodiesToDestroy.end(); it++) {
			if ((*it)->getAttribute("physic") != "") {
				if ((*it)->GetBody()) {
					(*it)->GetBody()->SetActive(false);
					theWorld.GetPhysicsWorld().DestroyBody((*it)->GetBody());
				}
			}
			theWorld.Remove(*it);
			Game::delElement(*it);
		}
		Game::bodiesToDestroy.clear();
		for (std::list<Elements*>::iterator it = Game::bodiesToCreate.begin(); it != Game::bodiesToCreate.end(); it++) {
		  (*it)->InitPhysics();
		  theWorld.Add((*it));
		}
		Game::bodiesToCreate.clear();
	}
	return false;
}

//! Intern callback for staring running
/**
 * Make an element running
 * This function do not make the element 'running', just add them to the callback list.
 * @param c The Element who start running.
 */
void	Game::startRunning(Elements *c) {
	Game::runningCharac.push_back(c);
}

//! Intern callback for stoping running
/**
 * Make an element stop running.
 * Same as Game::startRunning(), but the object is remove from the list.
 * @param c The Elements who stop running.
 */
void	Game::stopRunning(Elements *c) {
	Game::runningCharac.remove(c);
}

//! World Callback for making elements running.
/**
 * The callback for each frame, making the call to the interns callbacks.
 * This function call the function _run for each member of the list Game::runningCharac
 * Keep this in mind, you won't probably call this function.
 * @sa Characters
 */
void	Game::makeItRun(void) {
	std::list<Elements *>::iterator	i;

	for (i = Game::runningCharac.begin(); i != Game::runningCharac.end(); i++) {
		(*i)->_run();
	}
}

//! Changing character
/**
 * Will be called early on selection screen
 */

void	Game::changeCharacter(std::string name) {
	Game::addToDestroyList(this->getHero());

	Hero *hero = new Hero(name);

	this->displayHero(*(hero));
	hero->init();
	this->setHero(hero);

	hero->setStartingValues();

}

//! Intern callback for display text
/**
 * Callback for each frame, to display the text in the HUDs.
 * This function is just call the function displayText() on each member of the HUDList.
 * Keep this in mind, you won't probably call this function.
 */
void	Game::showText(void) {
	std::list<HUDWindow *>::iterator i;

	for (i = Game::windows.begin(); i != Game::windows.end(); i++) {
		(*i)->displayText();
	}
}

//! Register an HUD to the Game list.
/**
 * Add a HUDWindow object to the callback list.
 * @param w The new HUD to add in Game.
 */
void	Game::addHUDWindow(HUDWindow *w) {
	if (std::find(Game::windows.begin(), Game::windows.end(), w) == Game::windows.end())
		Game::windows.push_back(w);
}

//! Unregister an HUD to the Game list.
/**
 * Remove a HUDWindow from the callback list.
 * @param w The HUDWindow to delete.
 */
void	Game::removeHUDWindow(HUDWindow *w) {
	std::list<HUDWindow *>::iterator 	it;

	it = std::find(Game::windows.begin(), Game::windows.end(), w);
	if (it != Game::windows.end())
		Game::windows.erase(it);
}

//! Get the first / main HUD
/**
 * Get the first HUD from the list. (Assuming there is an element)
 * This function is generaly use for getting the main HUD (life, xp, items) object,
 * assuming this object is the first added to the list.
 */
HUDWindow	*Game::getHUD(void) {
	if (Game::windows.size() == 0)
		return nullptr;
	return Game::windows.front();
}

//! Display the base HUD
/**
 * The init function for display the base HUD.
 * @todo This function is nasty, we need a recap here.
 */
void		Game::displayHUD(void) {
	HUDWindow *w = Game::getHUD();
	Characters*	hero = Game::getHero();

	Game	*g = this;
	w->setGame(g);
	w->showHud();
	w->setMaxHP(hero->getMaxHP());
	w->gold(hero->getGold());
	// Work
   //w->setText("Burp.", this->_hero, Vector3(0, 0, 0), 0, 1);
	/*w->removeText("Burp.");*/
}

void		Game::reloadingHUD(void) {
	if (Game::reloadHUD && !Game::isInMenu) {
		theWorld.Render();
		Game::getHUD()->clearHUD();
		Game::getHUD()->showBackgrounds();
		Game::getHUD()->character();
		Game::getHUD()->spells();
		Game::getHUD()->bag();
		Game::getHUD()->minimap();
		Game::getHUD()->life(this->getHero()->getHP());
		Game::getHUD()->bag();
		Game::getHUD()->items(this->getHero()->getWeapon());
		Game::getHUD()->items(this->getHero()->getArmor());
		Game::getHUD()->items(this->getHero()->getRing());
		Game::getHUD()->consumable(this->getHero()->getInventory()->getItems());
		Game::getHUD()->gold(this->getHero()->getGold());
		Game::reloadHUD = 0;
	}
}

/* SETTERS */
void		Game::setHero(Characters * h) { this->_hero = h; };
Characters*	Game::getHero(void) { return this->_hero; };
Shopkeeper*	Game::getShopkeeper(void) { return this->_shopkeeper; };
void		Game::setShopkeeper(Shopkeeper *s) { this->_shopkeeper = s; };
std::map<std::string, Json::Value>		Game::getSave(void) { return this->_save; };

// Set for the statics
int Game::currentIds = 0;
std::map<int, Elements *>	Game::elementMap = {};
std::list<Elements *>		Game::runningCharac;
std::list<Elements *>		Game::bodiesToDestroy;
std::list<Elements *>		Game::bodiesToCreate;
std::list<HUDWindow *>		Game::windows;
ArmorList*					Game::aList;
WeaponList*					Game::wList;
RingList*					Game::rList;
EnemyList*					Game::eList;
Hitbox*						Game::hList;
DrugList*					Game::dList;
bool						Game::endGame = false;
bool						Game::ended = false;
int							Game::maxX = 0;
int							Game::maxY = 0;
int							Game::minX = 0;
int							Game::minY = 0;
int							Game::currentX = 0;
int							Game::currentY = 0;
Game*						Game::currentGame = 0;
int							Game::started = 0;
int							Game::isInMenu = 0;
int							Game::cameraTick = 0;
int							Game::isWaitingForBind = 0;
int							Game::reloadHUD = 0;
int							Game::isPaused = 0;
int							Game::asToStart = 0;
MenuCharacter				*Game::menuCharacter = nullptr;
Vector2						Game::spawnShop = Vector2();
Vector2						Game::spawnBossDoor = Vector2();
Vector2						Game::spawnSecretDoor = Vector2();
Vector2						Game::spawnSecretReturnDoor = Vector2();
Vector2						Game::spawnDealer = Vector2();
Dealer						*Game::dealer = nullptr;
Door						*Game::bossDoor = nullptr;
Door						*Game::secretDoor = nullptr;
Door						*Game::secretReturnDoor = nullptr;
Vector2						Game::spawnChest = Vector2();
Chest						*Game::chest = nullptr;
bool						Game::toggleMenu = true;
bool						Game::stopPattern = false;
bool						Game::deadWaiting = false;
