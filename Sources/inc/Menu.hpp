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
 * File: Menu.hpp
 * Creation: 2015-04-07 21:11
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "Game.hpp"

class		Menu : public Actor {
	public:
		Menu(void);
		~Menu(void);
		void	showMenu(Game *game);
		void	ReceiveMessage(Message *m);
		void	listMenu(void);
        void    removeBaseMenu(void);
        void    settings(void);
        void    removeSettings(void);

	private:
		Game		*_game;
		HUDWindow	*_window;
		std::list<std::string>	_menuChoices;
		std::string				_currentChoice;
        int                 _inMenu;
        std::map<std::string, std::map<std::string, int> >  _settingsValues;
};
