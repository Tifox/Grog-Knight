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
	_maxSearchDistance = height * width / 2;
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
	print();
	thirdPass();
	fourthPass();
	shockwave();
}

void LevelGenerator::print(void) {
	int n = 0;
	Log::info("Printing generated level : debug displays the number of links between rooms");
	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			if (n < _rooms->size()) {
				Room room = _rooms->at(n);
				if (room.getX() == j && room.getY() == i) {
					std::cout << room.getDistance();
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


//Generating base level with random rooms
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

//Linking rooms beteween them
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

// 3rd pass => supprimer les salles avec 0 liens
// 4th pass => shockwave pour tester que toutes les rooms sont reachable
// 5th pass => tester si on peut ajouter des rooms pour rendre reachable
// restart si ca foire
// 6th pass => ajouter entree
// 7th pass => shockwave depuis entree
// 8th pass => ajouter salle du boss
// 9th pass => ajouter salles speciales

//Deleting isolated rooms
void LevelGenerator::thirdPass(void) {
	int size = _rooms->size();
	int deleted = 0;
	for(int i = 0; i + deleted < size; i++) {
		Room *room = &_rooms->at(i);
		if (room->getLinks() == 0){
			std::cout << "erase room n" << room->getId() << std::endl; 
			_rooms->erase(_rooms->begin() + i);
			deleted++;
			i--;
		}
	}
}

//Creating entry room
void LevelGenerator::fourthPass(void) {
	int size = _rooms->size();
	Room *room = &_rooms->at(rand() % size);
	room->setSpecialType(ENTRY);
	room->setDistance(1);
}

//Computing distance between entry and other rooms
void LevelGenerator::shockwave() {
	int size = _rooms->size();
	int searchDistance = 1;
	while(searchDistance < _maxSearchDistance) {
		for(int i = 0; i < size; i++) {
			Room *room = &_rooms->at(i);
			if (room->getDistance() == searchDistance) {
				addDistanceToAdjacentRooms(room, searchDistance);
			}	
		}
		searchDistance++;
	}
}

int LevelGenerator::addDistanceToAdjacentRooms(Room *room, int searchDistance) {
	int x = room->getX();
	int y = room->getY();
	bool top = room->getTopDoor();
	bool left = room->getLeftDoor();
	bool bottom = room->getBottomDoor();
	bool right = room->getRightDoor();
	int toReturn;

	if (top == true) {
		Room *topRoom = findRoomByPos(x, y - 1);
		int distance = topRoom->getDistance();
		if (distance != 0 && distance < searchDistance)
			toReturn += 0;
		else {
			topRoom->setDistance(searchDistance + 1);
			toReturn += 1;
		}
	}
	if (left == true) {
		Room *leftRoom = findRoomByPos(x - 1, y);
		int distance = leftRoom->getDistance();
		if (distance != 0 && distance < searchDistance)
			toReturn += 0;
		else {
			leftRoom->setDistance(searchDistance + 1);
			toReturn += 1;
		}
	}
	if (bottom == true) {
		Room *bottomRoom = findRoomByPos(x, y + 1);
		int distance = bottomRoom->getDistance();
		if (distance != 0 && distance < searchDistance)
			toReturn += 0;
		else {
			bottomRoom->setDistance(searchDistance + 1);
			toReturn += 1;
		}
	}
	if (right == true) {
		Room *rightRoom = findRoomByPos(x + 1, y);
		int distance = rightRoom->getDistance();
		if (distance != 0 && distance < searchDistance)
			toReturn += 0;
		else {
			rightRoom->setDistance(searchDistance + 1);
			toReturn += 1;
		}
	}
	return toReturn;
}

Room *LevelGenerator::findRoomByPos(int x, int y) {
	int size = _rooms->size();
	for(int i = 0; i < size; i++) {
		Room *room = &_rooms->at(i);
		if (room->getX() == x && room->getY() == y) {
			return room;
		}
	}
	return NULL;
}

int LevelGenerator::getEntryId() {
	int size = _rooms->size();
	for(int i = 0; i < size; i++) {
		Room *room = &_rooms->at(i);
		if (room->getSpecialType() == ENTRY) {
			std::cout << "ENTRY found in " << room->getY() << "." << room->getX() << std::endl; 
			return i;
		}
	}
}

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
