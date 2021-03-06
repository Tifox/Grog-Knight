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
 * File: Hero.hpp
 * Creation: 2015-02-14 10:49
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

#ifndef __Hero__
# define __Hero__

# include "Weapon.hpp"
# include "Characters.hpp"

class Hero : public Characters {

	public:
		Hero(std::string);
		Hero(Hero *);
		~Hero();

		void	init();
		virtual void	BeginContact(Elements* m, b2Contact* contact);
		virtual void	EndContact(Elements* m, b2Contact* contact);
		virtual void	actionCallback(std::string name, int status);
		void			_takeDamage(Elements* m);
		void			setStartingValues(Hero *tmp = nullptr);

		Inventory	*getInventory(void);
		SpecialMoves *getEqMove(void);
		SpecialAttack *getEqAtt(void);
		void		setInventory(Inventory *i);
};
#endif
