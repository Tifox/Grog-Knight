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
 * File: Maps.hpp
 * Creation: 2015-02-13 08:03
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

#ifndef __Maps__
# define __Maps__

# include "../../Tools/jsoncpp/include/json/json.h"
# include <dirent.h>
# include <iostream>
# include <string>
# include <fstream>
# include <streambuf>
# include <sstream>
# include <list>
# include <string>
# include <vector>
# include <map>

# include "main.hpp"
# include "Elements.hpp"
# include "Map.hpp"

class Elements;

class Maps {
	public:
		Maps();
		Maps(std::string directory);
		~Maps();

		void	readMaps(void);
		int		rstrncmp(char *str, char *str2, int n);
		void	displayLevel(void);

	private:
		void	_getMap(void);

		std::string			_directory;
		Json::Value			_root;
		Json::Reader		_reader;
		std::map<int, Map *>	_maps;
};

# include "Elements.hpp"
#endif
