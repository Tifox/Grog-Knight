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
 * File: Projectile.cpp
 * Creation: 2015-02-23 15:140
 * Matthieu Maudet <mmaudet@student.42.fr>
 */

# include "../inc/Projectile.hpp"

Projectile::Projectile(Weapon* w, Characters* c){

	this->_name = w->getName();
	this->_flavor = w->getFlavor();
	this->_attack = w->getAttack();
	this->_active = w->getActive();
	this->_damage = w->getDamage();
	this->_pushback = w->getPushback();
	this->_recovery = w->getRecovery();
	this->SetSize(0.5f);
	this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	this->SetSprite("Resources/Images/kappa.png");
	this->SetDensity(1);
	this->SetFriction(0);
	this->SetRestitution(0.0f);
	this->SetFixedRotation(true);
	this->SetIsSensor(true);
	if (c->getAttributes()["type"] == "Hero")
		this->addAttribute("type", "HeroProjectile");
	else
		this->addAttribute("type", "Projectile");
	this->Tag("projectile");
	this->_initDirection(w, c);
	theSwitchboard.DeferredBroadcast(new Message("canAttack"), this->_recovery);
	theWorld.Add(this);
}

/**
 * Called during construction for clarity, used to determine projectile direction
 * @param: w (Weapon*)
 * @param: c (Characters*)
 */

void	Projectile::_initDirection(Weapon* w, Characters* c) {
	int xDecal;
	int yDecal;
	int xOrient;
	int yOrient;

	if (c->getOrientation() == Characters::RIGHT) {
		xOrient = 1;
		xDecal = 1;
		yDecal = 0;
		yOrient = 0;
	}
	if (c->getOrientation() == Characters::LEFT) {
		xOrient = -1;
		xDecal = -1;
		yDecal = 0;
		yOrient = 0;
	}
	if (c->getOrientation() == Characters::UP) {
		xOrient = 0;
		xDecal = 0;
		yDecal = 1;
		yOrient = 1;
	}
	if (c->getOrientation() == Characters::DOWN) {
		xOrient = 0;
		xDecal = 0;
		yDecal = -1;
		yOrient = -1;
	}
	this->SetPosition(c->GetBody()->GetWorldCenter().x + xDecal, c->GetBody()->GetWorldCenter().y + yDecal);
	this->InitPhysics();
	this->GetBody()->SetGravityScale(0.0f);
	this->GetBody()->SetBullet(true);
	this->ApplyLinearImpulse(Vector2(3 * xOrient, 3 * yOrient), Vector2(0, 0));
}

Projectile::~Projectile(void) {
	return;
}

void	Projectile::EndContact(Elements *elem, b2Contact *contact) {
}

/* GETTERS */
std::string     Projectile::getName(void) { return this->_name; }
std::string     Projectile::getFlavor(void) { return this->_flavor; }
std::string     Projectile::getAttack(void) { return this->_attack; }
float           Projectile::getActive(void) { return this->_active; }
int             Projectile::getSize(void) { return this->_size; }
int             Projectile::getDamage(void) { return this->_damage; }
int             Projectile::getPushback(void) { return this->_pushback; }
float           Projectile::getRecovery(void) { return this->_recovery; }

/**
 * Receive broadcasts message
 * @param: (Message *)
 */
void	Projectile::ReceiveMessage(Message *m) {
}

void	Projectile::BeginContact(Elements *m, b2Contact *c) {
	Game::addToDestroyList(this);
}
