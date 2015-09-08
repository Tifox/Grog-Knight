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
 * File: Chest.cpp
 * Creation: 2015-08-27 04:44
 * Vincent Rey <vrey@student.42.fr>
 */

#ifndef __Chest__
# define __Chest__

# include "Elements.hpp"

class Chest: public Elements {

	public:
		Chest(void);
		~Chest(void);
		void		spawn(void);
		int			isUsed;
		void		displayInterface(void);
		void		removeInterface(void);
		void		displayChestContent(void);
		void		ReceiveMessage(Message *m);
		void		makeChoices(void);
		void		updateItems(void);

	private:
		std::list<HUDActor *>		_interfaceElem;
		std::map<int, std::string>	_chestItems;
		std::map<int, HUDActor*>	_img;
		std::list<HUDActor *>		_choices;
		HUDActor *					_choicePointer;
		HUDActor*					_target;
};

# endif
