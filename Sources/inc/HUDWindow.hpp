
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
class	Game;

# include <iostream>

class	HUDWindow : public HUDActor {

	class	Text {
		public:
			Text() {};
			~Text() {};
			std::string		str;
			int				x;
			int				y;
			int				colorR;
			int				colorG;
			int				colorB;
			int				colorA;
	};

	public:
		HUDWindow(void);
		~HUDWindow(void);

		void	setText(std::string str, int x, int y);
		void	setText(std::string str, int x, int y, Vector3 color, int alpha);
		void	removeText(std::string str);
		void	displayText(void);
		void	addImage(std::string p, int x, int y);
		void	addImage(std::string path, int x, int y, float size);
		void	life(int l);
		void	mana(int mana);
		void	gold(int g);
		void	setGame(Game *);

	private:
		Game		*_g;
		std::list<HUDActor *>	_hearts;
		std::list<HUDActor *>	_mana;
		std::list<HUDWindow::Text *>	_text;
};

#endif
