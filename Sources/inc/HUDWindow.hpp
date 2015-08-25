
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
 * File: HUDWindow.hpp
 * Creation: 2015-03-14 05:35
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

#ifndef __HUDWindow__
# define __HUDWindow__

# include "../../Angel/Angel.h"
# include "Game.hpp"
# include "Map.hpp"
class	Game;
class   Ring;
class   Armor;

# include <iostream>

class	HUDWindow : public HUDActor {


	public:
	class	Text {
		public: 
			Text() {};
			~Text() {};
			std::string		str;
			std::string		font;
			int				x;
			int				y;
			int				colorR;
			int				colorG;
			int				colorB;
			float				colorA;
			Characters		*toFollow;
			int				isFading;
			int				isTalk;
	};

		HUDWindow(void);
		~HUDWindow(void);

		HUDWindow::Text	*setText(std::string str, int x, int y);
		HUDWindow::Text	*setText(std::string str, int x, int y, Vector3 color, int alpha);
		HUDWindow::Text	*setText(std::string str, int x, int y, Vector3 color, int alpha, std::string font);
		HUDWindow::Text	*setText(std::string str, Characters *toFollow, Vector3 color, int isFading, int isTalk, int isInMenu = 0);
		void	removeText(std::string str);
		void	removeText(HUDWindow::Text *t);
		void	displayText(void);
		HUDActor	*addImage(std::string p, int x, int y);
		HUDActor	*addImage(std::string path, int x, int y, float size);
		HUDActor	*addImage(std::string path, int x, int y, float size, int layer);
		HUDActor	*addImage(std::string path, int x, int y, Vector2 size, int layer);
		void	life(int l);
		void	mana(int mana);
		void	gold(int g);
		void	updateGold(int gold);
		void	items(Weapon *w);
		void	items(Armor *a);
		void	items(Ring *r);
		void	armor(void);
		void	boots(void);
		void	consumable(std::map<int, std::string> m);
		void	minimap(void);
		void	setGame(Game *);
		void	setMaxMana(int m);
		void	setMaxHP(int h);
		void	bag(void);
		void	initMinimapBackground(void);
		void	showHud(void);
		void	showBackgrounds(void);
		void	character(void);
		void	spells(void);
		void	clearHUD(void);
		void	bigMap(void);
		void	deleteBigMap(int n);
		void	updateBigMap(void);

		static	int		isToggled;

	private:
		Game		*_g;
		std::list<HUDActor *>	_hearts;
		std::list<HUDActor *>	_mana;
		std::list<HUDActor *>	_bag;
		std::list<HUDActor *>	_minimap;
		std::map<std::string, Elements*>		_dialog;
		std::list<HUDWindow::Text *>	_text;
		std::list<HUDActor *>	_allElems;
		HUDWindow::Text	*		_gold;
		int						_maxMana;
		int						_maxHP;
		std::list<HUDActor *>	_bigMapList;
		HUDActor				*_currentObjectMap;
		HUDActor				*_currentTotemMap;
		void					_drawDoor(Vector2 size, Vector2 position);
		int						_doNotDelete;
};

#endif
