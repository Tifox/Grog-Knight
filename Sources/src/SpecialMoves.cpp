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
 * File: SpecialMoves.cpp
 * Creation: 2015-06-30 09:57
 * Matthieu Maudet<arkhane84@gmail.com>
 */

 # include "SpecialMoves.hpp"
 # include <cstdlib>



 //! Base constructor
 SpecialMoves::SpecialMoves(void) {

 }

 //! Main constructor
 SpecialMoves::SpecialMoves(Characters* charac) : character(charac) {

 }

 //! Basic destructor
 SpecialMoves::~SpecialMoves(void) {
 }

 //! Set the current working category
 /**
  * This function is made for gain time and money, set first the category in this functions
  * then call the function _getAttr(std::string key).
  * See _getAttr(std::string key) for more info.
  * @param category The name of the category
  * @sa SpecialMoves::_getAttr
  */
 void	SpecialMoves::_setCategory(std::string category) {
 	this->character->_category = category;
 }


 //! Special move: dash
 /**
  * Character executes a dash if the cooldown is up and the conditions allows it
  * Properties of dash - no gravity, take damage, cant move
  * @sa SpecialMoves::_specialMove()
  */

 void	SpecialMoves::_dash(void) {
 	this->character->_setCategory("dash");
 	if (this->character->_isAttacking == 0 && this->character->_canMove == 1 && this->character->_hasDashed == 0 &&
 			this->character->_isDashing == false) {
 		this->character->_isDashing = true;
 		this->character->GetBody()->SetGravityScale(0);
 		this->character->actionCallback("dash", 0);
 		this->character->_canMove = 0;
 		if (this->character->_grounds.size() == 0)
 			this->character->_hasDashed = 1;
 		theSwitchboard.SubscribeTo(this->character, "dashEnd");
 		theSwitchboard.DeferredBroadcast(new Message("dashEnd"),
 				this->character->_getAttr("uptime").asFloat());
 		if (this->character->_latOrientation == Characters::LEFT)
 			this->character->GetBody()->SetLinearVelocity(b2Vec2(-this->character->_getAttr("dashSpeed").asInt(), 0));
 		else if (this->character->_latOrientation == Characters::RIGHT)
 			this->character->GetBody()->SetLinearVelocity(b2Vec2(this->character->_getAttr("dashSpeed").asInt(), 0));
 	}
 }

 //! Special move: charge
 /**
  * Character executes a charge if the cooldown is up and the conditions allows it
  * Properties of charge - invincibility, can move
  * @sa SpecialMoves::_specialMove()
  */

 void	SpecialMoves::_charge(void) {
 	this->character->_setCategory("charge");
 	if (this->character->_isAttacking == 0 && this->character->_canMove == 1 && this->character->_speMoveReady == 1 && this->character->_grounds.size() > 0) {
 		this->character->_speMoveReady = 0;
 		this->character->_invincibility = true;
 		this->character->_isCharging = true;
 		this->character->_canMove = 0;
 		theSwitchboard.SubscribeTo(this->character, "speMoveReady");
 		theSwitchboard.SubscribeTo(this->character, "chargeEnd");
 		theSwitchboard.DeferredBroadcast(new Message("speMoveReady"),
 				this->character->_getAttr("cooldown").asFloat());
 		theSwitchboard.DeferredBroadcast(new Message("chargeEnd"),
 				this->character->_getAttr("uptime").asFloat());
 		if (this->character->_latOrientation == Characters::LEFT)
 			this->character->GetBody()->SetLinearVelocity(b2Vec2(-this->character->_getAttr("chargeSpeed").asInt(), 0));
 		else if (this->character->_latOrientation == Characters::RIGHT)
 			this->character->GetBody()->SetLinearVelocity(b2Vec2(this->character->_getAttr("chargeSpeed").asInt(), 0));
 	}
 }

 //! Special move: stomp
 /**
  * If airborne, will allow you to slam the ground and deal damage to enemies
  * Properties of stomp - invincibility, can move
  * @sa SpecialMoves::_specialMove()
  */

 void	SpecialMoves::_stomp(void) {
 	this->character->_setCategory("stomp");
 	if (this->character->_isAttacking == 0 && this->character->_canMove == 1 && this->character->_speMoveReady == 1
 			&& this->character->_grounds.size() == 0) {
 		this->character->_speMoveReady = 0;
 		this->character->GetBody()->SetBullet(true);
 		this->character->_invincibility = true;
 		this->character->_isStomping = true;
 		this->character->_isCharging = true;
 		theSwitchboard.SubscribeTo(this->character, "speMoveReady");
 		theSwitchboard.SubscribeTo(this->character, "stompEnd");
 		theSwitchboard.DeferredBroadcast(new Message("speMoveReady"),
 				this->character->_getAttr("cooldown").asFloat());
 		this->character->GetBody()->SetLinearVelocity(b2Vec2(0, -this->character->_getAttr("stompSpeed").asInt()));
 	}
 }


 //! Special move: blink
 /**
  * Teleports the player in the direction he's facing
  * Properties of blink - instant
  * @sa SpecialMoves::_specialMove()
  */

 void	SpecialMoves::_blink(void) {
 	this->character->_setCategory("blink");
 	Map m = Game::currentGame->maps->getMapXY()[Game::currentY][Game::currentX];
 	int x = (this->character->GetBody()->GetWorldCenter().x) - m.getXStart();
 	int y = -((this->character->GetBody()->GetWorldCenter().y) - m.getYStart());
 	int range = this->character->_getAttr("blinkRange").asInt();
 	std::vector<std::vector<int>> t = m.getPhysicMap();

 	if (this->character->_isAttacking == 0 && this->character->_canMove == 1 && this->character->_speMoveReady == 1) {
 		this->character->_speMoveReady = 0;
 		theSwitchboard.SubscribeTo(this->character, "speMoveReady");
 		theSwitchboard.DeferredBroadcast(new Message("speMoveReady"),
 				this->character->_getAttr("cooldown").asFloat());
 		if (this->character->_orientation == Characters::UP) {
 			while (range > 0) {
 				if (y - range > 0 && !t[y - range][x])
 					break;
 				range--;
 			}
 			if (range > 0)
 				this->character->GetBody()->SetTransform(b2Vec2(this->character->GetBody()->GetWorldCenter().x,
 							this->character->GetBody()->GetWorldCenter().y + range), 0);
 		}
 		else if (this->character->_orientation == Characters::DOWN) {
 			while (range > 0) {
 				if (y + range < (t.size() - 2) && !t[y + range][x])
 					break;
 				range--;
 			}
 			range--;
 			if (range > 0)
 				this->character->GetBody()->SetTransform(b2Vec2(this->character->GetBody()->GetWorldCenter().x,
 							this->character->GetBody()->GetWorldCenter().y - range), 0);
 		}
 		else if (this->character->_orientation == Characters::RIGHT) {
 			while (range > 0) {
 				if (x + range < t[y].size() && !t[y][x + range])
 					break;
 				range--;
 			}
 			if (range > 0)
 				this->character->GetBody()->SetTransform(b2Vec2(this->character->GetBody()->GetWorldCenter().x + range,
 							this->character->GetBody()->GetWorldCenter().y), 0);
 		}
 		else if (this->character->_orientation == Characters::LEFT) {
 			while (range > 0) {
 				if (x - range > 0 && !t[y][x - range])
 					break;
 				range--;
 			}
 			if (range > 0)
 				this->character->GetBody()->SetTransform(b2Vec2(this->character->GetBody()->GetWorldCenter().x - range,
 							this->character->GetBody()->GetWorldCenter().y), 0);
 		}
 		if (range > 0) {
 			b2PolygonShape box = Game::hList->getHitbox(this->character->_hitbox);
 			b2Shape *shape = &box;
 			this->character->GetBody()->DestroyFixture(this->character->GetBody()->GetFixtureList());
 			this->character->GetBody()->CreateFixture(shape, 1);
 		}
 	}
 }

 //! Special move: fly
 /**
  * Toggles fly mode on. When it's on, the jump button makes the character fly
  * Properties of fly - toggle
  * @sa SpecialMoves::_specialMove()
  */

 void	SpecialMoves::_fly(void) {
 	this->character->_isFlying = (this->character->_isFlying ? false : true);
 }
