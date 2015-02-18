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
 * File: Hero.cpp
 * Creation: 2015-02-14 10:49
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "../inc/Hero.hpp"

/**
 * Basic constructor
 */
Hero::Hero(void) {
	this->addAttribute("sprite", "Resources/Images/hero.png");
	this->addAttribute("physic", "1");
	this->addAttribute("type", "Hero");
	this->SetDensity(1.0f);
	this->SetFriction(1.0f);
	this->SetRestitution(0.0f);
	this->SetFixedRotation(true);
	this->jumping = MAX_JUMP;
	this->invincibility = false;
	this->canMove = true;
	theSwitchboard.SubscribeTo(this, "MeleeAttack");
	theSwitchboard.SubscribeTo(this, "RangedAttack");
	theSwitchboard.SubscribeTo(this, "SmashAttack");
	theSwitchboard.SubscribeTo(this, "Jump");
	theSwitchboard.SubscribeTo(this, "Smash");
	theSwitchboard.SubscribeTo(this, "canMove");
	theSwitchboard.SubscribeTo(this, "endInvincibility");
	theSwitchboard.SubscribeTo(this, "ForPressed");
	theSwitchboard.SubscribeTo(this, "BackPressed");
	theSwitchboard.SubscribeTo(this, "UpPressed");
	theSwitchboard.SubscribeTo(this, "DownPressed");
	theSwitchboard.SubscribeTo(this, "ForReleased");
	theSwitchboard.SubscribeTo(this, "BackReleased");
	theSwitchboard.SubscribeTo(this, "UpReleased");
	theSwitchboard.SubscribeTo(this, "DownReleased");
}

/**
 * Basic Destructor
 */
Hero::~Hero(void) {
	return ;
}

/**
 * Collision with another element
 * @param: elem (Elements *)
 */
void	Hero::callback(Elements * elem) {
	if (elem->getAttributes()["type"] == "wall" ||
		elem->getAttributes()["type"] == "ground" ||
		elem->getAttributes()["type"] == "corner") {
		if (this->GetBody()->GetWorldCenter().y > elem->GetBody()->GetWorldCenter().y + HERO_SIZE) {
			//touching ground
			this->_jumping = MAX_JUMP;
		}
		else {
			//Touching wall
		}
	}
}

void	Hero::ReceiveMessage(Message *m) {
	if (this->_canMove == false)
		return;
	else if (m->GetMessageName() == "Jump") {

	}

	else if (m->GetMessageName() == "ForPressed") {

	}
	else if (m->GetMessageName() == "ForReleased") {

	}

	else if (m->GetMessageName() == "BackPressed") {

	}
	else if (m->GetMessageName() == "BackReleased") {

	}

	else if (m->GetMessageName() == "UpPressed" || m->GetMessageName() == "UpReleased") {
		this->_orientation == 0;
		this->_up = 1;
	}

	else if (m->GetMessageName() == "DownPressed" || m->GetMessageName() == "DownReleased") {
		this->_orientation == 0;
		this->_up = -1;
	}
}
