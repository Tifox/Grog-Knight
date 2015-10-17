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

# include <ctime>
# include "Game.hpp"
# include "Hero.hpp"
# include "Dealer.hpp"
# include "Shopkeeper.hpp"
# include "Enemy.hpp"
# include "Object.hpp"
# include "WeaponList.hpp"
# include "Equipment.hpp"
# include "Consumable.hpp"
# include "Hitbox.hpp"
# include "HUDWindow.hpp"
# include "LevelGenerator.hpp"
# include "Menu.hpp"

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
	if (ac > 1 && (std::string(av[1]) == "--debug")) {
		Log::setLog(1);
	}

	Game	*game = new Game();
	Game::hList = new Hitbox();
	Menu	*menu = new Menu();

	srand(time(NULL));
	game->readMaps();
	MouseDebugger l;
	Game::currentGame = game;
	if (ac > 1 && (std::string(av[1]) == "--no-menu")) {
		game->start();
		theWorld.StartGame();
	}
	else {
		game->menu = menu;
		Game::isInMenu = 1;
		menu->showMenu(game);
	}
	return 0;
}
