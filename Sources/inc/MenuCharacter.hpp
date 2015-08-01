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
 * File: MenuCharacter.hpp
 * @date 2015-07-23 16:44
 * @author Louis Solofrizzo <louis@ne02ptzero.me>
 */

#ifndef _MENU_CHARACTER_
# define _MENU_CHARACTER_

# include "Characters.hpp"

class		MenuCharacter : public Characters {
	public:
		MenuCharacter(void);
		~MenuCharacter(void);
		void		init(void);
		void		actionCallback(std::string name, int status);
		virtual void	BeginContact(Elements *elem, b2Contact *contact);
		virtual void	EndContact(Elements *elem, b2Contact *contact);
		virtual void	AnimCallback(String s);
		virtual void	ReceiveMessage(Message *m);
	protected:
		virtual void	_forward(int status);
		virtual void	_backward(int status);
};


#endif
