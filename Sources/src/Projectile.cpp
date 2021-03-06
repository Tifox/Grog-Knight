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

//! Constructor called by the Weapon class, when an attack was ordered using a ranged weapon
/**
 * Default constructor
 * @param w Weapon* (weapon object which called the attack)
 * @param c Characters* (Characters object which currently carries the weapon)
 */
 Projectile::Projectile(Weapon* w, Characters* c){

	this->_name = w->getName();
	this->_flavor = w->getFlavor();
	this->_attack = w->getAttack();
	this->_active = w->getActive();
	this->_damage = w->getDamage();
	this->_pushback = w->getPushback();
	this->_recovery = w->getRecovery();
	this->_critRate = w->getCritRate();
	this->SetSize(0.5f);
	this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	this->SetSprite("Resources/Images/arrow.png");
	this->SetDensity(1);
	this->SetLayer(110);
	this->SetFriction(0);
	this->SetRestitution(0.0f);
	this->SetFixedRotation(true);
	this->SetIsSensor(true);
	if (c->getAttribute("type") == "Hero") {
		this->addAttribute("type", "HeroProjectile");
	}
	else
		this->addAttribute("type", "Projectile");
	this->Tag("projectile");
	this->addAttribute("physic", "1");
	this->_initDirection(w, c);
	theSwitchboard.DeferredBroadcast(new Message("attackReady"), this->_recovery);
	theWorld.Add(this);
}

Projectile::Projectile(Weapon* w, int dmg) {

	int xDecal = 0, yDecal = 0, xOrient = 0, yOrient = 0;
	Characters *c = Game::currentGame->getHero();

	this->_name = "shockwave";
	this->_damage = dmg;
	this->_critRate = 0;
	this->_pushback = w->getPushback();
	this->SetSize(0.95f);
	this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	this->SetSprite("Resources/Images/Shockwave.png");
	this->SetDensity(0.5);
	this->SetLayer(101);
	this->SetFriction(0);
	this->SetRestitution(0.0f);
	this->SetFixedRotation(true);
	this->SetIsSensor(true);
	this->addAttribute("type", "Shockwave");
	this->Tag("projectile");
	this->addAttribute("physic", "1");

	if (c->getLatOrientation() == Characters::RIGHT) {
		xOrient = xDecal = 1;
	} else if (c->getLatOrientation() == Characters::LEFT) {
		xOrient = xDecal = -1;
	}
	this->SetPosition(c->GetBody()->GetWorldCenter().x + xDecal, c->GetBody()->GetWorldCenter().y + yDecal);
	this->InitPhysics();
	this->GetBody()->SetGravityScale(0.0f);
	this->GetBody()->SetBullet(true);
	this->ApplyLinearImpulse(Vector2(3 * xOrient, 3 * yOrient), Vector2(0, 0));
	theWorld.Add(this);
}

Projectile::Projectile(std::string img, int dmg, Vector2 pos, Vector2 force, Vector2 init, std::string name, int rotat, float isGravity) : Elements() {
	this->_name = name;
	this->SetName(name);
	this->_toDestroy = false;
	this->addAttribute("damage", std::to_string(dmg));
	if (isGravity != 0) {
		this->SetSize(1.0f);
		this->SetDensity(0.2f);
		this->SetRotation(220);
	} else {
		this->SetSize(0.5f);
		this->SetDensity(1);
	}
	this->SetShapeType(PhysicsActor::SHAPETYPE_CIRCLE);
	this->SetSprite(img);
	this->SetFriction(0);
	this->SetRestitution(0);
	this->SetIsSensor(true);
	this->SetFixedRotation(true);
	this->addAttribute("type", "projectile");
	this->addAttribute("spe", name);
	this->Tag("projectile");
	this->SetPosition(pos.X, pos.Y);
	if (rotat != -1)
	  this->SetRotation(rotat);
	this->InitPhysics();
	this->SetLayer(99);
	this->GetBody()->SetGravityScale(isGravity);
	this->GetBody()->SetBullet(true);
	if (name != "throwSpear" && rotat != -1) {
	  this->ApplyLocalForce(Vector2(75, 0), Vector2(0, 0));
	} else {
	  this->ApplyLinearImpulse(Vector2(force.X, force.Y), Vector2(init.X, init.Y));
	}
	theWorld.Add(this);
}

//! Clarity's cool man
/**
 * Called during construction for clarity, used to determine projectile direction
 * @param: w (Weapon*)
 * @param: c (Characters*)
 */
void	Projectile::_initDirection(Weapon* w, Characters* c) {
	int xDecal = 0, yDecal = 0, xOrient = 0, yOrient = 0;

	if (c->getOrientation() == Characters::RIGHT) {
		xOrient = xDecal = 1;
		this->SetRotation(135.0f);
	} else if (c->getOrientation() == Characters::LEFT) {
		xOrient = xDecal = -1;
		this->SetRotation(-43.0f);
	} else if (c->getOrientation() == Characters::UP) {
		yDecal = yOrient = 1;
		this->SetRotation(-130.0f);
	} else if (c->getOrientation() == Characters::DOWN) {
		yDecal = yOrient = -1;
		this->SetRotation(43.0f);
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


/* GETTERS */
std::string     Projectile::getName(void) { return this->_name; }
std::string     Projectile::getFlavor(void) { return this->_flavor; }
std::string     Projectile::getAttack(void) { return this->_attack; }
float           Projectile::getActive(void) { return this->_active; }
int             Projectile::getSize(void) { return this->_size; }
int             Projectile::getDamage(void) { return this->_damage; }
int             Projectile::getPushback(void) { return this->_pushback; }
float           Projectile::getRecovery(void) { return this->_recovery; }
int				Projectile::getCritRate(void) { return this->_critRate; }

void			Projectile::ReceiveMessage(Message *m) {
	if (m->GetMessageName() == "deleteProj" + this->GetName()) {
		theSwitchboard.UnsubscribeFrom(this, "deleteProj" + this->GetName());
		Game::addToDestroyList(this);
	}
}

//! Overload from b2Body's BeginContact
/**
 * Collision begin callback
 * See Angel docs for more information.
 * /!\ This function is called BEFORE a collision happened.
 * @param elem The Element who collide
 * @param contact The b2Contact object of the collision. See Box2D docs for more info.
 */
void	Projectile::BeginContact(Elements *m, b2Contact *c) {
	if (!m)
		return ;
	if (this->_name == "bossProjectile" && m->getAttribute("type") == "Hero")
		this->_toDestroy = true;
	if (this->_name == "shockwave") {
 		if (m->getAttribute("type") == "Object" || m->getAttribute("type") == "Hero" || m->getAttribute("type") == "Dealer" ||
 			m->getAttribute("type") == "Door" || m->getAttribute("type") == "Enemy")
			return;
	}
	if (this->_name == "bossProjectile" && m->getAttribute("spe") == "bossProjectile") {
		c->SetEnabled(false);
		c->enableContact = false;
		return ;
	}
	else if (this->_name == "bossProjectile" && m->getAttribute("boss") == "true") {
		c->SetEnabled(false);
		c->enableContact = false;
		return ;
	} else if (this->_name == "throwSpear" && m->getAttribute("type") == "Hero") {
		c->SetEnabled(false);
		c->enableContact = false;
		return ;
	} else {
		//here we put the arrow travelling through enemies (instead of 1!=1)
		if (m->getAttribute("type") != "ground" && ((m->getAttribute("type") != "Enemy" || (m->getAttribute("type") == "Enemy" && static_cast<Enemy*>(m)->dead() == true)) || 1 != 1)) {
			return;
		}
	}
	Game::addToDestroyList(this);
}

void	Projectile::EndContact(Elements *m, b2Contact *c) {
	if (!m)
		return ;
	if (this->_toDestroy)
		Game::addToDestroyList(this);
}
