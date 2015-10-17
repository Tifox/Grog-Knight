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

# include "LevelGenerator.hpp"
# include "Room.hpp"

/**
 * Standard constructor with :
 * - Height and Width
 * - minPathLenght : Min path between entry and exit rooms
 * - roomPopRate (of 100%) : Chances for a room to pop on given slot
 * - doorsPopRate (of 100%) : Chances for a door to pop on given wall
 * - nbMaps : Number of different maps
 */
LevelGenerator::LevelGenerator(int maxMapSize, int minPathLenght, int doorsPopRate) : _maxMapSize(maxMapSize), _minPathLenght(minPathLenght), _doorsPopRate(doorsPopRate) {
	srand(time(NULL));
	_rooms = new std::vector<Room*>;
	_nbMaps = 10;
	_id = 0;
	//_maxSearchDistance = height * width / 2;
	return;
}

/**
 * Basic Destructor
 */
LevelGenerator::~LevelGenerator(void) {
	return;
}

void LevelGenerator::execute(void) {
	int		i;
	createFirstRoom();
	for (int distance = 0; distance <= _maxMapSize; distance++)
		firstPass(distance);
}

void LevelGenerator::print() {
	for (unsigned int i = 0; i < _rooms->size(); i++){
		Room *room = _rooms->at(i);
		std::cout << "Room " << room->getId() << " in " << room->getX() << "/" << room->getY() << " " << room->getDistance() << " top = " << room->getTopDoor() << " left = " << room->getLeftDoor() << " bottom = " << room->getBottomDoor() << " right = " << room->getRightDoor() << std::endl;
	}
}

void LevelGenerator::createFirstRoom(void) {
	bool topDoor = (_doorsPopRate >(rand() % 100));
	bool leftDoor = (_doorsPopRate >(rand() % 100));
	bool bottomDoor = (_doorsPopRate >(rand() % 100));
	bool rightDoor = (_doorsPopRate >(rand() % 100));
	Room *newRoom = new Room(id(), 0, 0, 0, rand() % _nbMaps, topDoor, leftDoor, bottomDoor, rightDoor);
	_rooms->push_back(newRoom);
}

void LevelGenerator::firstPass(int distance) {
	for (unsigned int i = 0; i < _rooms->size(); i++){
		Room *room = _rooms->at(i);
		if (room->getDistance() == distance)
			generateRoomsBehindDoors(room);
	}
}

void LevelGenerator::generateRoomsBehindDoors(Room* room) {
	if (room->getTopDoor() == true) {
		room->setTopRoom(generateRoom(room->getX(), room->getY() + 1, room->getDistance() + 1, room, 0));
	}
	if (room->getLeftDoor() == true) {
		room->setLeftRoom(generateRoom(room->getX() - 1, room->getY(), room->getDistance() + 1, room, 1));
	}
	if (room->getBottomDoor() == true) {
		room->setBottomRoom(generateRoom(room->getX(), room->getY() - 1, room->getDistance() + 1, room, 2));
	}
	if (room->getRightDoor() == true) {
		room->setRightRoom(generateRoom(room->getX() + 1, room->getY(), room->getDistance() + 1, room, 3));
	}
}

Room* LevelGenerator::generateRoom(int x, int y, int distance, Room* prevRoom, int origin) {
	if (testRoom(x, y) == true)
		return 0;
	bool topDoor = testDoor(x, y + 1);
	bool leftDoor = testDoor(x - 1, y);
	bool bottomDoor = testDoor(x, y - 1);
	bool rightDoor = testDoor(x + 1, y);
	Room *newRoom = new Room(id(), y, x, distance, rand() % _nbMaps, topDoor, leftDoor, bottomDoor, rightDoor);
	if (origin == 0)
		newRoom->setBottomRoom(prevRoom);
	else if (origin == 1)
		newRoom->setRightRoom(prevRoom);
	else if (origin == 2)
		newRoom->setTopRoom(prevRoom);
	else if (origin == 3)
		newRoom->setLeftRoom(prevRoom);
	_rooms->push_back(newRoom);
	return newRoom;
}

int LevelGenerator::id(void) {
	return _id++;
}

bool LevelGenerator::testRoom(int x, int y) {
	for (unsigned int i = 0; i < _rooms->size(); i++){
		Room *room = _rooms->at(i);
		if (room->getX() == x && room->getY() == y) {
			// std::cout << "Already in " << room->getX() << "/" << room->getY() << std::endl;
			return true;
		}
	}
	return false;
}

bool LevelGenerator::testDoor(int x, int y) {
	for (unsigned int i = 0; i < _rooms->size(); i++){
		Room *room = _rooms->at(i);
		if (room->getX() == x && room->getY() == y)
			return true;
	}
	if (_doorsPopRate > (rand() % 100))
		return true;
	return false;
}

int	LevelGenerator::getNbMaps(void) {
	return this->_nbMaps;
}

std::string							LevelGenerator::getSpecialRoom(int i, int j) {
	if (this->roomMap[i][j]) {
		if (this->roomMap[i][j]->getSpecialType() == BOSS) {
			return "bossMap";
		} else if (this->roomMap[i][j]->getSpecialType() == CHEST) {
			return "secretMap";
		} else if (this->roomMap[i][j]->getSpecialType() == ITEM) {
			return "dealer";
		}
	}
	return "";
}

std::vector<std::vector<int> >		LevelGenerator::getLevel(void) {
	std::vector<std::vector<int> >			map;
	int									i, j, score, mid, maxDistance;
	Room								*tmp, *first;

	mid = this->_maxMapSize;
	for (i = maxDistance = 0; i < this->_rooms->size(); i++)
		maxDistance = (this->_rooms->at(i)->getDistance() > maxDistance ?
				this->_rooms->at(i)->getDistance() : maxDistance);
	if ((this->_maxMapSize / 2) < maxDistance)
		this->_maxMapSize += maxDistance * 2;
	for (i = 0; i < this->_maxMapSize; i++) {
		map.push_back(std::vector<int>(this->_maxMapSize * 2));
		this->roomMap.push_back(std::vector<Room *>(this->_maxMapSize));
	}
	map.push_back(std::vector<int>(this->_maxMapSize * 2));
	map.push_back(std::vector<int>(this->_maxMapSize * 2));
	map.push_back(std::vector<int>(this->_maxMapSize * 2));
	map.push_back(std::vector<int>(this->_maxMapSize * 2));
	map.push_back(std::vector<int>(this->_maxMapSize * 2));
	mid += 4;
	for (i = score = 0; i < this->_rooms->size(); i++, score = 0) {
		tmp = this->_rooms->at(i);
		if (tmp->getTopDoor())
			score++;
		if (tmp->getRightDoor())
			score += 2;
		if (tmp->getBottomDoor())
			score += 4;
		if (tmp->getLeftDoor())
			score += 8;
		map[mid - tmp->getY()][tmp->getX() + mid] = score;
		this->roomMap[mid - tmp->getY()][tmp->getX() + mid] = tmp;
		if (tmp->getY() == 0 && tmp->getX() == 0) {
			this->_startX = tmp->getX() + mid;
			this->_startY = tmp->getY() + mid;
		}
	}
	for (i = 0; i < map.size(); i++) {
		for (j = 0; j < map[i].size(); j++) {
			if (map[i][j]) {
				// If a door is open and there is no room behind it
				if (this->roomMap[i][j]->getTopDoor()) {
					if ((i - 1) < 0)
						map[i][j]--;
					else if (!map[i - 1][j])
						map[i][j]--;
				} if (this->roomMap[i][j]->getRightDoor()) {
					if ((j + 1) > map[i].size())
						map[i][j] -= 2;
					else if (!map[i][j + 1])
						map[i][j] -= 2;
				} if (this->roomMap[i][j]->getBottomDoor()) {
					if ((i + 1) > map.size())
						map[i][j] -= 4;
					else if (!map[i + 1][j])
						map[i][j] -= 4;
				} if (this->roomMap[i][j]->getLeftDoor()) {
					if ((j - 1) < 0)
						map[i][j] -= 8;
					else if (!map[i][j - 1])
						map[i][j] -= 8;
				}

				// If a door is closed and there is a room behind it
				if (!this->roomMap[i][j]->getTopDoor() && i > 0 && this->roomMap[i - 1][j])
					map[i][j]++;
				if (!this->roomMap[i][j]->getRightDoor() && (j + 1) < this->roomMap[i].size() && this->roomMap[i][j + 1])
					map[i][j] += 2;
				if (!this->roomMap[i][j]->getBottomDoor() && (i + 1) < this->roomMap.size() && this->roomMap[i + 1][j])
					map[i][j] += 4;
				if (!this->roomMap[i][j]->getLeftDoor() && j > 0 && this->roomMap[i][j - 1])
					map[i][j] += 8;
			}
		}
	}
	return map;
}

int			LevelGenerator::getStartX(void) { return this->_startX; };
int			LevelGenerator::getStartY(void) { return this->_startY; };
