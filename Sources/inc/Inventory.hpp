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
 * File: Inventory.hpp
 * Creation: 2015-05-27 04:44
 * Vincent Rey <vrey@student.42.fr>
 */

#ifndef __Inventory__
# define __Inventory__

# include "Log.hpp"
# include "Elements.hpp"
# ifdef __APPLE__
#  include "../../Tools/jsoncpp/include/json/json.h"
# else
#  include "json/json.h"
# endif

class Inventory {

public:
	Inventory(int slots);

	void			changeItemFocus(void);
	std::string		getCurrentFocus(void);
	int				addItemToInventory(std::string item);
	std::string		dropSelectedItem(void);
	std::string		equipSelectedItem(void);
	void			swapEquipmentAndInventory(std::string item);
	std::map<int, std::string>	getItems(void);
	int							getSlots(void);
	int							getNumFocus(void);

private:
	int							_slots;
	int							_focused;
	int							_inInventory;
	std::map<int, std::string>	_items;
};

#endif
