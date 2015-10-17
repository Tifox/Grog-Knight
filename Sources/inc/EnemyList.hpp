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
 * File: EnemyList.hpp
 * Creation: 2015-02-18 14:00
 * Vincent Rey <vrey@student.42.fr>
 */


#ifndef __EnemyList__
# define __EnemyList__

# include "Log.hpp"
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


class EnemyList: public Elements {
public:
	EnemyList(void);
	~EnemyList(void);

	std::string		getEnemyRandom(std::string flying);
	std::string		getEnemyRandom(int level, std::string flying);

	class EnemyData {
	public:
		EnemyData(std::string);
		~EnemyData();
		std::string	getName();
		int			getLevel();
		std::string	isFlying();

	private:
		std::string _name;
		int			_level;
		std::string	_flying;
		void		_readFile(std::string name);
		void		_parseJson(std::string file);
	};

private:
	std::list<EnemyData*> _allEnemies;

};

#endif
