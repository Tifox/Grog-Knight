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
 * File: ControllerInputManager.cpp
 * Creation: 2015-09-10 09:48
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "ControllerInputManager.hpp"

ControllerInputManager::ControllerInputManager(void) {
	;
}

ControllerInputManager::~ControllerInputManager(void) {
	;
}

void	ControllerInputManager::ReceiveMessage(Message *m) {

}

void	ControllerInputManager::tick(void) {
	Vec2i	left = theController.GetLeftThumbstick();
	std::string		direction = "";

	if (left.Y > 0 && left.X > -16000 && left.X < 16000) {
		direction = "up";
	} else if (left.Y < 0 && left.X > -16000 && left.X < 16000) {
		direction = "down";
	} else if (left.X > 0 && left.Y > -16000 && left.Y < 16000) {
		direction = "forward";
	} else if (left.X < 0 && left.Y > -16000 && left.Y < 16000) {
		direction = "backward";
	}

	if (direction == "" || this->_lastDirection != direction) {
		theSwitchboard.Broadcast(new Message(this->_lastDirection + "Released"));
	}
	if (direction != "") {
		theSwitchboard.Broadcast(new Message(direction + "Pressed"));
		this->_lastDirection = direction;
	}
}
