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
 * File: Maps.cpp
 * Creation: 2015-02-13 08:03
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "../inc/Maps.hpp"

//! Base constructor
Maps::Maps(void) {
	return ;
};

//! Main constructor
/**
 * Constructor with a map directory
 * @param directory The path of the directory
 */
Maps::Maps(std::string directory) : _directory(directory) {
	return ;
}

//! Base Deconstructor
Maps::~Maps(void) {
	return ;
}

//! Reverse strncmp
/**
 * strncmp, but start from the end
 * This function is used to determine whether a file is a json or not
 * @param str The first string
 * @param str2 The second string
 * @param n The number of bits of comparison
 * @return 1 or 0, 0 if match (FREE BSD style)
 */
int		Maps::rstrncmp(char *str, char *str2, int n) {
	int		i, j;

	for (i = (strlen(str) - 1), j = (strlen(str2) - 1); str[i] && (i - strlen(str)) > -(n) 
		&& str[i] == str2[j]; i--, j--);
	if ((i - strlen(str)) == -(n))
		return 0;
	return 1;
}

//! Read map function
/**
 * Read the maps into a list.
 * This function list all the .json in the directory specificied, and parse each one.
 */
void	Maps::readMaps(void) {
	DIR		*dir = opendir(this->_directory.c_str());
	struct dirent		*ent;
	std::string			file;
	std::stringstream 	buffer;
	std::ifstream		fd;

	Log::info("Reading Maps...");
	if (!dir)
		std::cout << "Error at opening dir" << std::endl;
	for (; ent = readdir(dir); ) {
		if (ent->d_name[0] != '.' && !this->rstrncmp(ent->d_name, ".json", 6)) {
			file = "./Maps/" + std::string(ent->d_name);
			fd.open(file.c_str());
			buffer << fd.rdbuf();

			this->_root.clear();
			if (!this->_reader.parse(buffer.str(), this->_root))
				std::cout << this->_reader.getFormattedErrorMessages() << std::endl;
			else
				this->_getMap();
			buffer.str("");
			fd.close();
		}
	}
}

//! Parsing the Map
/**
 * This function parse the json of the map, and create a new Map object with it.
 * @sa Map
 */
void	Maps::_getMap(void) {
	int					index, v;
	Json::Value			blocks, itr, vtr;
	Elements			*currentBlock;
	Json::ValueIterator	j, k;
	Map					*map;
	std::map<int, std::map<std::string, Json::Value> >	tileproperties;
	std::vector<int>	intMap;

	map = new Map(this->_root["tilesets"][0].get("name", "").asString());
	map->setHeight(this->_root["height"].asInt());
	map->setWidth(this->_root["width"].asInt());
	itr = this->_root["tilesets"];
	for (index = 0; index < itr.size(); index++) {
		map->setImage(itr[index].get("image", "").asString());
		map->setImageHeight(itr[index].get("imageheight", 0).asInt());
		map->setImageWidth(itr[index].get("imagewidth", 0).asInt());
		map->setTileHeight(itr[index].get("tileheight", 0).asInt());
		map->setTileWidth(itr[index].get("tilewidth", 0).asInt());
		vtr = itr[index].get("tileproperties", "");
		for (j = vtr.begin(); j != vtr.end(); j++) {
			for (k = (*j).begin(); k != (*j).end(); k++)
				tileproperties[atoi(j.key().asString().c_str()) + 1][k.key().asString()] = *k;
		}
	}
	map->setProperties(tileproperties);
	for (v = 0; v < this->_root["layers"].size(); v++) {
		itr = this->_root["layers"][v]["data"];
		for (index = 0; index < itr.size(); index++)
			intMap.insert(intMap.begin() + index, itr[index].asInt());
		map->setMap(intMap);
		intMap.clear();
	}
	this->_maps[atoi(this->_root["properties"].get("number", 0).asString().c_str())] = map;
	this->_root.clear();
}

//! Display an entire level
void	Maps::displayLevel(void) {
	int		i, x, y;
	int		columnSize = 2;

	for (i = 1, x = y = 0; i <= this->_maps.size(); i++) {
		if (i == (columnSize + 1)) {
			x = 0;
			y += -(this->_maps[i]->getHeight());
		}
		this->_maps[i]->setXStart(x);
		this->_maps[i]->setYStart(y);
		this->_maps[i]->display();
		x += this->_maps[i]->getWidth();
	}
	Game::maxX = x;
	Game::maxY = y;
}
