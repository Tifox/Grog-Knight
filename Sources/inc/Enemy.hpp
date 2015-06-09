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
 * File: Enemy.hpp
 * Creation: 2015-02-23 14:25
 * Manon Budin <mbudin@student.42.fr>
 */


#ifndef __Enemy__
# define __Enemy__

# include "Characters.hpp"
# include "Loot.hpp"
# include "Map.hpp"
# include "Pattern.hpp"
# include "PassivePattern.hpp"

class Enemy : public Characters {
public:
	Enemy();
	Enemy(std::string);
	~Enemy();

	int				takeDamage(int damage);
	void			actionCallback(std::string name, int status);
	void			init(void);
	void			BeginContact(Elements* m, b2Contact* contact);
	void			EndContact(Elements *m, b2Contact *contact);
	bool			toBeDeleted(void);
	void			setMap(Map *m);
	Map				*getMap(void);
	void			setPattern(CPattern *p);
	CPattern			*getPattern(void);
	bool			dead(void);

protected:
	int				_isTakingDamage;
	bool			isDead;
	Map				*_map;
	CPattern			*_pattern;
	int				_lastElement;
};

#endif
