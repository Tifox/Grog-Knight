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

# include "Game.hpp"
# include "Hero.hpp"
# include "Enemy.hpp"
# include "Object.hpp"
# include "WeaponList.hpp"
# include "Equipment.hpp"
# include "Consumable.hpp"
# include "Hitbox.hpp"
# include "HUDWindow.hpp"
# include "LevelGenerator.hpp"

class MouseDebugger: public MouseListener {
	public:
		MouseDebugger(void) {
		};
		~MouseDebugger(void) {
		};
		void MouseDownEvent(Vec2i sc, MouseButtonInput button) {
			Vector2 w;
			w = MathUtil::ScreenToWorld(sc.X, sc.Y);
		};
};

int		main(int ac, char **av) {
	Game	*game = new Game();
	Game::hList = new Hitbox();

	game->readMaps();
	MouseDebugger l;
	theWorld.SetBackgroundColor(*(new Color(0, 0, 0)));

	Game::wList = new WeaponList();

	game->showMap();

	Equipment		*equip = new Equipment();
	Consumable		*lol = new Consumable();
	Hero			*hero = new Hero();
	Enemy			*enemy = new Enemy("Enemy");
	Enemy			*enemy2 = new Enemy("Enemy2");



	//===== I temp map generation test =====

	LevelGenerator *levelGenerator = new LevelGenerator(6, 6, 5, 60, 80);
	levelGenerator->execute();
	levelGenerator->print();

	//===== O temp map generation test =====

//	theCamera.LockTo(hero);
	theCamera.SetPosition(13, -7);
	game->displayHero(*(hero));
	hero->init();
	enemy->init();
	enemy2->init();
	game->setHero(*hero);

	hero->equipWeapon(Game::wList->getWeapon("Sword"));
	//theWorld.SetSideBlockers(true, 0.7f);
	game->displayHUD();

	game->start();

	std::cout << "allol" << std::endl;

	return 0;
}
