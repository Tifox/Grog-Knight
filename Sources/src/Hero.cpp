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

//! Constructor
/**
 * This constructor is making some subscribtions for himself.
 */
Hero::Hero(void) : Characters("Hero") {
	theSwitchboard.SubscribeTo(this, "canMove");
	theSwitchboard.SubscribeTo(this, "endInvincibility");
	theSwitchboard.SubscribeTo(this, "enableAttackHitbox");
	theSwitchboard.SubscribeTo(this, "disableAttackHitbox");
	return ;
}

//! Destructor
/**
 * Basic Destructor
 */
Hero::~Hero(void) {
	return ;
}

//! Init Animation function
/**
 * This function making the first animation call.
 */
void	Hero::init(void) {
	this->AnimCallback("base");
}

//! Action callback function
/**
 * Mother's callback for actions
 * See Characters::ReceiveMessage for more information.
 * @param name The action name
 * @param status The key status (1 | 0)
 * @sa Characters::ReceiveMessage
 * @todo Extract the values from the consumable
 */
void	Hero::actionCallback(std::string name, int status) {
	if (name == "attack" && status == 1 && this->_weapon->attackReady() == 1) {
		this->_weapon->isAttacking(0);
		if (this->_orientation == RIGHT) {
			this->changeSizeTo(Vector2(2, 1));
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
									  this->_getAttr("beginFrame_right").asInt(),
									  this->_getAttr("endFrame_right").asInt(), "base");
		}
		else if (this->_orientation == LEFT) {
			this->changeSizeTo(Vector2(2, 1));
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
									  this->_getAttr("beginFrame_left").asInt(),
									  this->_getAttr("endFrame_left").asInt(), "base");
		}
		else if (this->_orientation == UP) {
			this->changeSizeTo(Vector2(1.5f, 2));
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
									  this->_getAttr("beginFrame_up").asInt(),
									  this->_getAttr("endFrame_up").asInt(), "base");
		}
		else if (this->_orientation == DOWN) {
			this->changeSizeTo(Vector2(1, 2.5f));
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
									  this->_getAttr("beginFrame_down").asInt(),
									  this->_getAttr("endFrame_down").asInt(), "base");
		}
	}
	return ;
}

//! Begin collision function
/**
 * Collision begin callback
 * /!\ This function is called just before a collision
 * @param: elem The other Element
 * @param: contact The Box2D contact object
 */
void	Hero::BeginContact(Elements* elem, b2Contact *contact) {
	Characters::BeginContact(elem, contact);
	if (elem->getAttributes()["type"] == "Enemy") {
		this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		Game::stopRunning(this);
		this->_isRunning = 0;
		this->_isJump = 1;
		this->changeSizeTo(Vector2(1, 1));
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
									  this->_getAttr("takeDamage", "beginFrame_left").asInt(),
									  this->_getAttr("takeDamage", "endFrame_left").asInt(),
									  "takeDamage");
		}
		else if (this->GetBody()->GetWorldCenter().x < elem->GetBody()->GetWorldCenter().x) {
			this->ApplyLinearImpulse(Vector2(-4, 4), Vector2(0, 0));
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
									  this->_getAttr("takeDamage", "beginFrame_right").asInt(),
									  this->_getAttr("takeDamage", "endFrame_right").asInt(),
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
					this->setHP(this->getHP() + 25);
					Game::getHUD()->life(this->getHP());
				}
			}
			if (elem->getAttributes()["type3"] == "gold") {
				Game::addToDestroyList(elem);
				this->_gold += 40;
				Game::getHUD()->updateGold(this->getGold());
			}
		}
		else if (elem->getAttributes()["type2"] == "Equipment") {
			this->_item = elem;
		}
	}
}

//! End collision function
/**
 * End contact callback.
 * /!\ This function is actually called after a collision.
 * @param: elem The other Element.
 * @param contact The Box2D contact object
 */
void	Hero::EndContact(Elements *elem, b2Contact *contact) {
	Characters::EndContact(elem, contact);
	if (elem->getAttributes()["type"] == "Object") {
		if (elem->getAttributes()["type2"] == "Equipment") {
			this->_item = nullptr;
		}
	}
}
