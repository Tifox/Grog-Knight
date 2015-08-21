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

/****
*** * File: Quit.hpp
* *** Creation: 2015-06-23 16:36
*** * Ne02ptzero <louis@ne02ptzero.me>
* *** lsolofri@e2r12p22.42.fr
****/

#ifndef __QUIT__
# define __QUIT__

# include "Menu.hpp"
# include "Key.hpp"
# ifdef __APPLE__
#  include "../../Tools/jsoncpp/include/json/json.h"
# else
#  include "json/json.h"
# endif

# include <sstream>

class	Hero;

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

class	Quit {

	public:
		Quit(void);
		~Quit(void);

		static void		quitGame(void);
		static void		writeBindings(std::map<std::string, std::list<t_bind *> > binds);
		static int		isUpper(std::string s);
		static void		doSave(Hero *h);
		static std::map<std::string, Json::Value>		getSave(void);
		static void		cheater(void);
};

#endif
