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

/**
 * Base constructor
 */
Maps::Maps(void) {
	return ;
};

/**
 * Constructor with a map directory
 * @param: directory (std::string)
 */
Maps::Maps(std::string directory) : _directory(directory) {
	return ;
}

/**
 * Base Deconstructor
 */
Maps::~Maps(void) {
	return ;
}

/**
 * Read the maps into a list
 */
void	Maps::readMaps(void) {
	DIR		*dir = opendir(this->_directory.c_str());
	struct dirent		*ent;
	std::string			file;
	std::stringstream 	buffer;
	std::ifstream		fd;

	if (!dir)
		std::cout << "Error at opening dir" << std::endl;
	for (; ent = readdir(dir); ) {
		if (ent->d_name[0] != '.') {
			file = "./Maps/" + std::string(ent->d_name);
			fd.open(file.c_str());
			buffer << fd.rdbuf();

			if (!this->_reader.parse(buffer.str(), this->_root))
				std::cout << this->_reader.getFormattedErrorMessages() << std::endl;
			else
				this->_getMap();
		}
	}
}

/**
 * Feed the empty list
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
				tileproperties[atoi(j.key().asString().c_str())][k.key().asString()] = *k;
		}
	}
	map->setProperties(tileproperties);
	for (index = 0; index < itr.size(); index++) {
		intMap.insert(intMap.begin() + index, itr[index].asInt());
	}
	map->setMap(intMap);
	this->_maps.push_back(map);
}

/**
 * Display the map at the top of list
 */
void	Maps::firstOne(void) {
	this->_maps.front()->display();
}
