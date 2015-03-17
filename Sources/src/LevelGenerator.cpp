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

# include "../inc/LevelGenerator.hpp"
# include "../inc/Room.hpp"

/**
 * Standard constructor with :
 * - Height and Width
 * - minPathLenght : Min path between entry and exit rooms
 * - roomPopRate (of 100%) : Chances for a room to pop on given slot
 * - doorsPopRate (of 100%) : Chances for a door to pop on given wall
 * - nbMaps : Number of different maps
 */
LevelGenerator::LevelGenerator(int height, int width, int minPathLenght, int roomPopRate, int doorsPopRate) : _height(height), _width(width), _minPathLenght(minPathLenght), _roomPopRate(roomPopRate), _doorsPopRate(doorsPopRate) {
	srand(time(NULL));
	_rooms = new std::vector<Room>;
	_nbMaps = 10;
	return ;
}

/**
 * Basic Destructor
 */
LevelGenerator::~LevelGenerator(void) {
	return ;
} 

void LevelGenerator::execute(void) {
	firstPass();
	secondPass();
	// thirdPass();
}

void LevelGenerator::print(void) {
	int n = 0;
	Log::info("Printing generated level : debug displays the number of links between rooms");
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			if (n < _rooms->size()) {
				Room room = _rooms->at(n);
				if (room.getX() == j && room.getY() == i) {
					std::cout << room.getLinks();
					n++;
				}
				else
					std::cout << "-";
				std::cout << " ";
			}
			else
				std::cout << "- ";
		}
		std::cout << std::endl;
	}
}

void LevelGenerator::firstPass(void) {
	int id = 0;
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			if (_roomPopRate > (rand() % 100)) {
				Room *newRoom = new Room(id, i, j, rand() % _nbMaps);
				_rooms->push_back(*newRoom);
				id++;
			}
		}
	}
}

void LevelGenerator::secondPass(void) {
	int n = 0;
	int size = _rooms->size();
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			if (n < size) {
				Room *room = &_rooms->at(n);
				if (room->getX() == j && room->getY() == i) {
					linkAdjacentRooms(room);
					n++;
				}
			}
		}
	}
}

// void LevelGenerator::thirdPass(void) {
// 	int n = 0;
// 	int size = _rooms->size();
// 	_rooms[0].setDistance(0);
// 	for (int i = 0; i < _width; i++) {
// 		for (int j = 0; j < _height; j++) {
// 			// std::cout << i << " " << j << std::endl; 
// 			// if (n < size) {
// 			// 	Room *room = &_rooms->at(n);
// 			// 	if (room->getX() == j && room->getY() == i) {
// 			// 		shockwave();
// 			// 		n++;
// 			// 	}
// 			// }
// 		}
// 	}
// }

void LevelGenerator::linkAdjacentRooms(Room *room) {
	for (int n = 0; n < _rooms->size(); ++n)	{
		Room *tmp = &_rooms->at(n);
		if (room->getY() == tmp->getY() && room->getX() == tmp->getX() - 1 ) {
			room->setRightDoor(true);
			tmp->setLeftDoor(true);
			room->addLink();
		}
		else if (room->getY() == tmp->getY() && room->getX() == tmp->getX() + 1 ) {
			room->setLeftDoor(true);
			tmp->setRightDoor(true);
			room->addLink();
		}
		else if (room->getY() == tmp->getY() - 1 && room->getX() == tmp->getX()) {
			room->setBottomDoor(true);
			tmp->setTopDoor(true);
			room->addLink();
		}
		else if (room->getY() == tmp->getY() + 1 && room->getX() == tmp->getX()) {
			room->setTopDoor(true);
			tmp->setBottomDoor(true);
			room->addLink();
		}
	}
}
