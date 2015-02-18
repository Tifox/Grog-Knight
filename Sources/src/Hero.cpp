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
	this->addAttribute("spritesFrame", "Resources/Images/Hero/hero_000.png");
	//theSwitchboard.SubscribeTo(this, "callbackEndRun");
	theSwitchboard.SubscribeTo(this, "forwardPress");
	theSwitchboard.SubscribeTo(this, "forwardRealeased");
	theSwitchboard.SubscribeTo(this, "Jump");
}

/**
 * Basic Deconstructor
 */
Hero::~Hero(void) {
	return ;
}

/**
 * Collision with another element
 * @param: elem (Elements *)
 */
void	Hero::callback(Elements * elem) {
	if (elem->getAttribute("type") == "ground") {
		//this->PlaySpriteAnimation(0.5f, SAT_OneShot, 0, 2, "laul");
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

/**
 * Receive broadcasts message
 * @param: (Message *)
 */
void	Hero::ReceiveMessage(Message *m) {
	std::cout << m->GetMessageName() << std::endl;
	if (m->GetMessageName() == "forwardPress") {
		this->ApplyLinearImpulse(Vector2(4, 0), Vector2(1, 1));
		if (this->GetSpriteFrame() <= 3)
			this->PlaySpriteAnimation(0.1f, SAT_Loop, 4, 9, "run");
	} else if (m->GetMessageName() == "forwardRealeased") {
		this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		this->PlaySpriteAnimation(0.1f, SAT_OneShot, 0, 3, "base");
	} else if (m->GetMessageName() == "Jump") {
		this->ApplyLinearImpulse(Vector2(0, 6), Vector2(1, 1));
	}
}

void	Hero::init(void) {
	this->PlaySpriteAnimation(0.2f, SAT_OneShot, 0, 3, "base");
}
