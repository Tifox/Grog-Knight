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

# include "Room.hpp"

/**
* Basic constructor
*/
Room::Room(int id, int y, int x, int distance, int mapId, bool topDoor, bool leftDoor, bool bottomDoor, bool rightDoor) : _id(id), _y(y), _x(x), _distance(distance), _mapId(mapId), _topDoor(topDoor), _leftDoor(leftDoor), _bottomDoor(bottomDoor), _rightDoor(rightDoor) {
	_links = 0;
	_specialType = NONE;
	return;
}

/**
* Basic Destructor
*/
Room::~Room(void) {
	return;
}

int Room::getX(void) const {
	return this->_x;
}

int Room::getY(void) const {
	return this->_y;
}

int Room::getMapId(void) const {
	return this->_mapId;
}

int Room::getId(void) const {
	return this->_id;
}

int Room::getDistance(void) const {
	return this->_distance;
}

int Room::getLinks(void) const {
	return this->_links;
}

SpecialType Room::getSpecialType(void) const {
	return this->_specialType;
}

bool Room::getTopDoor(void) const {
	return this->_topDoor;
}

bool Room::getLeftDoor(void) const {
	return this->_leftDoor;
}


bool Room::getBottomDoor(void) const {
	return this->_bottomDoor;
}


bool Room::getRightDoor(void) const {
	return this->_rightDoor;
}

Room* Room::getTopRoom(void) const {
	return this->_topRoom;
}

Room* Room::getLeftRoom(void) const {
	return this->_leftRoom;
}


Room* Room::getBottomRoom(void) const {
	return this->_bottomRoom;
}


Room* Room::getRightRoom(void) const {
	return this->_rightRoom;
}

void Room::addLink(void) {
	this->_links++;
}

void Room::setTopDoor(bool state) {
	this->_topDoor = state;
}

void Room::setLeftDoor(bool state) {
	this->_leftDoor = state;
}

void Room::setBottomDoor(bool state) {
	this->_bottomDoor = state;
}

void Room::setRightDoor(bool state) {
	this->_rightDoor = state;
}

void Room::setTopRoom(Room* r) {
	this->_topRoom = r;
}

void Room::setLeftRoom(Room* r) {
	this->_leftRoom = r;
}

void Room::setBottomRoom(Room* r) {
	this->_bottomRoom = r;
}

void Room::setRightRoom(Room* r) {
	this->_rightRoom = r;
}

void Room::setDistance(int distance) {
	this->_distance = distance;
}

void Room::setSpecialType(SpecialType type) {
	this->_specialType = type;
}
