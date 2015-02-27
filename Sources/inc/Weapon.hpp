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
 * File: Weapon.hpp
 * Creation: 2015-02-18 14:00
 * Vincent Rey <vrey@student.42.fr>
 */

#ifndef __Weapon__
# define __Weapon__

# include "Log.hpp"
# include "Elements.hpp"
# include "json/json.h"
/*
** Default constructor, using the element that called the attack
** @param: Elements *
*/

class Weapon: public Elements {
public:
	Weapon(std::string name);
	~Weapon(void);

	void			attack(int, int, int, int, b2Vec2);
	void			BeginContact(Elements *elem, b2Contact *contact);
	void			EndContact(Elements *elem, b2Contact *contact);
	void			ReceiveMessage(Message *m);

private:
	std::map<std::string, std::map<std::string, Json::Value> >	_attr;

	std::string		_name;
	std::string		_flavor;
	int				_canAttack;
	int				_recovery;
	int				_active;
	int				_size;
	void			_readFile(std::string name);
	void			_parseJson(std::string file);
//	WeaponArea*		_attackBox;

protected:
	Json::Value     _getAttr(std::string category, std::string key);
};

#endif
