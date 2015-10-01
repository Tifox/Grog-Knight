
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

# include "SpecialAttack.hpp"
# include "Characters.hpp"
# include <cstdlib>

//! Base constructor
SpecialAttack::SpecialAttack(void) {

}

SpecialAttack::~SpecialAttack(void) {

}
SpecialAttack::SpecialAttack(Characters* charac) {
	std::string		file;
	std::stringstream 	buffer;
	std::ifstream		fd;
	Json::Reader		read;
	Json::Value		json;
	Json::ValueIterator i, v;
	std::map<std::string, Json::Value>	tmp;
	this->character = charac;
	file = "Resources/Elements/SpecialAttack.json";
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
	theSwitchboard.SubscribeTo(this, "SpecialAttackEnd");
	theSwitchboard.SubscribeTo(this, "linkWeapon");
}

void		SpecialAttack::ReceiveMessage(Message *m) {
	Characters *c = Game::currentGame->getHero();
	if (m->GetMessageName() == "linkWeapon") {
		this->_left->GetBody()->SetGravityScale(0);
		this->_right->GetBody()->SetGravityScale(0);
		this->_left->GetBody()->SetLinearVelocity(b2Vec2(0,0));
		this->_right->GetBody()->SetLinearVelocity(b2Vec2(0,0));
		b2DistanceJointDef jointDef1;
		b2DistanceJointDef jointDef2;
		b2DistanceJointDef jointDef3;
		b2DistanceJointDef jointDef4;
		jointDef1.Initialize(c->GetBody(), this->_left->GetBody(), b2Vec2(c->GetBody()->GetWorldCenter().x, c->GetBody()->GetWorldCenter().y + 0.4),
							 b2Vec2(this->_left->GetBody()->GetWorldCenter().x, this->_left->GetBody()->GetWorldCenter().y + 0.4));
		jointDef1.collideConnected = false;

		jointDef2.Initialize(c->GetBody(), this->_right->GetBody(), b2Vec2(c->GetBody()->GetWorldCenter().x, c->GetBody()->GetWorldCenter().y + 0.4),
							 b2Vec2(this->_right->GetBody()->GetWorldCenter().x, this->_right->GetBody()->GetWorldCenter().y + 0.4));
		jointDef2.collideConnected = false;

		jointDef3.Initialize(c->GetBody(), this->_left->GetBody(), b2Vec2(c->GetBody()->GetWorldCenter().x, c->GetBody()->GetWorldCenter().y - 0.4),
							 b2Vec2(this->_left->GetBody()->GetWorldCenter().x, this->_left->GetBody()->GetWorldCenter().y - 0.4));
		jointDef3.collideConnected = false;

		jointDef4.Initialize(c->GetBody(), this->_right->GetBody(), b2Vec2(c->GetBody()->GetWorldCenter().x, c->GetBody()->GetWorldCenter().y - 0.4),
							 b2Vec2(this->_right->GetBody()->GetWorldCenter().x, this->_right->GetBody()->GetWorldCenter().y - 0.4));
		jointDef4.collideConnected = false;

		b2DistanceJoint *joint1 = (b2DistanceJoint*)theWorld.GetPhysicsWorld().CreateJoint(&jointDef1);
		b2DistanceJoint *joint2 = (b2DistanceJoint*)theWorld.GetPhysicsWorld().CreateJoint(&jointDef2);
		b2DistanceJoint *joint3 = (b2DistanceJoint*)theWorld.GetPhysicsWorld().CreateJoint(&jointDef3);
		b2DistanceJoint *joint4 = (b2DistanceJoint*)theWorld.GetPhysicsWorld().CreateJoint(&jointDef4);
	}
	if (m->GetMessageName() == "RapidFire") {
			Game::currentGame->getHero()->getWeapon()->attack(this->character);
			theSwitchboard.DeferredBroadcast(new Message("RapidFire"), this->character->_getAttr("rapidFire", "interval").asFloat());
	}
	if (m->GetMessageName() == "SpecialAttackEnd") {
		if (this->_currentAttack == "whirlwind") {
			Game::currentGame->getHero()->buff.bonusSpeed = this->_previousSpeed;
			Game::currentGame->getHero()->_isWhirlwinding = false;
			c->AnimCallback("base");
			c->_canAttack = true;
			c->_invincibility = false;
			this->_currentAttack = "";
		}
		else if (this->_currentAttack == "shockwave") {
			this->_currentAttack = "";
		}
		if (this->_currentAttack == "rapidFire") {
			Game::currentGame->getHero()->_isWhirlwinding = false;
			theSwitchboard.UnsubscribeFrom(this, "RapidFire");
			this->_currentAttack = "";
		}
	}
}

void	SpecialAttack::_whirlwind(void) {
	this->character->_setCategory("whirlwind");
	Weapon *currentWeapon = Game::currentGame->getHero()->getWeapon();
	Characters *hero = Game::currentGame->getHero();
	if (this->character->_isAttacking == 0 && this->character->_canMove == 1 && this->character->_speAttReady == 1 /*&& currentWeapon->getType() == "Sword"*/) {
	  this->character->_speAttReady = 0;
	  this->character->_isWhirlwinding = true;
	  currentWeapon->setActive(this->character->_getAttr("whirlwind", "uptime").asFloat());
	  this->_right = new Weapon (Game::currentGame->getHero()->_weapon, Game::currentGame->getHero(), 1);
	  this->_left = new Weapon (Game::currentGame->getHero()->_weapon, Game::currentGame->getHero(), -1);
	  this->_previousSpeed = hero->buff.bonusSpeed;
	  hero->_invincibility = true;
	  hero->buff.bonusSpeed = -(hero->_getAttr("forward", "force").asInt() / 2);
	  hero->_canAttack = false;
	  theSwitchboard.DeferredBroadcast(new Message("SpecialAttackEnd"),
					   this->character->_getAttr("whirlwind", "uptime").asFloat());
	  theSwitchboard.DeferredBroadcast(new Message("speAttReady"),
					   this->character->_getAttr("cooldown").asFloat());
	  hero->changeSizeTo(Vector2(hero->_getAttr("x").asFloat(), hero->_getAttr("y").asFloat()));
	  hero->PlaySpriteAnimation(hero->_getAttr("time").asFloat(), SAT_Loop,
				    hero->_getAttr("beginFrame").asInt(),
				    hero->_getAttr("endFrame").asInt(), "base");
	  this->_currentAttack = "whirlwind";
	}
}


void	SpecialAttack::_shockwave(void) {
	this->character->_setCategory("shockwave");
	Weapon *currentWeapon = Game::currentGame->getHero()->getWeapon();
	Characters *hero = Game::currentGame->getHero();
	if (this->character->_isAttacking == 0 && this->character->_canMove == 1 && this->character->_speAttReady == 1 && currentWeapon->getType() == "Axe") {
		this->character->_speAttReady = 0;
		this->character->_isShockWaving = true;
		theSwitchboard.DeferredBroadcast(new Message("speAttReady"),
				this->character->_getAttr("cooldown").asFloat());
		int dmg = this->character->_getAttr("shockwave", "damage").asInt();
		Projectile *wave = new Projectile(currentWeapon, dmg);
		this->_currentAttack = "shockwave";
		std::string orientation;
		if (hero->_latOrientation == Characters::RIGHT)
			orientation = "right";
		else if (hero->_latOrientation == Characters::LEFT)
			orientation = "left";
		hero->changeSizeTo(Vector2(hero->_getAttr("x").asInt(), hero->_getAttr("y").asInt()));
		hero->PlaySpriteAnimation(hero->_getAttr("time").asFloat(), SAT_OneShot,
								  hero->_getAttr("beginFrame_" + orientation).asInt(),
								  hero->_getAttr("endFrame_" + orientation).asInt(), "base");
		theSwitchboard.DeferredBroadcast(new Message("speAttReady"),
										 this->character->_getAttr("cooldown").asFloat());
	}
}

void	SpecialAttack::_rapidFire(void) {
	this->character->_setCategory("rapidFire");
	Weapon *currentWeapon = Game::currentGame->getHero()->getWeapon();
	Characters *hero = Game::currentGame->getHero();
	std::string orientation;

	theSwitchboard.SubscribeTo(this, "RapidFire");

	if (this->character->_isAttacking == 0 && this->character->_canMove == 1 && this->character->_speAttReady == 1 && currentWeapon->getType() == "Bow") {
		this->character->_speAttReady = 0;
		this->character->_isRapidFiring = true;
		if (hero->_latOrientation == Characters::RIGHT)
		  orientation = "right";
		else if (hero->_latOrientation == Characters::LEFT)
		  orientation = "left";
		hero->changeSizeTo(Vector2(hero->_getAttr("x").asInt(), hero->_getAttr("y").asInt()));
		hero->PlaySpriteAnimation(hero->_getAttr("time").asFloat(), SAT_OneShot,
								  hero->_getAttr("beginFrame_" + orientation).asInt(),
								  hero->_getAttr("endFrame_" + orientation).asInt(), "base");
		theSwitchboard.DeferredBroadcast(new Message("speAttReady"),
				this->character->_getAttr("cooldown").asFloat());
		theSwitchboard.DeferredBroadcast(new Message("SpecialAttackEnd"),
				this->character->_getAttr("rapidFire", "uptime").asFloat());
		theSwitchboard.DeferredBroadcast(new Message("RapidFire"), 0);
		this->_currentAttack = "rapidFire";
	}
}
