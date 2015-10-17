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
	std::list<std::string>::iterator		it;

	this->_buttons = {"A", "B", "X", "Y", "START", "BACK", "RB"};
	for (it = this->_buttons.begin(); it != this->_buttons.end(); it++) {
		theSwitchboard.SubscribeTo(this, "button" + (*it) + "Pressed");
		theSwitchboard.SubscribeTo(this, "button" + (*it) + "Released");
	}
	this->_parseBindings();
}

ControllerInputManager::~ControllerInputManager(void) {
	;
}

void	ControllerInputManager::ReceiveMessage(Message *m) {
	int		i, isUpper;
	std::string		message;

	for (i = 6, message = m->GetMessageName(); (message[i] >= 65 && message[i] <= 90) 
		&& i < message.size(); i++);
	std::string button = m->GetMessageName().substr(6, i - 7);
	std::string status = m->GetMessageName().substr(7 + (i - 8), m->GetMessageName().size() - (i - 6));

	if (button == "BACK") {
		std::cout << "here" << std::endl;
		theController.SetLeftVibration(250);
		theController.SetRightVibration(250);
	}
	if (Game::isPaused == 1 || Game::isInMenu != 0) {
		if (button == "A")
			theSwitchboard.Broadcast(new Message("enterPressed"));
		else if (button == "B")
			theSwitchboard.Broadcast(new Message("deletePressed"));
	}
	for (isUpper = i = 0; i < message.size(); i++) {
		if (message[i] >= 65 && message[i] <= 90)
			isUpper++;
	}
	if (isUpper)
		theSwitchboard.Broadcast(new Message(this->_bindings[button] + status));
	theSwitchboard.Broadcast(new Message(this->_bindings[button]));
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

	if ((direction == "" || this->_lastDirection != direction) && this->flag == 0) {
		theSwitchboard.Broadcast(new Message(this->_lastDirection + "Released"));
	}
	if (direction != "" && this->flag == 0) {
		theSwitchboard.Broadcast(new Message(direction + "Pressed"));
		this->_lastDirection = direction;
		this->flag = 1;
	}
}

void	ControllerInputManager::addBindings(std::string button, std::string broadcast) {
	this->_bindings[button] = broadcast;
}

void	ControllerInputManager::_parseBindings(void) {
	std::stringstream 	buffer;
	std::ifstream		fd;
	Json::Reader	read;
	Json::Value		json;
	Json::ValueIterator i, v, j;
	std::string broadcast, controller;

	fd.open("Config/Bindings.json");
	if (!fd.is_open())
		Log::error("Can't find Config/Settings.json");
	buffer << fd.rdbuf();

	if (!read.parse(buffer, json))
		Log::error("Error in json syntax :\n" + read.getFormattedErrorMessages());

	for (i = json.begin(); i != json.end(); i++) {
		for (v = (*i).begin(); v != (*i).end(); v++) {
			broadcast = controller = "";
			for (j = (*v).begin(); j != (*v).end(); j++) {
				if (j.key().asString() == "broadcast")
					broadcast = (*j).asString();
				else if (j.key().asString() == "controller")
					controller = (*j).asString();
			}
			if (controller != "" && broadcast != "")
				this->addBindings(controller, broadcast);
		}
	}

}
