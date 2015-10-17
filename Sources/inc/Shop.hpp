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
 * File: Shop.hpp
 * Creation: 2015-08-02 14:44
 * Vincent Rey <vrey@student.42.fr>
 */

#ifndef __Shop__
# define __Shop__

# include "Game.hpp"

class Shop: public Elements {
public:
	class ShopItem: public Elements {
	public:
		ShopItem(std::string name, int x, int y, int num);
	};
	Shop(void);
	Shop(int x, int y, int lvl, int nb);
	~Shop(void);
	void	revealShop(int x, int y);
	void	hideShop(void);
	void	ReceiveMessage(Message *m);

private:
	int	_nbItems;
	int	_lvlItems;

	std::vector<std::string> _items;
	std::vector<ShopItem*> _shopItems;

};

#endif
