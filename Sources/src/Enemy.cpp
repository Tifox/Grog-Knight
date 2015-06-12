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
 * File: Enemy.cpp
 * Creation: 2015-02-23 14:26
 * Manon Budin <mbudin@student.42.fr>
 */

# include "../inc/Enemy.hpp"

//! Basic constructor
/**
 * A basic constructor, with an mother (Characters) init.
 */
Enemy::Enemy(void) : Characters("Enemy") {
	return ;
}

//! Main constructor
/**
 * The main constructor, init with a name.
 * This constructor set position the hard way, may be better to use the config file info ?
 * @param str Name of the new Enemy
 */
Enemy::Enemy(std::string str) : Characters(str) {
	this->SetName("Enemy");
	this->addAttribute("type", "Enemy");
	this->addAttribute("name", str);
	this->addAttribute("enemy", "1");
	this->_isDead = false;
	this->_isTakingDamage = 0;
	return ;
}

//! Destructor
/**
 * Basic Destructor
 */
Enemy::~Enemy(void) {
	return ;
}

//! Init Animation
/**
 * Do the first animation call.
 */
void	Enemy::init(void) {
	this->display();
	this->AnimCallback("base");
	this->_orientation = RIGHT;
}

//! Action callback
/**
 * Mother's callback for actions
 * See Characters::ReceiveMessage for more information
 * @param name The name of the action
 * @param status The key status (1 | 0)
 * @sa Characters::ReceiveMessage
 */
void	Enemy::actionCallback(std::string name, int status) {
	return ;
}

//! Begin collision function
/**
 * Collision begin callback
 * /!\ This function is called just before a collision
 * @param elem The Elements who collide
 * @param contact The Box2D contact object
 */
void	Enemy::BeginContact(Elements* m, b2Contact *contact) {
	Characters::BeginContact(m, contact);
	Weapon* w = static_cast<Weapon*>(m);
	Projectile* p = static_cast<Projectile*>(m);
	if (m->getAttribute("type") == "ground") {
		Game::startRunning(this);
		this->_isTakingDamage = 0;
	}
	if (m->getAttribute("type") == "HeroWeaponHitBox") {
		this->GetBody()->SetLinearVelocity(b2Vec2(-2, 2));
		Game::stopRunning(this);
		if (this->takeDamage(w->getDamage()) == 1) {
			this->_isTakingDamage = 1;
			if (this->GetBody()->GetWorldCenter().x > m->GetBody()->GetWorldCenter().x) {
				this->ApplyLinearImpulse(Vector2(w->getPushback(), w->getPushback()), Vector2(0,0));
			} else {
				this->ApplyLinearImpulse(Vector2(-w->getPushback(), w->getPushback()), Vector2(0,0));
			}
		}
		else {
			this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		}
	} else if (m->getAttribute("type") == "HeroProjectile") {
		this->GetBody()->SetLinearVelocity(b2Vec2(-2, 2));
		Game::stopRunning(this);
		this->_isTakingDamage = 1;
		if (this->takeDamage(p->getDamage()) == 1) {
			if (this->GetBody()->GetWorldCenter().x > m->GetBody()->GetWorldCenter().x) {
				this->ApplyLinearImpulse(Vector2(p->getPushback(), p->getPushback()), Vector2(0,0));
			} else {
				this->ApplyLinearImpulse(Vector2(-p->getPushback(), p->getPushback()), Vector2(0,0));
			}
		}
		else {
			this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		}
	} else if (m->getAttribute("type") == "Hero") {
		this->_isTakingDamage = 1;
		Game::stopRunning(this);
		if (this->_orientation == LEFT)
			this->_orientation = RIGHT;
		else
			this->_orientation = LEFT;
		if (static_cast<Hero*>(m)->getCharging() == true) {
			if (this->GetBody()->GetWorldCenter().x > m->GetBody()->GetWorldCenter().x) {
				this->GetBody()->SetLinearVelocity(b2Vec2(10, 10));
			} else {
				this->GetBody()->SetLinearVelocity(b2Vec2(-10, 10));
			}
		} else {
			if (this->GetBody()->GetWorldCenter().x > m->GetBody()->GetWorldCenter().x) {
				this->GetBody()->SetLinearVelocity(b2Vec2(2, 2));
			} else {
				this->GetBody()->SetLinearVelocity(b2Vec2(-2, 2));
			}
		}
		this->actionCallback("heroHit", 0);
	}
	else if (m->getAttribute("speType") == "spikes") {
		this->_isTakingDamage = 1;
		if (this->GetBody()->GetWorldCenter().x > m->GetBody()->GetWorldCenter().x) {
			this->ApplyLinearImpulse(Vector2(5, 5), Vector2(0,0));
		} else {
			this->ApplyLinearImpulse(Vector2(-5, 5), Vector2(0,0));
		}
	}
}

void	Enemy::EndContact(Elements *m, b2Contact *contact) {
	Characters::EndContact(m, contact);
	if (m->getAttribute("type") == "ground" && !this->_isDead && !this->_isTakingDamage) {
		if (this->_lastElement != m->getId()) {
			this->_pattern->tick(Game::currentGame->maps->getMapXY()[Game::currentY][Game::currentX]);
			this->_lastElement = m->getId();
		}
	}

}

//! Take Damage
/**
 * Function that applies damage
 * In this function, we reduce the HP, reduce the speed, play a sprite animation.
 * @param damage The damage amount
 */
int		Enemy::takeDamage(int damage) {
	if (this->_hp <= 0)
		return 0;
	this->actionCallback("takeDamage", 0);
	if (this->_hp - damage <= 0) {
		this->_isDead = true;
		this->actionCallback("death", 0);
		this->_setCategory("death");
		theSwitchboard.SubscribeTo(this, "setToStatic" + this->GetName());
		theSwitchboard.Broadcast(new Message("setToStatic" + this->GetName()));
		this->GetBody()->ResetMassData();
		this->GetBody()->GetFixtureList()->SetSensor(true);
		this->GetBody()->SetGravityScale(0);
		this->LoadSpriteFrames(this->_getAttr("newSprites").asString());
		this->changeSizeTo(Vector2(this->_getAttr("size").asInt(),
					this->_getAttr("size").asInt()));
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
				this->_getAttr("beginFrame").asInt(),
				this->_getAttr("endFrame").asInt());
		theSwitchboard.SubscribeTo(this, "destroyEnemy");
		theSwitchboard.DeferredBroadcast(new Message("destroyEnemy"), 0.5);
		return 0;
	}
	this->_hp -= damage;
	return 1;
}

/* SETTERS */

void	Enemy::setMap(Map *m) { this->_map = m; };
void	Enemy::setPattern(CPattern *p) { this->_pattern = p; };

/* GETTERS */

Map		*Enemy::getMap(void) { return this->_map; };
CPattern	*Enemy::getPattern(void) { return this->_pattern; };
bool	Enemy::dead(void) { return this->_isDead; };
