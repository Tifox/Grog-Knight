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
 SpecialMoves::SpecialMoves(Characters* charac) {
	 std::string		file;
	 std::stringstream 	buffer;
	 std::ifstream		fd;
	 Json::Reader		read;
	 Json::Value		json;
	 Json::ValueIterator i, v;
	 this->character = charac;
	 std::map<std::string, Json::Value>	tmp;
	 file = "Resources/Elements/SpecialMoves.json";
	 fd.open(file.c_str());
	 if (!fd.is_open())
		Log::error("Can't open the file for the specialmoves class.");
	buffer << fd.rdbuf();
	 if (!read.parse(buffer, json))
		 Log::error("Error in json syntax :\n" + read.getFormattedErrorMessages());
	 for (i = json.begin(); i != json.end(); i++) {
		 for (v = (*i).begin(); v != (*i).end(); v++) {
			 tmp[v.key().asString()] = (*v);
			 this->character->_attr[i.key().asString()] = tmp;
		 }
	 }
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


 void	SpecialMoves::_disengage(void) {
 	this->character->_setCategory("disengage");
 	if (this->character->_isAttacking == 0 && this->character->_canMove == 1 && this->character->_speMoveReady == 1) {
 		this->character->_speMoveReady = 0;
 		this->character->_isDisengaging = true;
 		this->character->_canMove = 0;
 		theSwitchboard.SubscribeTo(this->character, "disengageEnd");
	        theSwitchboard.DeferredBroadcast(new Message("speMoveReady"),
 				this->character->_getAttr("cooldown").asFloat());
		Game::getHUD()->speMoveCooldown(this->character->_getAttr("cooldown").asFloat());
		theSwitchboard.DeferredBroadcast(new Message("disengageEnd"),
 				this->character->_getAttr("uptime").asFloat());
		Weapon *currentWeapon = Game::currentGame->getHero()->getWeapon();
		theSwitchboard.SubscribeTo(this->character, "enableAttackHitbox");
		theSwitchboard.Broadcast(new Message("enableAttackHitbox"));
		theSwitchboard.UnsubscribeFrom(this->character, "enableAttackHitbox");
		if (this->character->_latOrientation == Characters::LEFT)
		  this->character->GetBody()->SetLinearVelocity(b2Vec2(this->character->_getAttr("chargeSpeed").asInt(), this->character->_getAttr("djump").asInt()));
		else if (this->character->_latOrientation == Characters::RIGHT)
		  this->character->GetBody()->SetLinearVelocity(b2Vec2(-this->character->_getAttr("chargeSpeed").asInt(), this->character->_getAttr("djump").asInt()));
		 }
 }


 //! Special move: dash
 /**
  * Character executes a dash if the cooldown is up and the conditions allows it
  * Properties of dash - no gravity, take damage, cant move
  * @sa SpecialMoves::_specialMove()
  */

 void	SpecialMoves::_dash(void) {
   this->character->_setCategory("dash");
 	if (this->character->_isAttacking == 0 && this->character->_canMove == 1) {
 		this->character->_isDashing = true;
 		this->character->GetBody()->SetGravityScale(0);
 		this->character->actionCallback("dash", 0);
		// 		this->character->_canMove = 0;
 		if (this->character->_grounds.size() == 0)
 			this->character->_hasDashed = 1;
 		theSwitchboard.SubscribeTo(this->character, "dashEnd");
 		theSwitchboard.DeferredBroadcast(new Message("dashEnd"),
						 this->character->_getAttr("uptime").asFloat());
 		theSwitchboard.DeferredBroadcast(new Message("speMoveReady"),
						 this->character->_getAttr("cooldown").asFloat());
		Game::getHUD()->speMoveCooldown(this->character->_getAttr("cooldown").asFloat());
 		if (this->character->_latOrientation == Characters::LEFT)
 			this->character->GetBody()->SetLinearVelocity(b2Vec2(-this->character->_getAttr("dashSpeed").asInt(), 0));
 		else if (this->character->_latOrientation == Characters::RIGHT)
 			this->character->GetBody()->SetLinearVelocity(b2Vec2(this->character->_getAttr("dashSpeed").asInt(), 0));
 	}
 }

 //! Special move: charge
 /**
  * Character executes a charge if the cooldown is up and the conditions allows it
  * Properties of charge - invincibility, can move, deals damage
  * @sa SpecialMoves::_specialMove()
  */

 void	SpecialMoves::_charge(void) {
 	this->character->_setCategory("charge");
 	if (this->character->_isAttacking == 0 && this->character->_canMove == 1 && this->character->_speMoveReady == 1 && this->character->_grounds.size() > 0) {
 		this->character->_invincibility = true;
 		this->character->_isCharging = true;
 		this->character->_canMove = 0;
 		theSwitchboard.SubscribeTo(this->character, "chargeEnd");
 		theSwitchboard.DeferredBroadcast(new Message("speMoveReady"),
 				this->character->_getAttr("cooldown").asFloat());
		Game::getHUD()->speMoveCooldown(this->character->_getAttr("cooldown").asFloat());
 		theSwitchboard.DeferredBroadcast(new Message("chargeEnd"),
 				this->character->_getAttr("uptime").asFloat());
 		if (this->character->_latOrientation == Characters::LEFT)
 			this->character->GetBody()->SetLinearVelocity(b2Vec2(-this->character->_getAttr("chargeSpeed").asInt(), 0));
 		else if (this->character->_latOrientation == Characters::RIGHT)
 			this->character->GetBody()->SetLinearVelocity(b2Vec2(this->character->_getAttr("chargeSpeed").asInt(), 0));
		this->character->_setCategory(this->character->getWeapon()->getType());
		std::string o;
		if (this->character->_latOrientation == Characters::LEFT)
			o = "Left";
		else
			o = "Right";
		this->character->changeSizeTo(Vector2(this->character->_getAttr("chargeX").asFloat(), this->character->_getAttr("chargeY").asFloat()));
		this->character->PlaySpriteAnimation(this->character->_getAttr("time").asFloat(), SAT_OneShot,
								  this->character->_getAttr("beginFrameCharge" + o).asInt(),
								  this->character->_getAttr("endFrameCharge" + o).asInt(), "chargeHold");
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
 		theSwitchboard.SubscribeTo(this->character, "stompEnd");
 		theSwitchboard.DeferredBroadcast(new Message("speMoveReady"),
										 this->character->_getAttr("cooldown").asFloat());
		Game::getHUD()->speMoveCooldown(this->character->_getAttr("cooldown").asFloat());
 		this->character->GetBody()->SetLinearVelocity(b2Vec2(0, -this->character->_getAttr("stompSpeed").asInt()));
 		this->character->actionCallback("stomp", 0);
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
	Map m = Game::currentGame->getCurrentMap();
 	int x = (this->character->GetBody()->GetWorldCenter().x) - m.getXStart() + 0.5;
 	int y = -((this->character->GetBody()->GetWorldCenter().y) - m.getYStart() - 0.5);
 	int range = this->character->_getAttr("blinkRange").asInt();
 	std::vector<std::vector<int>> t = m.getPhysicMap();
	if (y >= 16)
		y = 15;
 	if (this->character->_isAttacking == 0 && this->character->_canMove == 1 && this->character->_speMoveReady == 1) {
	  if (this->character->_orientation == Characters::UP) {
 			while (range > 0) {
 				if (y - range > 0 && !t[y - range][x])
 					break;
 				range--;
				goto speMoveReady;
			}
 			if (range > 0) {
 				this->character->GetBody()->SetTransform(b2Vec2(this->character->GetBody()->GetWorldCenter().x,
 							this->character->GetBody()->GetWorldCenter().y + range), 0);
				goto speMoveReady;
			}
 		}
 		else if (this->character->_orientation == Characters::DOWN) {
 			while (range > 0) {
 				if (y + range < (t.size()) && !t[y + range - 1][x])
 					break;
 				range--;
				goto speMoveReady;
			}
 			range--;
 			if (range > 0) {
 				this->character->GetBody()->SetTransform(b2Vec2(this->character->GetBody()->GetWorldCenter().x,
 							this->character->GetBody()->GetWorldCenter().y - range), 0);
				goto speMoveReady;
			}
 		}
 		else if (this->character->_orientation == Characters::RIGHT) {
 			while (range > 0) {
 				if (x + range < t[y].size() && !t[y][x + range])
 					break;
 				range--;
				goto speMoveReady;
			}
 			if (range > 0) {
 				this->character->GetBody()->SetTransform(b2Vec2(this->character->GetBody()->GetWorldCenter().x + range,
 							this->character->GetBody()->GetWorldCenter().y), 0);
				goto speMoveReady;
			}
 		}
 		else if (this->character->_orientation == Characters::LEFT) {
 			while (range > 0) {
 				if (x - range > 0 && !t[y][x - range])
 					break;
 				range--;
				goto speMoveReady;
			}
 			if (range > 0) {
				this->character->_speMoveReady = 0;
				this->character->_grounds.clear();
 				this->character->GetBody()->SetTransform(b2Vec2(this->character->GetBody()->GetWorldCenter().x - range,
 					this->character->GetBody()->GetWorldCenter().y), 0);
				goto speMoveReady;
			}
 		}
checkHitbox:
 		if (range > 0) {
  			b2PolygonShape box = Game::hList->getHitbox(this->character->_hitbox);
 			b2Shape *shape = &box;
 			this->character->GetBody()->DestroyFixture(this->character->GetBody()->GetFixtureList());
 			this->character->GetBody()->CreateFixture(shape, 1);
 		}
 	}
	return ;
speMoveReady:
		theSwitchboard.DeferredBroadcast(new Message("speMoveReady"),
			this->character->_getAttr("cooldown").asFloat());
		Game::getHUD()->speMoveCooldown(this->character->_getAttr("cooldown").asFloat());
		goto checkHitbox;
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

 //! Special move: totem
 /**
  * Puts a totem where the hero is, then teleports the hero on it when reactivated
  * Properties of totem - place and activate
  * @sa SpecialMoves::_specialMove()
  */

 void	SpecialMoves::_totem(void) {
	 if (Game::stopPattern == true)
		 return;
	 if (this->character->_totem == nullptr && this->character->_grounds.size() > 0) {
		 this->character->_totem = new Elements();
		 this->character->_totem->SetSize(0.6);
		 this->character->_totem->SetName("Totem");
		 this->character->_totem->SetDrawShape(ADS_Square);
		 this->character->_totem->SetColor(0, 1, 0, 1);
		 this->character->_totem->SetSprite("Resources/Images/HUD/cible.png");
		 this->character->_totem->SetLayer(105);
		 this->character->_totem->addAttribute("physic", "1");
		 this->character->_totem->addAttribute("currentX", std::to_string(Game::currentX));
		 this->character->_totem->addAttribute("currentY", std::to_string(Game::currentY));
		 this->character->_totem->SetDensity(0);
		 this->character->_totem->SetFixedRotation(true);
		 this->character->_totem->SetIsSensor(true);
		 this->character->_totem->SetPosition(this->character->GetBody()->GetWorldCenter().x, this->character->GetBody()->GetWorldCenter().y);
		 this->character->_totem->InitPhysics();
		 Game::getHUD()->addTotemToBigMap();
		 theWorld.Add(this->character->_totem);
	 }
	 else if (this->character->_totem != nullptr) {
		 if (atoi(this->character->_totem->getAttribute("currentX").c_str()) != Game::currentX && atoi(this->character->_totem->getAttribute("currentY").c_str()) != Game::currentY) {
			 Game::currentGame->getCurrentMap().destroyMap();
			 this->character->inSpecialMap = 0;
			 Game::currentX = atoi(this->character->_totem->getAttribute("currentX").c_str());
			 Game::currentY = atoi(this->character->_totem->getAttribute("currentY").c_str());
			 Game::currentGame->maps->_XYMap[Game::currentY][Game::currentX] = Game::currentGame->getCurrentMap().display();
			 theCamera.SetPosition(Game::currentGame->getCurrentMap().getXMid(), Game::currentGame->getCurrentMap().getYMid() + 1.8);
			 if (Game::isInMenu == 0)
				 Game::getHUD()->minimap();
		 }
		 this->character->GetBody()->SetTransform(b2Vec2(this->character->_totem->GetBody()->GetWorldCenter().x, this->character->_totem->GetBody()->GetWorldCenter().y), 0);
		 Game::addToDestroyList(this->character->_totem);
		 this->character->_totem = nullptr;
		 Game::getHUD()->removeText("T");
	 }
 }

//! Special move: shunpo
/**
 * Special move that requires a target - on use, dashes to the target and deals damage
 * Properties of shunpo - requires target, range, deals damage, cooldown
 */

void	SpecialMoves::_shunpo(void) {
	if (this->character->_target != nullptr) {
		int i;
		int x;
		int x2;
		int y;
		int y2;
		x = this->character->GetBody()->GetWorldCenter().x;
		if (x < 0) x *= -1;
		x2 = this->character->_target->getCurrentEnemy()->GetBody()->GetWorldCenter().x;
		if (x2 < 0) x2 *= -1;
		x = x - x2;
		if (x < 0) x *= -1;
		y = this->character->GetBody()->GetWorldCenter().y;
		if (y < 0) y *= -1;
		y2 = this->character->_target->getCurrentEnemy()->GetBody()->GetWorldCenter().y;
		if (y2 < 0) y2 *= -1;
		y = y - y2;
		if (y < 0) y *= -1;
		if (x + y > this->character->_getAttr("shunpo", "range").asInt()) {
			return;
		}
		if (this->character->_speMoveReady == false || this->character->_canMove == false)
			return;
		this->character->_speMoveReady = false;
		theSwitchboard.DeferredBroadcast(new Message("speMoveReady"), this->character->_getAttr("shunpo", "cooldown").asFloat());
		Game::getHUD()->speMoveCooldown(this->character->_getAttr("cooldown").asFloat());
		if (this->character->_target->getCurrentEnemy()->getOrientation() == Characters::LEFT)
			i = 1;
		else
			i = -1;
		this->character->GetBody()->SetTransform(b2Vec2(this->character->_target->GetBody()->GetWorldCenter().x + 1,
														this->character->_target->GetBody()->GetWorldCenter().y), 0);
		if (this->character->_target->getCurrentEnemy()->takeDamage(this->character->_weapon->getDamage(),
																	this->character->_weapon->getCritRate()) == 1)
		  this->character->_target->getCurrentEnemy()->GetBody()->SetLinearVelocity(b2Vec2((5 * i), 5));
	}
}
