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
  this->_lastHitID = 0;
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
	if (!m)
		return ;
	Characters::BeginContact(m, contact);
	Weapon* w = static_cast<Weapon*>(m);
	Projectile* p = static_cast<Projectile*>(m);
	if (m->getAttribute("type") == "ground") {
	  //	Game::startRunning(this);
	} else if (m->getAttribute("type") == "HeroWeaponHitBox") {
		if (this->_lastHitID == m->getId())
			return;
		else
			this->_lastHitID = m->getId();
	  	Characters *h = Game::currentGame->getHero();
		this->GetBody()->SetLinearVelocity(b2Vec2(-2, 2));
		Game::stopRunning(this);
		if (this->takeDamage(w->getDamage() + h->buff.bonusDmg, (w->getCritRate() + h->buff.critBuff)) == 1) {
			if (this->GetBody()->GetWorldCenter().x > h->GetBody()->GetWorldCenter().x) {
				this->ApplyLinearImpulse(Vector2(w->getPushback(), w->getPushback()), Vector2(0,0));
			} else {
				this->ApplyLinearImpulse(Vector2(-w->getPushback(), w->getPushback()), Vector2(0,0));
			}
		} else {
			this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		}
	} else if (m->getAttribute("type") == "HeroProjectile") {
		if (this->_lastHitID == m->getId())
			return;
		else
			this->_lastHitID = m->getId();
		Characters *h = Game::currentGame->getHero();
		this->GetBody()->SetLinearVelocity(b2Vec2(-2, 2));
		Game::stopRunning(this);
		if (this->takeDamage(p->getDamage() + h->buff.bonusDmg, (w->getCritRate() + h->buff.critBuff)) == 1) {
			if (this->GetBody()->GetWorldCenter().x > h->GetBody()->GetWorldCenter().x) {
				this->ApplyLinearImpulse(Vector2(p->getPushback(), p->getPushback()), Vector2(0,0));
			} else {
				this->ApplyLinearImpulse(Vector2(-p->getPushback(), p->getPushback()), Vector2(0,0));
			}
		}
		else {
			this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		}

	} else if (m->getAttribute("type") == "Shockwave") {
		if (this->_lastHitID == m->getId())
		return;
	  else
		this->_lastHitID = m->getId();
		Characters *h = Game::currentGame->getHero();
		this->GetBody()->SetLinearVelocity(b2Vec2(-2, 2));
		Game::stopRunning(this);
		if (this->takeDamage(static_cast<Projectile*>(m)->getDamage(), 0)) {
			if (this->GetBody()->GetWorldCenter().x > h->GetBody()->GetWorldCenter().x) {
				this->ApplyLinearImpulse(Vector2(p->getPushback(), p->getPushback()), Vector2(0,0));
			} else {
				this->ApplyLinearImpulse(Vector2(-p->getPushback(), p->getPushback()), Vector2(0,0));
			}
		}
		else {
			this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
		}
	} else if (m->getAttribute("type") == "Hero") {
		Game::stopRunning(this);
		if (static_cast<Hero*>(m)->getCharging() == true) {
			this->takeDamage(static_cast<Characters*>(m)->getWeapon()->getDamage(), static_cast<Characters*>(m)->getWeapon()->getCritRate());
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
		if (this->GetBody()->GetWorldCenter().x > m->GetBody()->GetWorldCenter().x) {
			this->ApplyLinearImpulse(Vector2(5, 5), Vector2(0,0));
		} else {
			this->ApplyLinearImpulse(Vector2(-5, 5), Vector2(0,0));
		}
	}
}

void	Enemy::EndContact(Elements *m, b2Contact *contact) {
	Characters::EndContact(m, contact);
	if (!m)
		return ;
	if (m->getAttribute("type") == "ground" && !this->_isDead && !this->_isTakingDamage && Game::stopPattern == false) {
		if (this->_lastElement != m->getId()) {
			this->_pattern->tick(Game::currentGame->getCurrentMap());
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
int		Enemy::takeDamage(int damage, int critRate) {
	if (this->_hp <= 0)
		return 0;
	this->actionCallback("takeDamage", 0);
	if (critRate != 0) {
		if ((rand() % critRate + 1) ==  critRate) {
			damage *= 2 ;
			Game::getHUD()->setText("Crit ! (" + std::to_string(damage) + ")", this,
									Vector3(255, 0, 0), 1, 0);
		} else {
			// The following display damage with any hit. This is debug code, don't forget to remove it with beta.
			Game::getHUD()->setText(std::to_string(damage), this,
									Vector3(1, 1, 1), 1, 0);
		}
	}
	if (this->_hp - damage <= 0) {
		Game::stopRunning(this);
		this->GetBody()->SetLinearVelocity(b2Vec2(0,0));
		Game::currentGame->maps->_XYMap[Game::currentY][Game::currentX].removeEnemy(this);
		this->actionCallback("death", 0);
		this->_setCategory("death");
		theSwitchboard.SubscribeTo(this, "setToStatic" + this->GetName());
		theSwitchboard.Broadcast(new Message("setToStatic" + this->GetName()));
		this->GetBody()->GetFixtureList()->SetDensity(0);
		this->GetBody()->ResetMassData();
		this->LoadSpriteFrames(this->_getAttr("newSprites").asString());
		this->changeSizeTo(Vector2(this->_getAttr("size").asInt(),
								   this->_getAttr("size").asInt()));
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
								  this->_getAttr("beginFrame").asInt(),
								  this->_getAttr("endFrame").asInt());
		theSwitchboard.SubscribeTo(this, "destroyEnemy" + this->GetName());
		theSwitchboard.DeferredBroadcast(new Message("destroyEnemy" + this->GetName()), 0.5);
		theSwitchboard.Broadcast(new Message(std::to_string(this->getId())));
		this->_hp = 0;
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
