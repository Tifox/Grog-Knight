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
 * File: Map.hpp
 * Creation: 2015-03-02 19:10
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "Map.hpp"

//! Basic constructor
Map::Map(void) : _mapCount(0) {
	return ;
}

//! Main constructor
/**
 * @param: name The name of the map
 */
Map::Map(std::string name) : _name(name), _mapCount(0) {
	return ;
}

//! Basic destructor
Map::~Map(void) {
	return ;
}

/* SETTERS */

void	Map::setHeight(int h) { this->_height = h; };
void	Map::setWidth(int w) { this->_width = w; };
void	Map::setImage(std::string n) { this->_image = n; };
void	Map::setTileHeight(int h) { this->_tileHeight = h; };
void	Map::setTileWidth(int w) { this->_tileWidth = w; };
void	Map::setImageHeight(int h) { this->_imageHeight = h; };
void	Map::setImageWidth(int w) { this->_imageWidth = w; };
void	Map::setMap(std::vector<int> map) { this->_map = map; };
void	Map::setProperties(std::map<int, std::map<std::string, Json::Value> > p) { this->_properties = p; };
void	Map::addElement(Elements *e) { this->_elems.push_back(e); };
void	Map::addMapElement(int n) { this->_map[this->_mapCount++] = n; };

//! Display the map
/**
 * Iterate and display the element of a map.
 * Lot's of stuff in this function, but the code's pretty clear, see source for more information
 */
void	Map::display(void) {
	float						x = 0, y = 0;
	std::vector<int>::iterator	it;
	Elements					*elem;

	for (it = this->_map.begin(); it != this->_map.end(); it++, x++) {
		if (x == this->_width) {
			x = 0;
			y--;
		}

		elem = new Elements();
		elem->removeAttr("physic");
		elem->addAttribute("image", this->_image);
		elem->setFrame(*(it));
		elem->setXStart(x);
		elem->setYStart(y);
		elem->setCutWidth(this->_tileWidth);
		elem->setCutHeight(this->_tileHeight);
		elem->setWidth(this->_imageWidth);
		elem->setHeight(this->_imageHeight);
		elem->addAttribute("type", "ground");
		elem->addAttribute("spriteMap", "TRUE");
		if (this->_properties.find(*it) == this->_properties.end()) {
			elem->addAttribute("physic", "TRUE");
		} else {
			std::map<std::string, Json::Value>::iterator	it2;
			int												isPhysic = 1;

			for (it2 = this->_properties.find(*it)->second.begin();
			it2 != this->_properties.find(*it)->second.end(); it2++) {
				if (it2->first == "physic") {
					isPhysic = 0;
				} else if (it2->first == "hitbox") {
					elem->setHitbox(it2->second.asString());
				}
			}
			if (isPhysic)
				elem->addAttribute("physic", "TRUE");
		}
		elem->display();
	}
}
