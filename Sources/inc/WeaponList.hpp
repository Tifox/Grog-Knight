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
 * File: WeaponList.hpp
 * Creation: 2015-02-18 14:00
 * Vincent Rey <vrey@student.42.fr>
 */

#ifndef __WeaponList__
# define __WeaponList__

# include "Log.hpp"
# include "Elements.hpp"
# include "Weapon.hpp"
# include "json/json.h"

class WeaponList: public Elements {
public:
	WeaponList(void);
	~WeaponList(void);

	void	statWeapon(std::string);
	Weapon*	equipWeapon(std::string);

private:
	std::list<Weapon*> _allWeapons;

};

#endif
