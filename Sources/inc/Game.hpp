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

# include "Maps.hpp"
# ifndef __Elements__
#  include "main.hpp"
#  include "Elements.hpp"
# endif
# include "GameContactListener.hpp"

class Game {

	public:
		Game();
		Game(unsigned int width, unsigned int height);
		~Game();

		void	grid(void);
		void	start(void);
		void	readMaps(void);
		void	initMap(void);
		void	displayMap(t_map map);
		void	displayHero(Elements & Hero);

		static int	getNextId(void);
		static void	addElement(Elements & elem);
		static void	listElement(void);
		static void	callCallbacks(int a, int b);

		Maps		*maps;
		static int	currentIds;
		static std::map<int, Elements *>	elementMap;

	private:
		float		beginXHero;
		float		beginYHero;
};


#endif
