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
 * File: Game.hpp
 * Creation: 2015-02-13 07:20
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

#ifndef __Game__
# define __Game__

class	Map;
class	Maps;
class Characters;
class MenuCharacter;

# ifndef __Map__
# include "Maps.hpp"
# endif
# ifndef __Elements__
#  include "main.hpp"
#  include "Elements.hpp"
# endif
# include "GameContactListener.hpp"
# include "ContactFilter.hpp"
# include "Characters.hpp"
# include "WeaponList.hpp"
# include "Hitbox.hpp"
# include "HUDWindow.hpp"
# include "EnemyList.hpp"
# include "ArmorList.hpp"
# include "RingList.hpp"
# include "LevelGenerator.hpp"
# include "Tooltip.hpp"
# include "Menu.hpp"
# include "InGameMenu.hpp"
# include "Shop.hpp"
# include "DrugList.hpp"
# include "ControllerInputManager.hpp"

class ArmorList;
class EnemyList;
class RingList;
class WeaponList;
class DrugList;
class Shopkeeper;
class Hitbox;
class HUDWindow;
class Menu;
class Dealer;
class Chest;
class Door;
class Boss;

class Game {

	public:
		Game();
		Game(unsigned int width, unsigned int height);
		~Game();

		void	grid(void);
		void	start(void);
		void	readMaps(void);
		void	displayHero(Elements & Hero);
		void	displayEnemy(Elements & Enemy);
		void	displayObject(Elements & Object);
		void	showMap(void);
		Map		getCurrentMap(void);
		void	changeCharacter(std::string);
		void	displayHUD(void);
		void	setHero(Characters *h);
		void	moveCamera(void);
		void	simulateHeroItemContact(void);
		void	reloadingHUD(void);
		Characters	*getHero(void);
		Shopkeeper	*getShopkeeper(void);
		void		setShopkeeper(Shopkeeper *s);
		void	menuInGame(void);
		void	endingGame(void);
		std::map<std::string, Json::Value>	getSave(void);

		static bool	endGame;
		static bool	ended;
		static bool	destroyAllBodies(void);
		static void checkHeroPosition(void);
		static void	addToDestroyList(Elements *m);


		static int	getNextId(void);
		static void	addElement(Elements & elem);
		static void	delElement(Elements *elem);
		static void	listElement(void);
		static void	callCallbacks(int a, int b);
		static void	startRunning(Elements *c);
		static void	stopRunning(Elements *c);
		static void	makeItRun(void);
		static void	showText(void);
		static void	addHUDWindow(HUDWindow *);
		static void	removeHUDWindow(HUDWindow *);
		static void	addBoss(std::string n, int x, int y);
		static HUDWindow*	getHUD(void);
		static int			lol;
		static Game*		currentGame;

		Maps		*maps;
		Menu		*menu;
		std::vector<Room*>				*gameMap;
		Tooltip 						*tooltip;
		static int						currentIds;
		static std::map<int, Elements *>	elementMap;
		static std::list<Elements *>	bodiesToDestroy;
		static std::list<Elements *>	bodiesToCreate;
		static std::list<Elements *>	runningCharac;
		static std::list<HUDWindow *>	windows;
		static WeaponList*				wList;
		static RingList*				rList;
		static ArmorList*				aList;
		static EnemyList*				eList;
		static Hitbox*					hList;
		static DrugList*				dList;
		static int						maxX;
		static int						maxY;
		static int						minX;
		static int						currentX;
		static int						currentY;
		static int						minY;
		static int						started;
		static int						isInMenu;
		static int						cameraTick;
		static int						isWaitingForBind;
		static int						reloadHUD;
		static int						asToStart;
		static int						isPaused;
		static MenuCharacter			*menuCharacter;
		static Vector2					spawnDealer;
		static Vector2					spawnSecretDoor;
		static Vector2					spawnBossDoor;
		static Vector2					spawnSecretReturnDoor;
		static Vector2					spawnShop;
		static Vector2					spawnChest;
		static Dealer					*dealer;
		static Chest					*chest;
		static Door						*bossDoor;
		static Door						*secretDoor;
		static Door						*secretReturnDoor;
		static bool						toggleMenu;
		static bool						stopPattern;
		static bool						deadWaiting;
		static int						World;
		static Boss						*boss;
		static void						newBoss(std::string n, int x, int y);

	private:
		float				beginXHero;
		float				beginYHero;
		std::vector<std::vector<int> >	_tmpMap;
		Characters	*_hero;
		Shopkeeper	*_shopkeeper;
		ControllerInputManager		*_controller;
		std::map<std::string, Json::Value>	_save;
};

# include "Dealer.hpp"
# include "Chest.hpp"
# include "Door.hpp"

#endif
