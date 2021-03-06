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
 * File: Tooltip.hpp
 * Creation: 2015-06-03 14:59
 * Manon Budin <mbudin@student.42.fr>
 */

#ifndef __Tooltip__
# define __Tooltip__

# include "Log.hpp"
# include "../../Angel/Angel.h"

class Characters;	
class Elements;	


# ifdef __APPLE__
#  include "../../Tools/jsoncpp/include/json/json.h"
# else
#  include "json/json.h"
# endif


class Tooltip : public MessageListener {
	public:
		Tooltip();
		~Tooltip();

		void	tip(Elements *elem, Characters *c);
		void	info(Elements *elem);
		void	talk(Elements *elem);
		void	clearInfo(int clean = 1);
		void 	ReceiveMessage(Message *m);


	private:
		HUDActor			*_equip;
		std::string 		_name;
		std::string 		_flavor;
		std::string 		_talk;
		std::string 		_hp;
		std::string			_speed;
		std::string			_reduc;
		std::string			_dmg;
		std::string 		_val;
		Elements			*_lastElem;
};


# include "Characters.hpp"

#endif
