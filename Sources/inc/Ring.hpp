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
 * File: Ring.hpp
 * Creation: 2015-05-07 17:44
 * Manon Budin <mbudin@student.42.fr>
 */

#ifndef __Ring__
# define __Ring__

# include "Log.hpp"
# include "Elements.hpp"
# include "Projectile.hpp"
# ifdef __APPLE__
#  include "../../Tools/jsoncpp/include/json/json.h"
# else
#  include "json/json.h"
# endif

class Characters;

class Ring: public Elements {
public:
	Ring(std::string name);
	Ring(Ring* Ring);
	~Ring(void);

	void			BeginContact(Elements *elem, b2Contact *contact);
	void			EndContact(Elements *elem, b2Contact *contact);
	void			ReceiveMessage(Message *m);

	Bonus			*getBonus(void);
	std::string		getName(void);
	std::string		getType(void);
	std::string		getFlavor(void);
	std::string		getSprite(void);
	int				getLootLevel(void);

private:
	std::map<std::string, std::map<std::string, Json::Value> >	_attr;

	std::string		_name;
	std::string		_type;
	std::string		_flavor;
	std::string		_sprite;
	int				_lootLevel;
	Bonus 			*_bonus;
	void			_readFile(std::string name);
	void			_parseJson(std::string file);

protected:
	Json::Value     _getAttr(std::string category, std::string key);
};

#endif
