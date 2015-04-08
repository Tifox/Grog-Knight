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


class WeaponList;
class Hitbox;
class HUDWindow;

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
		void	displayHUD(void);
		void	setHero(Characters &h);
		Characters	&getHero(void);

		static bool	endGame;
		static void	destroyAllBodies(void);
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
		static HUDWindow*	getHUD(void);
		static int			lol;

		Maps		*maps;
		static int	currentIds;
		static std::map<int, Elements *>	elementMap;
		static std::list<Elements *>	bodiesToDestroy;
		static std::list<Elements *>	bodiesToCreate;
		static std::list<Elements *>	runningCharac;
		static std::list<HUDWindow *>	windows;
		static WeaponList*				wList;
		static Hitbox*					hList;

	private:
		float		beginXHero;
		float		beginYHero;
		Characters	&_hero;
};

#endif
