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
	std::string 	orientation;
	float				x = 2, y = 1;
	if (name == "attack" && status == 0 && this->_weapon->attackReady() == 1 &&
		this->_fullChargedAttack == false && this->_isLoadingAttack == 0 &&
		this->_isAttacking == 1) {
		this->_weapon->isAttacking(0);
		if (this->_orientation == RIGHT) {
			orientation = "right";
		} else if (this->_orientation == LEFT) {
			orientation = "left";
		} else if (this->_orientation == UP) {
			x = 1.5f; y = 2;
			orientation = "up";
		} else if (this->_orientation == DOWN) {
			x = 1; y = 2.5f;
			orientation = "down";
		}
		this->changeSizeTo(Vector2(x, y));
		this->_setCategory("attack");
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
								  this->_getAttr("beginFrame_" +
												 orientation).asInt(),
								  this->_getAttr("endFrame_" +
												 orientation).asInt(), "base");

	} else if (name == "attack" && status == 0 &&
			   this->_weapon->attackReady() == 1 &&
			   this->_fullChargedAttack == true) {
		if (this->_orientation == RIGHT) {
			orientation = "right";
		} else if (this->_orientation == LEFT) {
			orientation = "left";
		} else if (this->_orientation == UP) {
			x = 1.5f; y = 2;
			orientation = "up";
		} else if (this->_orientation == DOWN) {
			x = 1; y = 2.5f;
			orientation = "down";
		}
		this->_setCategory("loadAttack_done");
		this->_weapon->isAttacking(0);
		this->_isLoadingAttack = 0;
		this->_fullChargedAttack = false;
		this->changeSizeTo(Vector2(2, 2));
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
								  this->_getAttr("beginFrame_" + orientation).asInt(),
								  this->_getAttr("endFrame_" + orientation).asInt(), "base");
	} else if (name == "loadAttack_charge") {
		if (this->_latOrientation == RIGHT) {
			orientation = "right";
		} else if (this->_latOrientation == LEFT)
			orientation = "left";
		this->_setCategory("loadAttack_charge");
		this->changeSizeTo(Vector2(2, 2));
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
								  this->_getAttr("beginFrame_" + orientation).asInt(),
								  this->_getAttr("endFrame_" + orientation).asInt());
	}
	return ;
}

//! Begin collision function
/**
 * Collision begin callback
 * /!\ This function is called just before a collision
 * @param elem The other Element
 * @param contact The Box2D contact object
 */
void	Hero::BeginContact(Elements* elem, b2Contact *contact) {
	Characters::BeginContact(elem, contact);

	if (elem->getAttribute("type") == "Enemy" && elem->isDead() == false) {
		if (this->_invincibility == false)
			this->_takeDamage(elem);
		else {
			this->_enemiesTouched.push_back(elem);
		}
	}
	else if (elem->getAttribute("type") == "Object") {
		if (elem->getAttribute("type2") == "Consumable") {
			if (elem->getAttribute("type3") == "HP") {
				if (this->_hp != this->_maxHp) {
					Game::addToDestroyList(elem);
					this->setHP(this->getHP() + stoi(elem->getAttribute("value")));
					Game::getHUD()->life(this->getHP());
				}
			} else if (elem->getAttribute("type3") == "mana") {
				if (this->_mana < this->_maxMana) {
					Game::addToDestroyList(elem);
					this->setMana(this->getMana() + stoi(elem->getAttribute("value")));
					Game::getHUD()->mana(this->getMana());
				}
			}
			if (elem->getAttribute("type3") == "gold") {
				Game::addToDestroyList(elem);
				this->_gold += stoi(elem->getAttribute("value"));
				Game::currentGame->tooltip->tip(elem, this);
				Game::getHUD()->updateGold(this->getGold());
			}
		}
		else if (elem->getAttribute("type2") == "Equipment") {
			this->_item = elem;
		}
	}
	if (elem->getAttribute("type") == "ground" &&
		elem->getAttribute("speType") == "spikes") {
		if (this->_invincibility == false)
			this->_takeDamage(elem);
		else
			this->_enemiesTouched.push_back(elem);
	}
   /* if (elem->getAttribute("speType") == "water")*/
		/*this->GetBody()->SetGravityScale(0.3);*/
}

//! End collision function
/**
 * End contact callback.
 * /!\ This function is actually called after a collision.
 * @param elem The other Element.
 * @param contact The Box2D contact object
 */
void	Hero::EndContact(Elements *elem, b2Contact *contact) {
	Characters::EndContact(elem, contact);
		if (elem->getAttribute("type") == "Object") {
			if (elem->getAttributes()["type2"] == "Equipment") {
				this->_item = nullptr;
			}
		}
		if (elem->getAttribute("type") == "Enemy" ||
			elem->getAttribute("speType") == "spikes") {
			this->_enemiesTouched.remove(elem);
		}
   /* if (elem->getAttribute("speType") == "water")*/
		/*this->GetBody()->SetGravityScale(1);*/
}

//! Function called when the hero is taking damage
/**
 * Called by BeginContact mostly
 * @param the elem that has damaged hero
 * @todo monster damage should not be hard-written to 25
 */
void	Hero::_takeDamage(Elements* elem) {
  this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
  Game::stopRunning(this);
  this->_isRunning = 0;
  this->_isJump = 1;
  this->changeSizeTo(Vector2(1, 1));
  if (this->_invincibility == false) {
	this->changeCanMove();
	this->setHP(this->getHP() - 25);
	theSwitchboard.DeferredBroadcast(new Message("canMove"), 0.4f);
	theSwitchboard.DeferredBroadcast(new Message("endInvincibility"), 1.5f);
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
