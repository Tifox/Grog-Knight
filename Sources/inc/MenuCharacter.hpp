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
 * File: MenuCharacter.hpp
 * @date 2015-07-23 16:44
 * @author Louis Solofrizzo <louis@ne02ptzero.me>
 */

#ifndef _MENU_CHARACTER_
# define _MENU_CHARACTER_

# include "Characters.hpp"
# include "ArmorList.hpp"
# include "WeaponList.hpp"
# include "RingList.hpp"

class		MenuCharacter : public Characters {

	class	itemInfo {
		public:
			Actor					*background;
			Actor					*sprite;
			std::list<std::string>	textInfo;
	};

	public:
		MenuCharacter(void);
		~MenuCharacter(void);
		void		init(void);
		void		actionCallback(std::string name, int status);
		virtual void	BeginContact(Elements *elem, b2Contact *contact);
		virtual void	EndContact(Elements *elem, b2Contact *contact);
		virtual void	AnimCallback(String s);
		virtual void	ReceiveMessage(Message *m);
		virtual void	trigger(std::string name, int status);
		std::string		getHeroType(void);
		Weapon			*getWeapon(void);
		Ring			*getRing(void);
		Armor			*getArmor(void);
		std::vector<std::string>	getSkills(void);
		int							getLevel(void);

	protected:
		virtual void	_forward(int status);
		virtual void	_backward(int status);
		virtual void	_up(int status);
		virtual void	_down(int status);
		void			_showTextInfo(std::string text, std::string switchboard = "enterPressed");
		void			_makeItChoice(void);
		void			_openCloset(void);
		void			_closeCloset(void);
		void			_equipmentChoose(void);
		void			_closetBackChoiceUpdate(void);
		void			_updateSelection(void);
		void			_showSelectInfo(Elements *obj);
		void			_flavorInfo(std::string n);
		void			_cleanCloset(void);
		void			_kitchen(void);
		void			_changeKitchen(void);
		void			_hideKitchen(int reload = 0);
		void			_getSkills(void);
		void			_makeSkillChoice(void);
		void			_levels(void);

	private:
		HUDActor			*_image;
		std::list<Elements *>	_choices;
		std::list<Elements *>	_backChoices;
		Elements				*_choicePointer;
		Actor				*_target;
		Actor				*_target2;
		int					_isBlock;
		std::string			_character;
		Elements			*_closet;
		std::list<Elements *>	_backCloset;
		Elements				*_closetChoice;
		std::list<Ring *>				_ringList;
		std::list<Weapon *>				_weaponList;
		std::list<Armor *>				_armorList;
		std::list<Elements *>			_equipSelectionBack;
		std::list<Elements *>			_kitchenSkills;
		int								_chooseEquipment;
		std::map<std::string, Elements *> _equipSelection;
		MenuCharacter::itemInfo				*_currentItemInfo;
		Json::Value							_skills;
		Json::Value							_skillTree;
		std::vector<int>					_skillsLvl;
		int									_characLvl;
		std::vector<std::list<Elements *> >	_skillsChoices;
		std::vector<std::string>			_finalSkillChoices;
		std::vector<Elements *>			_finalSkillTargets;
		Elements						*_descriptionBackground;
		Elements						*_iconBackground;
		Elements						*_icon;
		std::list<std::string>			_description;
		Elements						*_levelsBackground;
		Elements						*_lvlUp;
		std::string						_textInfo;
};
#endif
