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
 * Creation: 2015-02-13 07:20
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

#include "../inc/Game.hpp"

/**
 * Basic constructor, set the window to default value
 * (http://docs.angel2d.com/class_world.html#ae5d7e8d20d3e6fc93785ab2014ac0c13)
 */
Game::Game(void) {
	#ifdef __APPLE__
		theWorld.Initialize(1920, 1080, NAME);
	#else
		theWorld.Initialize(1600, 1200, NAME, false, true);
	#endif
	theWorld.SetupPhysics();
	GameContactListener *gListen = new GameContactListener();
	ContactFilter *filter = new ContactFilter();
	theWorld.GetPhysicsWorld().SetContactFilter(filter);
	theWorld.GetPhysicsWorld().SetContactListener(gListen);
	this->maps = new Maps("Maps/");
	return ;
}

/**
 * Constructor with custom window width & height
 * @param: width (unsigned int)
 * @param: heigh (unsigned int)
 */
Game::Game(unsigned int width, unsigned int height) {
	theWorld.Initialize(width, height, NAME);
	theWorld.SetupPhysics();
	GameContactListener *gListen = new GameContactListener();
	ContactFilter *filter = new ContactFilter();
	theWorld.GetPhysicsWorld().SetContactFilter(filter);
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
	theWorld.StartGame();
}

/**
 * Read the maps
 */
void	Game::readMaps(void) {
	this->maps->readMaps();
}

/**
 * Display the first map
 */
void	Game::showMap(void) {
	this->maps->firstOne();
}

/**
 * Display the Hero
 * @param: Hero (Elements &)
 */
void	Game::displayHero(Elements & Hero) {
	Hero.setXStart(10);
	Hero.setYStart(-10);
	Hero.addAttribute("hero", "1");
	Hero.display();
}

/**
 * Display the Enemy
 * @param: Enemy (Elements &)
 */
void	Game::displayEnemy(Elements & Enemy) {
	Enemy.setXStart(5);
	Enemy.setYStart(-5);
	Enemy.addAttribute("enemy", "1");
	Enemy.display();
}

/**
 * Display the Object
 * @param: Object (Elements &)
 */
void	Game::displayObject(Elements & Object) {
	Object.setXStart(6);
	Object.setYStart(6);
	Object.addAttribute("Object", "1");
	Object.display();
}

/**
 * Get the current id, for the intern elements map
 */
int		Game::getNextId(void) {
	return Game::currentIds;
}

/**
 * Add an element to the intern map
 * @param: elem (Elements &)
 */
void	Game::addElement(Elements & elem) {
	Game::elementMap[Game::currentIds] = &elem;
	Game::currentIds += 1;
}

/**
 * Deletes an element from the intern map
 * @param: elem (Elements &)
 */
void	Game::delElement(Elements* elem) {
	for (int i = 0; Game::elementMap[i]; i++) {
		if (Game::elementMap[i] == elem)
			Game::elementMap.erase(i);
	}
}

/**
 * Call the collision callbacks on two objects
 * @param: a (int)
 * @param: b (int)
 */
void	Game::callCallbacks(int a, int b) {
	if (a != -1 && b != -1) {
		Game::elementMap[a]->callback(Game::elementMap[b]);
		Game::elementMap[b]->callback(Game::elementMap[a]);
	}
}

/**
 * List all Elements
 */
void	Game::listElement(void) {
	int		i;

	for (i = 0; i < Game::currentIds; i++) {
		std::cout << Game::elementMap[i] << std::endl;
	}
}

/**
 * Call to add an element to the destroy list after the tick()
 * @param: m (Elements*)
 */

void	Game::addToDestroyList(Elements *m) {
	for (std::list<Elements*>::iterator it = Game::bodiesToDestroy.begin(); it != Game::bodiesToDestroy.end(); it++) {
		if ((*it) == m)
			return;
	}
	Game::bodiesToDestroy.push_back(m);
}

/**
 * Called after each tick() in order to destroy all elements set to destroy
 */

void	Game::destroyAllBodies(void) {
	for (std::list<Elements*>::iterator it = Game::bodiesToDestroy.begin(); it != Game::bodiesToDestroy.end(); it++) {
		theWorld.GetPhysicsWorld().DestroyBody((*it)->GetBody());
		theWorld.Remove(*it);
		Game::delElement(*it);
	}
	Game::bodiesToDestroy.clear();
}

void	Game::startRunning(Elements *c) {
	Game::runningCharac.push_back(c);
}

void	Game::stopRunning(Elements *c) {
	Game::runningCharac.remove(c);
}

void	Game::makeItRun(void) {
	std::list<Elements *>::iterator	i;

	for (i = Game::runningCharac.begin(); i != Game::runningCharac.end(); i++) {
		(*i)->_run();
	}
}

void	Game::showText(void) {
	std::list<HUDWindow *>::iterator i;

	for (i = Game::windows.begin(); i != Game::windows.end(); i++) {
		(*i)->displayText();
	}
}

void	Game::addHUDWindow(HUDWindow *w) {
	Game::windows.push_back(w);
}

HUDWindow	*Game::getHUD(void) {
	return Game::windows.front();
}

void		Game::displayHUD(void) {
	HUDWindow *w = new HUDWindow();
	w->SetPosition(theCamera.GetWindowWidth() / 2, 50);
	w->SetSize(theCamera.GetWindowWidth(), 100.0f);
	w->SetColor(1.13f, 0.94f, 0.86f, 1);
	w->SetDrawShape(ADS_Square);
	w->SetLayer(-1);
	w->addImage("Resources/Images/bourse.png", 100, 50);
	w->addImage("Resources/Images/HUD/heart.png", 900, 50);
	theWorld.Add(w);
	w->setText(std::string("Laul"), 500.0f, 100.0f);
}

// Set for the statics
int Game::currentIds = 0;
std::map<int, Elements *>	Game::elementMap = {};
std::list<Elements *>		Game::runningCharac;
std::list<Elements *>		Game::bodiesToDestroy;
std::list<HUDWindow *>		Game::windows;
WeaponList*					Game::wList;
