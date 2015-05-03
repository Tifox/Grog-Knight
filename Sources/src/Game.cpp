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

#include "../inc/Game.hpp"

//! Basic constructor, set the window to default value
/**
 * Initialize the world window, init Physics.
 * (http://docs.angel2d.com/class_world.html#ae5d7e8d20d3e6fc93785ab2014ac0c13)
 * Load the maps with Maps::Maps
 * @sa Maps
 */
Game::Game(void) : _hero(*(new Characters())) {
	#ifdef __APPLE__
		theWorld.Initialize(1920, 1080, NAME, false, false);
	#else
		theWorld.Initialize(1024, 720, NAME, false, false);
	#endif
	theWorld.SetupPhysics(Vector2(0, -20));
	GameContactListener *gListen = new GameContactListener();
//	ContactFilter *filter = new ContactFilter();
//	theWorld.GetPhysicsWorld().SetContactFilter(filter);
	theWorld.GetPhysicsWorld().SetContactListener(gListen);
	this->maps = new Maps("Maps/");
	return ;
}

/**
 * Constructor with custom window width & height
 * @param width The width of the window
 * @param heigh The height of the window
 */
Game::Game(unsigned int width, unsigned int height) : _hero(*(new Characters())) {
	theWorld.Initialize(width, height, NAME);
	theWorld.SetupPhysics();
	GameContactListener *gListen = new GameContactListener();
//	ContactFilter *filter = new ContactFilter();
//	theWorld.GetPhysicsWorld().SetContactFilter(filter);
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
	this->showMap();

	Consumable		*lol = new Consumable();
	Hero			*hero = new Hero();
	Enemy			*enemy = new Enemy("Enemy");
	Enemy			*enemy2 = new Enemy("Enemy2");

	//===== I temp map generation test =====
	//LevelGenerator *levelGenerator = new LevelGenerator(6, 6, 5, 60, 80);
	//levelGenerator->execute();
	//levelGenerator->print();
	//===== O temp map generation test =====

	this->displayHero(*(hero));
	theCamera.LockTo(hero);
	hero->init();
	enemy->init();
	enemy2->init();
	hero->equipWeapon(Game::wList->getWeapon("Sword"));
	this->setHero(*hero);
	this->displayHUD();
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
	this->maps->displayLevel();
}

//! Display the Hero
/**
 * We add the hero to the world, with an init position (x, y)
 * @param Hero The Hero Object
 */
void	Game::displayHero(Elements & Hero) {
	Hero.setXStart(15);
	Hero.setYStart(-8);
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

//! Add an Element
/**
 * Add an element to the intern map, to the static elements map.
 * This map is usefull for cleaning an all level, or fading out at death, etc.
 * @param elem The Elements to add at the list
 */
void	Game::addElement(Elements & elem) {
	Game::elementMap[Game::currentIds] = &elem;
	Game::currentIds += 1;
}

//! Delete an element
/**
 * Delete an element from the intern map
 * @param elem The element to delete (The reference is important !)
 */
void	Game::delElement(Elements* elem) {
	for (int i = 0; Game::elementMap[i]; i++) {
		if (Game::elementMap[i] == elem)
			Game::elementMap.erase(i);
	}
	//delete elem;
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
		int i;
		Game::getHUD()->setText("YOU ARE DEAD", 400, 400, Vector3(1, 0, 0), 1, "dead");
		for (i = 0; i < Game::elementMap.size() - 2; i++) {
			if (Game::elementMap[i] && Game::elementMap[i]->getAttribute("type") != "Hero") {
				Game::elementMap[i]->ChangeColorTo(Color(0, 0, 0, 1), 1, "PauseGame");
				if (Game::elementMap[i]->getAttribute("physic") != "")
					theWorld.GetPhysicsWorld().DestroyBody((Game::elementMap[i])->GetBody());
				theWorld.Remove(Game::elementMap[i]);
			}
		}
		Game::elementMap.clear();
		return true;
	} else {
		for (std::list<Elements*>::iterator it = Game::bodiesToDestroy.begin(); it != Game::bodiesToDestroy.end(); it++) {
			theWorld.GetPhysicsWorld().DestroyBody((*it)->GetBody());
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
	return Game::windows.front();
}

//! Display the base HUD
/**
 * The init function for display the base HUD.
 * @todo This function is nasty, we need a recap here.
 */
void		Game::displayHUD(void) {
	HUDWindow *w = new HUDWindow();
	Characters	&lol = Game::getHero();
	w->SetPosition(theCamera.GetWindowWidth() / 2 - 100, 50);
	w->SetSize(theCamera.GetWindowWidth() - 200, 100.0f);
	w->SetSprite("Resources/Images/HUD/background_hud.png");
	w->SetDrawShape(ADS_Square);
	w->SetLayer(-1);
	w->addImage("Resources/Images/HUD/perso.png", 100, 50);
	theWorld.Add(w);
	Game	*g = this;
	w->setGame(g);
	w->life(125);
	w->mana(90);
	w->gold(0);
	w->armor();
	w->boots();
	w->consumable();
	w->minimap();
	Game::addHUDWindow(w);
}

/* SETTERS */
void		Game::setHero(Characters & h) { this->_hero = h; };
Characters	&Game::getHero(void) { return this->_hero; };

// Set for the statics
int Game::currentIds = 0;
std::map<int, Elements *>	Game::elementMap = {};
std::list<Elements *>		Game::runningCharac;
std::list<Elements *>		Game::bodiesToDestroy;
std::list<Elements *>		Game::bodiesToCreate;
std::list<HUDWindow *>		Game::windows;
WeaponList*					Game::wList;
Hitbox*						Game::hList;
bool						Game::endGame = false;
bool						Game::ended = false;
int							Game::maxX = 0;
int							Game::maxY = 0;
