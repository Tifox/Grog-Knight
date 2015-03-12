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
 * File: Equipment.hpp
 * Creation: 2015-03-06 15:51
 * Manon Budin <mbudin@student.42.fr>
 */

#ifndef __Equipment__
# define __Equipment__

# include "Elements.hpp"
# include "Hero.hpp"
# include "WeaponList.hpp"
# include "Object.hpp"

class Equipment : public Object {
public:
	Equipment();
	~Equipment();

	void			init(void);
	Weapon* 	getWeapon(void);
	void	BeginContact(Elements *elem, b2Contact *contact);
private:
	Weapon*			_weapon;
};

#endif
