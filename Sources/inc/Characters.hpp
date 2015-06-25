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

# include "Inventory.hpp"
# include "Weapon.hpp"
# include "Armor.hpp"
# include "Ring.hpp"
# include "Log.hpp"

class Inventory;
class Weapon;
class Armor;
class Ring;

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
		friend	class	Pattern;
		friend	class	PassivePattern;

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
		int								getGold(void);
		int								getHP(void);
		void							setHP(int hp);
		int								getMana(void);
		void							setMana(int mana);
		int								getMaxMana(void);
		int								getMaxHP(void);
		int								getLevel(void);
		Weapon							*getWeapon(void);
		Armor							*getArmor(void);
		Ring							*getRing(void);
		bool							getCharging(void);
		int								getMaxInventory();

		int				forwardLimit;
		int				backwardLimit;

		// Virtual function, overwritten in childs
		virtual void	actionCallback(std::string name, int status) {};
		virtual void	equipWeapon(Weapon* weapon);
		virtual void	unequipWeapon(void);
		virtual void	equipArmor(Armor* armor);
		virtual void	unequipArmor(void);
		virtual void	equipRing(Ring* ring);
		virtual void	unequipRing(void);

		void			changeCanMove(void);
		std::list<std::string>		getSubscribes(void);
		void						unsubscribeFromAll(void);
		void						subscribeToAll(void);

	//Moved in order to get loot infos outside of class
		Json::Value		_getAttr(std::string category, std::string key);


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
		int				_gold;
		int				_maxHp;
		int 			_mana;
		int 			_maxMana;
		bool			_canMove;
		bool			_canJump;
		bool			_invincibility;
		bool			_attackPressed;
		int				_isLoadingAttack;
		bool			_fullChargedAttack;
		int				_speMoveReady;
		bool			_canAttack;
		bool			_isCharging;
		bool			_isStomping;
		int				_hasDashed;
		int				_level;
		std::string		_speMove;
		Weapon*			_weapon;
		Armor*			_armor;
		Ring*			_ring;
		Elements*		_item;
		Inventory*		_inventory;
		Characters::Orientation				_orientation;
		Characters::Orientation				_latOrientation;
		std::list<Elements*>				_grounds;
		std::list<Elements*>				_enemiesTouched;
		std::list<Elements*> 				_wallsLeft;
		std::list<Elements*> 				_ceiling;
		std::list<Elements*> 				_wallsRight;
		std::list<std::string>				_subsc;

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
		virtual void	_specialMove(void);
		virtual void	_dash(void);
		virtual void	_charge(void);
		virtual void	_stomp(void);
		virtual void	_blink(void);
		void			_destroyEnemy(void);
		Elements*		getItem(void);

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
# include "Enemy.hpp"

#endif
