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
}

/**
 * Mother's callback for actions
 * @param: name (std::string)
 * @param: status (int)
 */
void	Enemy::actionCallback(std::string name, int status) {
	std::cout << "CALLBACK " << name << std::endl;
	return ;
}

void	Enemy::BeginContact(Elements* m, b2Contact *contact) {
	Characters::BeginContact(m, contact);
	Weapon* w = static_cast<Weapon*>(m);
	Projectile* p = static_cast<Projectile*>(m);
	if (m->getAttributes()["type"] == "HeroWeaponHitBox") {
		if (this->GetBody()->GetWorldCenter().x > m->GetBody()->GetWorldCenter().x) {
			this->ApplyLinearImpulse(Vector2(w->getPushback(), w->getPushback()), Vector2(0,0));
		} else {
			this->ApplyLinearImpulse(Vector2(-w->getPushback(), w->getPushback()), Vector2(0,0));
		}
	} else if (m->getAttributes()["type"] == "HeroProjectile") {
		if (this->GetBody()->GetWorldCenter().x > m->GetBody()->GetWorldCenter().x) {
			this->ApplyLinearImpulse(Vector2(p->getPushback(), p->getPushback()), Vector2(0,0));
		} else {
			this->ApplyLinearImpulse(Vector2(-p->getPushback(), p->getPushback()), Vector2(0,0));
		}
	}
}
