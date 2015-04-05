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
 * File: Characters.hpp
 * Creation: 2015-02-27 04:45
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

#ifndef __Characters__
# define __Characters__

# include "Weapon.hpp"
# include "Log.hpp"

class Weapon;

# ifdef __APPLE__
#  include "../../Tools/jsoncpp/include/json/json.h"
# else
#  include "json/json.h"
# endif

# include <list>
# include "../../Angel/Angel.h"
# ifndef __Elements__
#  include "Elements.hpp"
# endif

class Characters : public Elements {

	public:
		friend	class	Game;

		enum Orientation {
			UP,
			DOWN,
			LEFT,
			RIGHT
		};

		Characters(void);
		Characters(std::string name);
		~Characters();

		virtual void	ReceiveMessage(Message *m);
		virtual void	AnimCallback(String s);
		virtual void	BeginContact(Elements *elem, b2Contact *contact);
		virtual void	EndContact(Elements *elem, b2Contact *contact);
		Characters::Orientation			getOrientation(void);
		std::string						getLastAction(void);
		int								getHP(void);
		void							setHP(int h);
		Weapon							*getWeapon(void);
		// Virtual function, overwritten in childs
		virtual void	actionCallback(std::string name, int status) {};
		virtual void	equipWeapon(Weapon* weapon);
		void			changeCanMove(void);

	protected:
		std::string		_name;
		std::string		_lastAction;
		int				_id;
		int				_size;
		int				_maxSpeed;
		int				_isJump;
		int				_isRunning;
		int				_isAttacking;
		int				_hp;
		int				_maxHp;
		bool			_canMove;
		bool			_canJump;
		bool			_invincibility;
		Weapon*			_weapon;
		Elements*		_item;
		Characters::Orientation				_orientation;
		Characters::Orientation				_latOrientation;
		std::list<Elements*>				_grounds;
		std::list<Elements*> 				_wallsLeft;
		std::list<Elements*> 				_walls;
		std::list<Elements*> 				_wallsRight;

		Json::Value		_getAttr(std::string category, std::string key);
		Json::Value		_getAttr(std::string key);
		void			_setCategory(std::string category);
		virtual void	_forward(int status);
		virtual void	_backward(int status);
		virtual void	_up(int status);
		virtual void	_down(int status);
		virtual void	_jump(int status);
		virtual void	_attack(int status);
		virtual void	_pickupItem(int status);
		virtual void	_run(void);
		void			_destroyEnemy(void);

	private:
		std::map<std::string, std::map<std::string, Json::Value> >	_attr;
		std::string		_category;

		void	_heroDeath();
		void	_readFile(std::string name);
		void	_parseJson(std::string file);
};

# ifndef __Object__
#  include "Equipment.hpp"
#  include "Consumable.hpp"
# endif

# include "Game.hpp"

#endif
