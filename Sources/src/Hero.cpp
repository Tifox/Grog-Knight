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
	//this->addAttribute("sprite", "Resources/Images/test.png");
	this->addAttribute("physic", "1");
	this->addAttribute("type", "Hero");
	this->SetDensity(1.0f);
	this->SetFriction(1.0f);
	this->SetRestitution(0.0f);
	this->SetFixedRotation(true);
	this->_jumping = MAX_JUMP;
	this->_invincibility = false;
	this->_canMove = true;
	this->_orientation = 1;
	this->_up = 0;
	theSwitchboard.SubscribeTo(this, "MeleeAttack");
	theSwitchboard.SubscribeTo(this, "RangedAttack");
	theSwitchboard.SubscribeTo(this, "SmashAttack");
	theSwitchboard.SubscribeTo(this, "Jump");
	theSwitchboard.SubscribeTo(this, "Smash");
	theSwitchboard.SubscribeTo(this, "canMove");
	theSwitchboard.SubscribeTo(this, "endInvincibility");
	theSwitchboard.SubscribeTo(this, "forwardPressed");
	theSwitchboard.SubscribeTo(this, "backPressed");
	theSwitchboard.SubscribeTo(this, "upPressed");
	theSwitchboard.SubscribeTo(this, "downPressed");
	theSwitchboard.SubscribeTo(this, "forwardRealeased");
	theSwitchboard.SubscribeTo(this, "backReleased");
	theSwitchboard.SubscribeTo(this, "upReleased");
	theSwitchboard.SubscribeTo(this, "downReleased");
	this->addAttribute("spritesFrame", "Resources/Images/Hero/hero_000.png");
	//theSwitchboard.SubscribeTo(this, "callbackEndRun");
	theSwitchboard.SubscribeTo(this, "Jump");
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
	else if (elem->getAttributes()["type"] == "enemy") {
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
void	Hero::ReceiveMessage(Message *m) {
	if (m->GetMessageName() == "canMove")
		this->_canMove = true;
	else if (m->GetMessageName() == "endInvincibility")
		this->_invincibility = false;

	if (this->_canMove == false)
		return;
	else if (m->GetMessageName() == "Jump") {
		this->ApplyLinearImpulse(Vector2(0, 10), Vector2(0, 0));
	}

	else if (m->GetMessageName() == "Smash") {
		this->ApplyLinearImpulse(Vector2(0, -100), Vector2(0, 0));
	}

	else if (m->GetMessageName() == "forwardPressed") {
		this->_orientation = 1;
		this->_up = 0;
		if (this->GetBody()->GetLinearVelocity().x < MAX_RUN_SPEED) {
			this->ApplyLinearImpulse(Vector2(RUN_SPEED, 0), Vector2(0, 0));
		}
		if (this->GetSpriteFrame() <= 3)
			this->PlaySpriteAnimation(0.1f, SAT_Loop, 4, 9, "run");
	}
	else if (m->GetMessageName() == "forwardRealeased") {
		this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		this->PlaySpriteAnimation(0.1f, SAT_OneShot, 0, 3, "base");
	}

	else if (m->GetMessageName() == "backPressed") {
		this->_orientation = -1;
		this->_up = 0;
		if (this->GetBody()->GetLinearVelocity().x > -MAX_RUN_SPEED) {
			this->ApplyLinearImpulse(Vector2(-RUN_SPEED, 0), Vector2(0, 0));
		}
		if (this->GetSpriteFrame() <= 3)
			this->PlaySpriteAnimation(0.1f, SAT_Loop, 4, 9, "run");
	}
	else if (m->GetMessageName() == "backReleased") {
		this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		this->PlaySpriteAnimation(0.1f, SAT_OneShot, 0, 3, "base");
	}

	else if (m->GetMessageName() == "upPressed") {
		this->_orientation == 0;
		this->_up = 1;
	}

	else if (m->GetMessageName() == "downPressed") {
		this->_orientation == 0;
		this->_up = -1;
	}
}

/**
 * Callback for end of animation
 * @param: name (String)
 */
void	Hero::AnimCallback(String name) {
	std::cout << name << std::endl;
	if (name == "base") {
		this->PlaySpriteAnimation(0.2f, SAT_Loop, 0, 3, "base");
 	} else if (name == "run") {
		this->PlaySpriteAnimation(0.1f, SAT_OneShot, 4, 9, "run");
	}
}

void	Hero::init(void) {
	this->PlaySpriteAnimation(0.2f, SAT_OneShot, 0, 3, "base");
}
