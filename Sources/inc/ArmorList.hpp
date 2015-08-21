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
 * File: ArmorList.hpp
 * Creation: 2015-05-07 17:33
 * Manon Budin <mbudin@student.42.fr>
 */


#ifndef __ArmorList__
# define __ArmorList__

# include "Armor.hpp"
# include "Log.hpp"
# include "Elements.hpp"
# include <sstream>

# ifdef __APPLE__
#  include "../../Tools/jsoncpp/include/json/json.h"
# else
#  include "json/json.h"
# endif

# include <list>
# include "../../Angel/Angel.h"
# ifndef __Elements__
# include "Elements.hpp"
#endif

class Armor;

class ArmorList: public Elements {
public:
	ArmorList(void);
	~ArmorList(void);

	void					statArmor(std::string);
	int						checkExists(std::string);
	Armor*					getArmor(std::string);
	Armor*					getArmorRandom(void);
	Armor*					getArmorRandom(int level);
	std::list<Armor *>		get3Starters(int level);

private:
	std::list<Armor*> _allArmors;

};

#endif
