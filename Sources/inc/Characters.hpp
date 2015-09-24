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
# include "SpecialMoves.hpp"
# include "SpecialAttack.hpp"
# include "Weapon.hpp"
# include "Armor.hpp"
# include "Ring.hpp"
# include "Log.hpp"
# include "HUDTargeting.hpp"

class Inventory;
class Weapon;
class Armor;
class Ring;
class SpecialMoves;
class SpecialAttack;
class HUDTargeting;

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

typedef struct s_buff {
		std::string cur;
  		int 		bonusDmg;
  		int 		bonusSpeed;
  		int 		drugSpeed;
		int 		dmgReduc;
		int 		critBuff;
}				t_buff;

class Characters : public Elements {

	public:
		friend	class	Game;
		friend	class	Pattern;
		friend	class	PassivePattern;
		friend	class	SpecialMoves;
		friend  class	SpecialAttack;
		friend	class	HUDWindow;

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
		virtual void	trigger(std::string name, int status) {};
		Characters::Orientation			getOrientation(void);
		std::string						getLastAction(void);
		Characters::Orientation			getLatOrientation(void);
		int								getGold(void);
		void							setGold(int);
		void							setDrug(std::string name);
		int								getHP(void);
		void							setHP(int hp);
		int								getMana(void);
		void							setMana(int mana);
		int								getMaxMana(void);
		int								getMaxHP(void);
		int								getLevel(void);
		void							setLevel(int);
		void							setInvincibility(bool invincibility);
		Weapon							*getWeapon(void);
		Armor							*getArmor(void);
		Ring							*getRing(void);
		bool							getCharging(void);
		int								getMaxInventory(void);
		Inventory						*getInventory(void);
		void							destroyTarget(void);
		Actor							*getGhost(void);

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
		t_buff						buff;
		int							inSpecialMap;
		Elements*					hookedTo;

	//Moved in order to get loot infos outside of class
		Json::Value		_getAttr(std::string category, std::string key);


	protected:
		std::string		_name;
		std::string		_lastAction;
		std::string 	_talk;
		std::string 	_drug;
		int				_id;
		int				_size;
		int				_maxSpeed;
		int				_isJump;
		int				_isRunning;
		int				_isAttacking;
		int				_hp;
		int				_gold;
		int				_isChoosingItem;
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
		int				_speAttReady;
		bool			_canAttack;
		bool			_isWhirlwinding;
		bool			_isRapidFiring;
		bool			_isCharging;
		bool			_isStomping;
		bool			_isFlying;
		bool			_flyTrigger;
		bool			_isDashing;
		int				_totemPlaced;
		int				_totemDeletionSent;
		int				_hasDashed;
		int				_level;
		std::string		_currentTrigger;
		bool			_speMoveIsSet;
		bool			_isDisengaging;
		bool			_isShockWaving;
		bool			_isTouchingChest;
		bool			_isTouchingSecretDoor;
		bool			_isTouchingBossDoor;
		bool			_isTouchingDealer;
		std::string		_speMove;
		std::string		_speAtt;
		SpecialMoves*	_eqMove;
		SpecialAttack*	_eqAtt;
		Weapon*			_weapon;
		Armor*			_armor;
		Ring*			_ring;
		Elements*		_item;
		std::string		_shopItem;
  		int				_shopItemNumber;
		int				_shopItemPrice;
		Elements*		_totem;
		Inventory*		_inventory;
		HUDTargeting*	_target;
		Actor			*_blast;
		Actor			*_ghost;
		Characters::Orientation				_orientation;
		Characters::Orientation				_latOrientation;
		std::list<Elements*>				_grounds;
		std::list<Elements*>				_enemiesTouched;
		std::list<Elements*> 				_wallsLeft;
		std::list<Elements*> 				_ceiling;
		std::list<Elements*> 				_wallsRight;
		std::list<std::string>				_subsc;
		bool			_execFlag;
		bool			_forwardFlag;
		bool			_backwardFlag;
		bool			_actionFlag;
		bool			_doFlyFlag;

		Json::Value		_getAttr(std::string key);
		void			_setCategory(std::string category);
		virtual void	characterLoop(void);
		virtual void	_tryFly(void);
		virtual void	_resetBroadcastFlags(void);
		virtual void	_forward(int status);
		virtual void	_backward(int status);
		virtual void	_up(int status);
		virtual void	_down(int status);
		virtual void	_jump(int status);
		virtual void	_attack(int status);
		virtual void	_pickupItem(int status);
		virtual void 	_executeAction(int status);
		virtual void	_run(void);
		virtual void	_specialMove(int status);
		virtual void	_specialAttack(int status);
		virtual void	_callTrigger(std::string name, int status);
		void			_destroyEnemy(void);
		Elements*		getItem(void);

	private:
		std::map<std::string, std::map<std::string, Json::Value> >	_attr;
		std::map<std::string, int>	_triggers;
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
# include "MenuCharacter.hpp"

#endif
