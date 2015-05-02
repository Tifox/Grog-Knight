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
 * Creation: 2015-02-02 19:06
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

#ifndef __Map__
# define __Map__

# include <vector>
# include <list>
# include <iostream>
# include "Game.hpp"

# ifdef __APPLE__
#  include "../../Tools/jsoncpp/include/json/json.h"
# else
#  include "json/json.h"
# endif

class	Map {

	public:
		Map();
		Map(std::string name);
		~Map();

		void	setHeight(int h);
		void	setWidth(int w);
		void	setImage(std::string n);
		void	setTileHeight(int h);
		void	setTileWidth(int w);
		void	setImageHeight(int h);
		void	setImageWidth(int w);
		void	addElement(Elements *e);
		void	addMapElement(int n);
		void	setMap(std::vector<int> map);
		void	setProperties(std::map<int, std::map<std::string, Json::Value> > p);
		void	setLayer(int n);
		void	setXStart(int x);
		void	setYStart(int y);
		void	display(void);
		int		getHeight(void);
		int		getWidth(void);

	private:
		int					_mapCount;
		std::string			_name;
		int					_height;
		int					_width;
		std::string			_image;
		std::list<std::vector<int> >	_map;
		int					_tileHeight;
		int					_tileWidth;
		int					_imageHeight;
		int					_imageWidth;
		int					_layer;
		int					_xStart;
		int					_yStart;
		std::list<Elements *>	_elems;
		std::map<int, std::map<std::string, Json::Value> >	_properties;
};

#endif
