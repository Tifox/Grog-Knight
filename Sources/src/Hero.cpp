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

# include "Hero.hpp"

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
	if (name == "attack" && status == 1 && this->_weapon->attackReady() == 1) {
		this->_weapon->isAttacking(0);
		// To be used later
		//this->changeSizeTo(Vector2(1.5f, 1));
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
								  this->_getAttr("beginFrame").asInt(),
								  this->_getAttr("endFrame").asInt(), "base");
	}
	return ;
}

/**
 * Collision begin callback
 * @param: elem (Elements *)
 * @param: contact (b2Contact *)
 * @note: This function is called just before a collision
 */
void	Hero::BeginContact(Elements* elem, b2Contact *contact) {
	Characters::BeginContact(elem, contact);
	if (elem->getAttributes()["type"] == "Enemy") {
		this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		Game::stopRunning(this);
		this->_isRunning = 0;
		this->_isJump = 1;
		if (this->_invincibility == false) {
			this->changeCanMove();
			this->setHP(this->getHP() - 25);
			theSwitchboard.DeferredBroadcast(new Message("canMove"), 0.5f);
			theSwitchboard.DeferredBroadcast(new Message("endInvincibility"), 1);
			Game::getHUD()->life(this->getHP());
		}
		if (this->GetBody()->GetWorldCenter().x >= elem->GetBody()->GetWorldCenter().x) {
			this->ApplyLinearImpulse(Vector2(4, 4), Vector2(0, 0));
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
									  this->_getAttr("takeDamage", "beginFrame_right").asInt(),
									  this->_getAttr("takeDamage", "endFrame_right").asInt(),
									  "takeDamage");
		}
		else if (this->GetBody()->GetWorldCenter().x < elem->GetBody()->GetWorldCenter().x) {
			this->ApplyLinearImpulse(Vector2(-4, 4), Vector2(0, 0));
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
									  this->_getAttr("takeDamage", "beginFrame_left").asInt(),
									  this->_getAttr("takeDamage", "endFrame_left").asInt(),
									  "takeDamage");
		}
		this->SetColor(1,0,0,0.8f);
		theSwitchboard.SubscribeTo(this, "colorDamageBlink1");
		theSwitchboard.SubscribeTo(this, "colorDamageBlink2");
		theSwitchboard.DeferredBroadcast(new Message("colorDamageBlink1"), 0.1f);
		this->_invincibility = true;
	}
	else if (elem->getAttributes()["type"] == "Object") {
		if (elem->getAttributes()["type2"] == "Consumable") {
			if (elem->getAttributes()["type3"] == "HP") {
				if (this->_hp != this->_maxHp) {
					Game::addToDestroyList(elem);
					this->setHP(this->getHP() + 50);
					Game::getHUD()->life(this->getHP());
				}
			}
		}
		else if (elem->getAttributes()["type2"] == "Equipment") {
			this->_item = elem;
		}
	}
}

void	Hero::EndContact(Elements *elem, b2Contact *contact) {
	Characters::EndContact(elem, contact);
	if (elem->getAttributes()["type"] == "Object") {
		if (elem->getAttributes()["type2"] == "Equipment") {
			this->_item = nullptr;
		}
	}
}
