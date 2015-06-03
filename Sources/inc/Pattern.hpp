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
 * File: Pattern.cpp
 * Creation: 2015-05-03 10:22
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

#ifndef __Pattern__
# define __Pattern__
# include <iostream>
class		Enemy;
class		Map;

class		Pattern {
	public:
		Pattern(void);
		Pattern(std::string name);
		Pattern(Pattern & p);
		~Pattern(void);

		virtual void	tick(Map m);
		void			setEnemy(Enemy *);
		std::string		getName(void);
		Enemy			*getEnemy(void);

	protected:
		std::string		_name;
		Enemy			*_enemy;
		int				_x;
		int				_y;
		int				_orientation;
};

# include "Enemy.hpp"
# include "Map.hpp"

#endif
