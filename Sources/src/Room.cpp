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
 * File: LevelGenerator.cpp
 * Creation: 2015-03-02 16:05
 * Matthieu Maudet <mmaudet@student.42.fr>
 */

# include "../inc/Room.hpp"

/**
 * Basic constructor
 */
Room::Room(int id, int y, int x, int mapId) : _id(id), _y(y), _x(x), _mapId(mapId) {
	return ;
}

/**
 * Basic Destructor
 */
Room::~Room(void) {
	return ;
}

int Room::getX(void) {
	return this->_x;
}

int Room::getY(void) {
	return this->_y;
}

int Room::getMapId(void) {
	return this->_mapId;
}