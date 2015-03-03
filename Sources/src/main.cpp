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
 * File: main.cpp
 * Creation: 2015-02-13 10:30
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "../inc/Game.hpp"
# include "../inc/Hero.hpp"
# include "../inc/Enemy.hpp"
 # include "../inc/Object.hpp"
# include "../inc/WeaponList.hpp"


class MouseDebugger: public MouseListener {
	public:
		MouseDebugger(void) {
		};
		~MouseDebugger(void) {
		};
		void MouseDownEvent(Vec2i sc, MouseButtonInput button) {
			Vector2 w;
			w = MathUtil::ScreenToWorld(sc.X, sc.Y);
			std::cout << w.X << ":" << w.Y << std::endl;
		};
};

int		main(int ac, char **av) {
	Game	*game = new Game();

	game->readMaps();
	game->initMap();
	MouseDebugger l;
	theWorld.SetBackgroundColor(*(new Color(0.51f, 0.90f, 1)));
	Hero		*hero = new Hero();
	Enemy		*enemy = new Enemy();
	Object		*object = new Object();
	WeaponList	*wList = new WeaponList();

	wList->statWeapon("Sword");
	wList->statWeapon("Bow");
	std::cout << wList->equipWeapon("Sword") << std::endl;

	theCamera.LockTo(hero);
	game->displayHero(*(hero));
	game->displayEnemy(*(enemy));
	game->displayObject(*(object));
	hero->init();
	enemy->init();
	object->init();

	//theWorld.SetSideBlockers(true, 0.7f);

	game->start();
	return 0;
}
