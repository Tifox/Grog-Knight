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

 #include "../inc/Enemy.hpp"

Enemy::Enemy(void) : Characters("Enemy") {
	return ;
}

Enemy::Enemy(std::string str) : Characters(str) {
	this->setXStart(15);
	this->setYStart(-3);
	if (str == "Enemy") {
		theSwitchboard.SubscribeTo(this, "startPathing");
		theSwitchboard.Broadcast(new Message("startPathing"));
		this->setXStart(5);
		this->setYStart(-5);
	} else {
		
	}
	this->addAttribute("type", "Enemy");
	this->addAttribute("name", str);
	this->addAttribute("enemy", "1");
	this->display();
	return ;
}

/**
 * Basic Destructor
 */
Enemy::~Enemy(void) {
	return ;
}

/**
 * Init Animation
 */
void	Enemy::init(void) {
	this->AnimCallback("base");
	this->_orientation = RIGHT;
}

/**
 * Mother's callback for actions
 * @param: name (std::string)
 * @param: status (int)
 */
void	Enemy::actionCallback(std::string name, int status) {
	return ;
}

/**
 * Collision begin callback
 * @param: elem (Elements *)
 * @param: contact (b2Contact *)
 * @note: This function is called just before a collision
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


/**
 * Function that applies damage
 * @param: damage (int)
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
