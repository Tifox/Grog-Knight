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
Hero::Hero(void) : Characters("Hero") {
	theSwitchboard.SubscribeTo(this, "canMove");
	theSwitchboard.SubscribeTo(this, "endInvincibility");
	return ;
}

/**
 * Basic Destructor
 */
Hero::~Hero(void) {
	return ;
}

/**
 * Init Animation
 */
void	Hero::init(void) {
	this->AnimCallback("base");
}


/**
 * Mother's callback for actions
 * @param: name (std::string)
 * @param: status (int)
 */
void	Hero::actionCallback(std::string name, int status) {
	return ;
}

void	Hero::BeginContact(Elements* elem, b2Contact *contact) {
	Characters::BeginContact(elem, contact);
	if (elem->getAttributes()["type"] == "Enemy") {
		if (this->_invincibility == true)
			return;
		this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		Game::stopRunning(this);
		this->_isRunning = 0;
		//Damage here
		this->_canMove = false;
		this->_invincibility = true;
		theSwitchboard.DeferredBroadcast(new Message("canMove"), 1);
		theSwitchboard.DeferredBroadcast(new Message("endInvincibility"), 1);
		if (this->GetBody()->GetWorldCenter().x >= elem->GetBody()->GetWorldCenter().x) {
			this->ApplyLinearImpulse(Vector2(10, 10), Vector2(0, 0));
		}
		else if (this->GetBody()->GetWorldCenter().x < elem->GetBody()->GetWorldCenter().x) {
			this->ApplyLinearImpulse(Vector2(-10, 10), Vector2(0, 0));
		}
	}
}
