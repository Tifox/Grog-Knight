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
 * File: Hitbox.cpp
 * Creation: 2015-02-18 14:00
 * Vincent Rey <vrey@student.42.fr>
 */

# include "Hitbox.hpp"


/**
 * Default constructor, creating the list
 * @param: Elements *
 */
Hitbox::Hitbox(void) {
	DIR				*dir;
	struct dirent	*dirEntry;
	std::istringstream	iss;
	std::string		res;

	dir = opendir("./Resources/Elements/Hitbox/");
	while (dirEntry = readdir(dir)) {
		if (dirEntry && strcmp(dirEntry->d_name, ".") && strcmp(dirEntry->d_name, "..")) {
			iss.str(dirEntry->d_name);
			std::getline(iss, res, '.');
			this->_hitboxes.push_back(this->_getPolygon(res));
		}
	}
}

b2PolygonShape	Hitbox::_getPolygon(std::string res) {
	std::string         file;
	std::stringstream   buffer;
	std::ifstream       fd;

	file = "./Resources/Elements/Hitbox/" + res + ".json";
	fd.open(file.c_str());
	if (!fd.is_open())
		Log::error("Can't open the file for the " +
				   res + " hitbox. (Expected path is Resources/Elements/Hitbox/" + res +".json)");
	buffer << fd.rdbuf();
	return this->_parseJson(buffer.str());
}

b2PolygonShape	Hitbox::_parseJson(std::string file) {
	Json::Reader    read;
	Json::Value     json;
	Json::Value		hitbox;
	int				vertices = 0;

    if (!read.parse(file, json))
		Log::error("Error in json syntax :\n" + read.getFormattedErrorMessages());
// 	for (int i = 0; hitbox[i]; i++) {
// 		if (hitbox[i] == 1)
// 			vertices++;
// 	}
// 	std::cout << vertices << std::endl;
}

/**
 * Basic destructor
 */
Hitbox::~Hitbox(void) {
	return;
}
