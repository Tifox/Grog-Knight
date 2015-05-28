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
* File: LevelGenerator.hpp
* Creation: 2015-03-02 16:05
* Matthieu Maudet <mmaudet@student.42.fr>
*/

#ifndef __LevelGenerator__
# define __LevelGenerator__

# include <vector>
# include <cstdlib>
# include <ctime>
# include <iostream>

# include "Room.hpp"

class LevelGenerator {

public:
	LevelGenerator(int maxMapSize, int minPathLenght, int doorsPopRate);
	~LevelGenerator();

	void							print(void);
	void							execute(void);			//execute generation process
	void							createFirstRoom(void);
	void							firstPass(int distance);		//generate base rooms
	void							generateRoomsBehindDoors(Room* room);
	Room*							generateRoom(int x, int y, int distance, Room* prevRoom, int origin);
	bool							testDoor(int x, int y);
	bool 							testRoom(int x, int y);
	void							linkAdjacentRooms(Room *room);
	void							shockwave(void);
	int								getEntryId(void);
	int								id(void);
	std::vector<std::vector<int> >	getLevel(void);
	std::vector<Room*>				*_rooms;
	int								getStartX();
	int								getStartY();

private:
	int								_id;
	int								_height;
	int								_width;
	int								_nbMaps;
	int								_minPathLenght;
	int								_maxMapSize;
	int								_doorsPopRate;
	int								_startX;
	int								_startY;

};

#endif
