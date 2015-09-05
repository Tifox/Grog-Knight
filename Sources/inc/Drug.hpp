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
 * File: Drug.hpp
 * Creation: 2015-05-07 17:44
 * Manon Budin <mbudin@student.42.fr>
 */

#ifndef __Drug__
# define __Drug__

# include "Log.hpp"
# include "Elements.hpp"
 # include <sstream>
# ifdef __APPLE__
#  include "../../Tools/jsoncpp/include/json/json.h"
# else
#  include "json/json.h"
# endif

# include <list>
# include <map>
# include "../../Angel/Angel.h"
# ifndef __Elements__
# include "Elements.hpp"
#endif

class Characters;

class Drug: public Elements {
public:
	Drug(std::string name);
	Drug(Drug* Drug);
	~Drug(void);

	void	pot(int status);
	void	cocaine(int status);
	void	morphine(int status);


	void			ReceiveMessage(Message *m);

	std::string		getName(void);
	std::string		getType(void);
	std::string		getFlavor(void);
	std::string		getSprite(void);
	int 			getEffect(void);


private:
	std::map<std::string, std::map<std::string, Json::Value> >	_attr;
	std::list<std::string> _allDrugs;


	std::string		_name;
	std::string		_type;
	std::string		_flavor;
	std::string		_sprite;
	std::string 	_curDrug;
	int 			_effect;
	void			_readFile(std::string name);
	void			_parseJson(std::string file);

protected:
	Json::Value     _getAttr(std::string category, std::string key);
};

#endif
