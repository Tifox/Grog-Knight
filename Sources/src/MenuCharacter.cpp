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
 * File: MenuCharacter.hpp
 * @date 2015-07-23 14:45
 * @author Louis Solofrizzo <louis@ne02ptzero.me>
 */

#include "MenuCharacter.hpp"

MenuCharacter::MenuCharacter(void) : Characters("MenuCharacter") {
	this->addAttribute("type", "Hero");
	this->addAttribute("physic", "1");
	this->addAttribute("hero", "1");
	theSwitchboard.SubscribeTo(this, "enterPressed");
}

MenuCharacter::~MenuCharacter(void) {

}

void	MenuCharacter::init(void) {
	this->AnimCallback("base");
}

void	MenuCharacter::actionCallback(std::string name, int status) {
}

void	MenuCharacter::BeginContact(Elements *elem, b2Contact *contact) {
	contact->SetEnabled(false);
	if (elem->getAttribute("trigger") != "") {
		std::cout << elem->getAttribute("trigger") << std::endl;;
		//this->trigger(name);
	}
}

void	MenuCharacter::EndContact(Elements *elem, b2Contact *contact) {

}

void	MenuCharacter::trigger(std::string name) {

}

void	MenuCharacter::ReceiveMessage(Message *m) {
	Characters::ReceiveMessage(m);
	if (m->GetMessageName() == "enterPressed") {
		if (Game::isPaused == 0 && Game::isInMenu == 1) {
			theSwitchboard.UnsubscribeFrom(this, "enterPressed");
			Game::isInMenu = 0;
			Game::currentGame->start();
		}
	}
}

void	MenuCharacter::AnimCallback(String s) {
	if (s == "base") {
		this->_setCategory("breath");
		if (this->_latOrientation == LEFT) {
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
					this->_getAttr("beginFrame_left").asInt(),
					this->_getAttr("endFrame_left").asInt(), "base");
		} else if (this->_latOrientation == RIGHT) {
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
					this->_getAttr("beginFrame_right").asInt(),
					this->_getAttr("endFrame_right").asInt(), "base");
		}
	}
}

void	MenuCharacter::_forward(int status) {
	this->_setCategory("forward");
	if (this->_forwardFlag == true && status == 1)
		return ;
	this->_forwardFlag = true;

	if (status == 1) {
		if (this->_isRunning == 0 || this->_isRunning == 2) {
			this->_orientation = RIGHT;
			this->_latOrientation = RIGHT;
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
				this->_getAttr("beginFrame").asInt(), this->_getAttr("endFrame").asInt());
		}
		this->_isRunning = 1;
		Game::startRunning(this);
	} else if (status == 0) {
		Game::stopRunning(this);
		this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
		this->_isRunning = 0;
		this->AnimCallback("base");
	} else {
		this->GetBody()->SetLinearVelocity(b2Vec2(this->_getAttr("force").asFloat(), this->GetBody()->GetLinearVelocity().y));
		this->_isRunning = 1;
	}
}

void	MenuCharacter::_backward(int status) {
	this->_setCategory("backward");
	if (this->_backwardFlag == true && status == 1)
		return ;
	this->_backwardFlag = true;

	if (status == 1) {
		if (this->_isRunning == 0 || this->_isRunning == 1) {
			this->_orientation = LEFT;
			this->_latOrientation = LEFT;
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
				this->_getAttr("beginFrame").asInt(), this->_getAttr("endFrame").asInt());
		}
		this->_isRunning = 2;
		Game::startRunning(this);
	} else if (status == 0) {
		Game::stopRunning(this);
		this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
		this->_isRunning = 0;
		this->AnimCallback("base");
	} else {
		this->GetBody()->SetLinearVelocity(b2Vec2(-(this->_getAttr("force").asFloat()), this->GetBody()->GetLinearVelocity().y));
		this->_isRunning = 2;
	}
}
