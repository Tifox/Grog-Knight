
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
# include "json/json.h"
# include <list>
# include "../../Angel/Angel.h"
# ifndef __Elements__
# include "Elements.hpp"
#endif

class Characters : public Elements {

	public:
		friend	class	Game;

		Characters(void);
		Characters(std::string name);
		~Characters();

		virtual void	ReceiveMessage(Message *m);
		virtual void	AnimCallback(String s);
		virtual void	BeginContact(Elements *elem, b2Contact *contact);
		virtual void	EndContact(Elements *elem, b2Contact *contact);
		// Virtual function, overwritten in childs
		virtual void	actionCallback(std::string name, int status) {};

	protected:
		std::string		_name;
		int				_id;
		int				_size;
		int				_maxSpeed;
		int				_isJump;
		int				_isRunning;
		Weapon*			_weapon;
		std::list<Elements*>				_grounds;
		std::list<Elements*> 				_walls;

		Json::Value		_getAttr(std::string category, std::string key);
		Json::Value		_getAttr(std::string key);
		void			_setCategory(std::string category);
		virtual void	_forward(int status);
		virtual void	_backward(int status);
		virtual void	_jump(int status);
		virtual void	_attack(int status);
		virtual void	_run(void);

	private:
		std::map<std::string, std::map<std::string, Json::Value> >	_attr;
		std::string		_category;

		void	_readFile(std::string name);
		void	_parseJson(std::string file);
};

# include "Game.hpp"

#endif
