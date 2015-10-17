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
 * File: ControllerManager.hpp
 * Creation: 2015-09-10 09:48
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

#ifndef __Controller_Manager__
# define __Controller_Manager__

# include "../../Angel/Angel.h"
# include "../../Tools/jsoncpp/include/json/json.h"
# include "Log.hpp"
# include <list>
# include <map>
# include <iostream>
# include <fstream>

class	ControllerInputManager : public MessageListener {
	public:
		ControllerInputManager(void);
		~ControllerInputManager(void);
		void	ReceiveMessage(Message *m);
		void	tick(void);
		void	addBindings(std::string button, std::string broadcast);

		int										flag;

	private:
		void			_parseBindings(void);

		std::string		_lastDirection;
		std::list<std::string>	_buttons;
		std::map<std::string, std::string>		_bindings;

};

# include "Game.hpp"

#endif
