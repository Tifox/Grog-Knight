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

//! Constructor called at the beginning, parses json and creates a list containing all the hitbox listed in the files
/**
 * Default constructor, creating the list
 * /!\ General warning:
 * /!\ Box2D hitbox creator does not allow : concave polygons, less-than-four vertices polygons (aka triangles), more-than-eight vertices polygons
 * /!\ The game will either not start or crash during startup if one of theses is attempted
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
			this->_hitboxes[res] = this->_getPolygon(res);
		}
	}
}

//! Readability function, follow-up from the default constructor
/**
 * Checks for the number of vertices in the hitbox json file
 * @param res the location of the file
 */
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

//! Json parser, see Characters/Weapons
b2PolygonShape	Hitbox::_parseJson(std::string file) {
	Json::Reader    read;
	Json::Value     json;
	Json::Value		hitbox;
	int				v, i, j, vertices = 0;
	std::vector<std::vector<int> >		map;
	std::vector<int>					tmp;

	if (!read.parse(file, json))
		Log::error("Error in json syntax :\n" + read.getFormattedErrorMessages());

	hitbox = json["data"]["hitbox"];
	for (v = i = j = 0; i < hitbox.size(); i++, v++) {
		if (v == 11) {
			map.insert(map.begin() + j, tmp);
			j++;
			v = 0;
			tmp.clear();
		}
		tmp.insert(tmp.begin() + v, hitbox[i].asInt());
	}
	map.insert(map.begin() + j, tmp);

	std::vector<std::vector<int> >::iterator	itr;
	std::vector<int>::iterator	itr2;

	for (itr = map.begin(); itr != map.end(); itr++) {
		for (itr2 = (*itr).begin(); itr2 != (*itr).end(); itr2++) {
			if ((*itr2) == 1)
				vertices++;
		}
	}

	if (vertices < 4 || vertices > 8)
		Log::error(json["data"]["name"].asString() + " contains an invalid hitbox");
	return this->_parseVertices(vertices, map);
}

//! Creates the polygon based on the vertices
b2PolygonShape	Hitbox::_parseVertices(int v, std::vector<std::vector<int> > map) {
	std::vector<std::vector<int> >::iterator	itr;
	std::vector<int>::iterator	itr2;
	std::vector<std::vector<int> >::reverse_iterator	ritr;
	std::vector<int>::reverse_iterator	ritr2;
	b2Vec2 vertices[8]/* =  new b2Vec2[8]*/;
	//std::vector<b2Vec2> vertices(v * 8);

	//vertices = (b2Vec2 *)malloc(sizeof(b2Vec2) * v + 1);
	int 		i = 0;

	b2PolygonShape box;
	float x2, y2, x, y = 0;

	for (itr = map.begin(); itr != map.end(); itr++, y++) {
		for (itr2 = (*itr).begin(), x = 0; itr2 != (*itr).end(); itr2++, x++) {
			if ((*itr2) == 1) {
				//				std::cout << x - 5 << ":" << -(y - 5) << std::endl;
				vertices[i].Set((x - 5) / 10, -(y - 5)/ 10 );
				i++;
				x = 0;
				break ;
			}
		}
	}
	for (ritr = map.rbegin(), y = 10; ritr != map.rend(); ritr++, y--) {
		for (ritr2 = (*ritr).rbegin(), x = 10; ritr2 != (*ritr).rend(); ritr2++, x--) {
			if ((*ritr2) == 1) {
				vertices[i].Set((x - 5) / 10, -(y - 5)/ 10 );
				//				std::cout << x - 5 << ":" << -(y - 5) << std::endl;
				i++;
				break ;
			}
		}
	}
	box.Set(vertices, i);
	return box;
}

//! Always return 1, cause its funnier
/**
 * Function that checks if requested hitbox does exists, as the object cannot be null
 * @param n name of the hitbox
 * Do not call getHitbox if this function returns false
 * @todo yeah...
 */
int			Hitbox::checkExists(std::string n) {
	return 1;
}

//! Returns the ordered vertices in order to create the hitbox when necessary
/**
 * Called mostly by characters, weapons, or objects to load a pre-designed hitbox
 * @param n name of the hitbox
 */
b2PolygonShape	Hitbox::getHitbox(std::string n) {
	return this->_hitboxes[n];
}

/**
 * Basic destructor
 */
Hitbox::~Hitbox(void) {
	return;
}
