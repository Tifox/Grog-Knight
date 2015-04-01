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
	this->setXStart(15);
	this->setYStart(-3);
	if (str == "Enemy") {
		theSwitchboard.SubscribeTo(this, "startPathing");
		theSwitchboard.Broadcast(new Message("startPathing"));
		this->setXStart(5);
		this->setYStart(-5);
	} else {
		// ????
	}
	this->addAttribute("type", "Enemy");
	this->addAttribute("name", str);
	this->addAttribute("enemy", "1");
	this->display();
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
	this->AnimCallback("base");
	this->_orientation = RIGHT;
}

//! Action callback
/**
 * Mother's callback for actions
 * See Characters::ReceiveMessage for more information
 * @param: name The name of the action
 * @param: status The key status (1 | 0)
 * @sa Characters::ReceiveMessage
 */
void	Enemy::actionCallback(std::string name, int status) {
	return ;
}

//! Begin collision function
/**
 * Collision begin callback
 * /!\ This function is called just before a collision
 * @param: elem The Elements who collide
 * @param: contact The Box2D contact object
 */
void	Enemy::BeginContact(Elements* m, b2Contact *contact) {
	Characters::BeginContact(m, contact);
	Weapon* w = static_cast<Weapon*>(m);
	Projectile* p = static_cast<Projectile*>(m);
	if (m->getAttributes()["type"] == "HeroWeaponHitBox") {
		if (this->takeDamage(w->getDamage()) == 1) {
			if (this->GetBody()->GetWorldCenter().x > m->GetBody()->GetWorldCenter().x) {
				this->ApplyLinearImpulse(Vector2(w->getPushback(), w->getPushback()), Vector2(0,0));
			} else {
				this->ApplyLinearImpulse(Vector2(-w->getPushback(), w->getPushback()), Vector2(0,0));
			}
		}
	} else if (m->getAttributes()["type"] == "HeroProjectile") {
		if (this->takeDamage(p->getDamage()) == 1) {
			if (this->GetBody()->GetWorldCenter().x > m->GetBody()->GetWorldCenter().x) {
				this->ApplyLinearImpulse(Vector2(p->getPushback(), p->getPushback()), Vector2(0,0));
			} else {
				this->ApplyLinearImpulse(Vector2(-p->getPushback(), p->getPushback()), Vector2(0,0));
			}
		}
	} else if (m->getAttributes()["type"] == "Hero") {
		if (this->_orientation == LEFT)
			this->_orientation = RIGHT;
		else
			this->_orientation = LEFT;
		if (this->GetBody()->GetWorldCenter().x > m->GetBody()->GetWorldCenter().x) {
			this->GetBody()->SetLinearVelocity(b2Vec2(2, 2));
		} else {
			this->GetBody()->SetLinearVelocity(b2Vec2(-2, 2));
		}
	}
}

//! Take Damage
/**
 * Function that applies damage
 * In this function, we reduce the HP (no way ?), reduce the speed, play a sprite animation.
 * @param: damage The damage amount
 */
int		Enemy::takeDamage(int damage) {
	if (this->_hp - damage <= 0) {
		this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		this->PlaySpriteAnimation(0.1, SAT_OneShot, 5, 5, "destroyEnemy");
		theSwitchboard.SubscribeTo(this, "destroyEnemy");
		theSwitchboard.DeferredBroadcast(new Message("destroyEnemy"), 0.1);
		theSwitchboard.UnsubscribeFrom(this, "startPathing");
		return 0;
	}
	this->_hp -= damage;
	return 1;
}
