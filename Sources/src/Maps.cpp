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
	t_map				current;
	int					index, v;
	Json::Value			blocks;
	Elements			*currentBlock;
	Json::ValueIterator	itr;

	current.id = this->_root["general"].get("id", 0).asInt();
	current.name = this->_root["general"].get("name", "Map").asString();
	current.world = this->_root["general"].get("world", "Base").asString();
	current.callbackBegin = this->_root["general"].get("callbackBegin", "basic").asString();
	current.callbackEnd = this->_root["general"].get("callbackEnd", "basic").asString();
	blocks = this->_root["Blocks"];

	for (index = 0; index < blocks.size(); index++) {
		currentBlock = new Elements(index);
		for (itr = blocks[std::to_string(index)].begin(); itr != blocks[std::to_string(index)].end(); itr++)
			currentBlock->addAttribute(itr.key().asString(), (*itr).asString());
		current.elements[index] = currentBlock;
	}

	blocks = this->_root["Map"];
	std::vector<int> 	tmp;
	for (index = 0; index < blocks.size(); index++) {
		for (v = 0; v < blocks[index].size(); v++) {
			tmp.push_back(blocks[index][v].asInt());
		}
		current.map.push_back(tmp);
		tmp.clear();
	}
	this->_maps.push_back(current);
}

/* GETTERS */

std::list<t_map>	Maps::getFormattedMaps(void) { return this->_maps; };
