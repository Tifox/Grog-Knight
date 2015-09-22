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
# include "Projectile.hpp"
# ifdef __APPLE__
#  include "../../Tools/jsoncpp/include/json/json.h"
# else
#  include "json/json.h"
# endif

class Characters;

class Weapon: public Elements {
public:
	Weapon(std::string name);
	Weapon(Weapon* weapon);
	Weapon(Weapon* weapon, Characters* c);
	Weapon(Weapon* weapon, Characters* c, int i);
	~Weapon(void);

	void			setActive(float f);

	void			attack(Characters *c);
	void			BeginContact(Elements *elem, b2Contact *contact);
	void			EndContact(Elements *elem, b2Contact *contact);
	void			ReceiveMessage(Message *m);

	std::string		getName(void);
	std::string		getType(void);
	std::string		getFlavor(void);
	std::string		getAttack(void);
	std::string		getSprite(void);
	float			getActive(void);
	int				getCritRate(void);
	int				getSize(void);
	int				getLootLevel(void);
	int				getDamage(void);
	int				getPushback(void);
	float			getRecovery(void);

private:
	std::map<std::string, std::map<std::string, Json::Value> >	_attr;

	std::string		_name;
	std::string		_flavor;
	std::string		_sprite;
	std::string		_attack;
	std::string		_type;
	int				_lootLevel;
	float			_recovery;
	float			_active;
	int				_size;
	int				_damage;
	int				_pushback;
	int				_critRate;
	void			_initDirection(Weapon* w, Characters* c);
	void			_readFile(std::string name);
	void			_readFileFromFilename(std::string name);
	void			_parseJson(std::string file);

protected:
	Json::Value     _getAttr(std::string category, std::string key);
};

#endif
