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
	theWorld.Initialize(1920, 1080, NAME);
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
 * Display a map
 * @param: map (t_map)
 */
void	Game::displayMap(t_map map) {
	std::vector<std::vector<int> >::iterator	i;
	std::vector<int>::iterator			v;
	float								x = 0.0f, y = -10.0f;
	Elements							*block;
	Elements							&tmp = *(new Elements());

	for (i = map.map.begin(); i != map.map.end(); i++, x -= 1.0f) {
		for (y = -10.0f, v = i->begin(); v != i->end(); v++, y += 1.0f) {
			if ((*v) == 0)
				block = new Elements();
			else {
				tmp = (*(map.elements[(*v)]));
				block = new Elements(tmp);
			}
			block->setXStart(y);
			block->setYStart(x);
			block->display();
			if (block->getAttribute("attribute") == "start") {
				this->beginXHero = y;
				this->beginYHero = x;
			}
		}
	}
}

/**
 * Display the first map
 */
void	Game::initMap(void) {
	std::list<t_map> 	maps = this->maps->getFormattedMaps();
	std::list<t_map>::iterator	it;

	for (it = maps.begin(); it != maps.end(); it++) {
		if ((*it).id == 1) {
			this->displayMap(*it);
		}
		std::cout << (*it).id << std::endl;
	}
}

/**
 * Display the Hero
 * @param: Hero (Elements &)
 */
void	Game::displayHero(Elements & Hero) {
	Hero.setXStart(this->beginXHero);
	Hero.setYStart(this->beginYHero);
	Hero.addAttribute("hero", "1");
	Hero.display();
}

/**
 * Display the Enemy
 * @param: Enemy (Elements &)
 */
void	Game::displayEnemy(Elements & Enemy) {
	Enemy.setXStart(3);
	Enemy.setYStart(3);
	Enemy.addAttribute("enemy", "1");
	Enemy.display();
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

void	Game::destroyAllBodies(void) {
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

// Set for the statics
int Game::currentIds = 0;
std::map<int, Elements *> Game::elementMap = {};
std::list<Elements *> Game::runningCharac;
