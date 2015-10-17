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
 * File: Shopkeeper.hpp
 * Creation: 2015-08-07 15:14
 * Vincent Rey <vrey@student.42.fr>
 */

#ifndef __Shopkeeper__
# define __Shopkeeper__

# include "Game.hpp"

class Shop;

class Shopkeeper : public Characters {

public:
	Shopkeeper(std::string);
	~Shopkeeper();

	void	init();
	virtual void	BeginContact(Elements* m, b2Contact* contact);
	virtual void	EndContact(Elements* m, b2Contact* contact);
	void			ReceiveMessage(Message *m);
	void			spawn();
	void			displayText(std::string say);
	void			removeText(void);
	Shop			*getShop();

private:
	Shop*			_shop;
	std::string		_currentPhrase;
};
# include "Shop.hpp"

#endif
