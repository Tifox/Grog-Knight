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

#ifndef __Room__
# define __Room__
# include <iostream>

enum DepthType { CEIL, STD, FLOOR };

enum SpecialType { NONE, ENTRY, BOSS, CHEST, MONSTER, MINIBOSS, ITEM };

class Room {

public:
	Room(int id, int y, int x, int distance, int mapId, bool topDoor, bool leftDoor, bool bottomDoor, bool rightDoor);
	~Room();

	int getX() const;
	int getY() const;
	int getMapId() const;
	int getId() const;
	int getDistance() const;
	int getLinks() const;
	SpecialType getSpecialType() const;
	bool	getTopDoor() const;
	bool	getLeftDoor() const;
	bool	getBottomDoor() const;
	bool	getRightDoor() const;
	Room* 	getTopRoom() const;
	Room*	getLeftRoom() const;
	Room*	getBottomRoom() const;
	Room*	getRightRoom() const;
	void	closeDoor(void);


	void addLink();

	void setTopDoor(bool state);
	void setLeftDoor(bool state);
	void setBottomDoor(bool state);
	void setRightDoor(bool state);
	void setTopRoom(Room* r);
	void setLeftRoom(Room* r);
	void setBottomRoom(Room* r);
	void setRightRoom(Room* r);
	void setDistance(int distance);
	void setSpecialType(SpecialType type);

//private:
	int					_id;			//uniqueid
	int					_y;				//width position
	int					_x;				//height position
	int					_mapId;			//map prefab id
	DepthType			_depthType;		//depth in level (Ceil => y = 0 / Floor => y = mapWidth / Std = other width)
	SpecialType			_specialType;	//special type of room
	int					_distance;		//distance from starting point for shockwave algorythm
	int					_links;			//number of links between this room and surrounding ones
	bool				_topDoor;		//has top door
	bool				_leftDoor;		//has left door
	bool				_bottomDoor;	//has bottom door
	bool				_rightDoor;		//has right door
	Room*				_leftRoom;
	Room*				_bottomRoom;
	Room*				_topRoom;
	Room*				_rightRoom;
	Room(void);
};

#endif
