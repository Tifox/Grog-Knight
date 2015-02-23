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
 * File: Enemy.cpp
 * Creation: 2015-02-23 14:26
 * Manon Budin <mbudin@student.42.fr>
 */

 #include "../inc/Enemy.hpp"

/**
 * Basic Constructor
 */
Enemy::Enemy(void) {
	this->addAttribute("physic", "1");
	this->addAttribute("type", "Enemy");
	this->SetDensity(1.0f);
	this->SetFriction(1.0f);
	this->SetRestitution(0.0f);
	this->SetFixedRotation(true);
	this->SetDrawShape(ADS_Square);
}

/**
 * Collision with another element
 * @param: elem (Elements *)
 */
void	Enemy::callback(Elements * elem) {
	if (elem->getAttributes()["type"] == "Hero") {
		this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		this->_canMove = false;
		this->_invincibility = true;

		if (this->GetBody()->GetWorldCenter().x > elem->GetBody()->GetWorldCenter().x)
			this->ApplyLinearImpulse(Vector2(10, 10), Vector2(0, 0));
		else
			this->ApplyLinearImpulse(Vector2(-10, 10), Vector2(0, 0));
		theSwitchboard.DeferredBroadcast(new Message("canMove"), 1);
		theSwitchboard.DeferredBroadcast(new Message("endInvincibility"), 1.5);
	}
}


/**
 * Receive broadcasts message
 * @param: (Message *)
 */
void	Enemy::ReceiveMessage(Message *m) {
	return;
}



void	Enemy::init(void) {
	this->SetColor(1,1,1,1);
}

/**
 * Basic Destructor
 */
Enemy::~Enemy(void) {
	return;
}

