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
 * File: Boss.cpp
 * Creation: 2015-09-29 18:09
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

#include "../inc/Boss.hpp"

Boss::Boss(std::string name, int x, int y) : Characters("Bosses/" + name), _inactive(0) {
	this->setXStart(x);
	this->setYStart(y + 1);
	this->addAttribute("hitbox", "circle");
	this->addAttribute("boss", "true");
	this->display();
	this->_h = Game::getHUD();
	this->AnimCallback("idle");
	this->_h->setText(this->_name, theCamera.GetWindowWidth() / 2 - this->_name.size() * 3, 
		theCamera.GetWindowHeight() / 5 , Vector3(1, 1, 1), 1);
	this->lifeBar();
	theSwitchboard.SubscribeTo(this, "attack");
	theSwitchboard.SubscribeTo(this, "phase1");
	theSwitchboard.SubscribeTo(this, "phase2");
}

Boss::~Boss(void) {
	;
}

void		Boss::ReceiveMessage(Message *m) {
	float	percent = this->_hp * 100 / this->_maxHp;
	int		x = this->GetBody()->GetWorldCenter().x, y = this->GetBody()->GetWorldCenter().y, size = this->GetSize().X / 2 + 1;

	if (m->GetMessageName() == "attack") {
		if (percent > 50)
			this->ReceiveMessage(new Message("phase1"));
		else 
			this->ReceiveMessage(new Message("phase2"));
	} else if (m->GetMessageName() == "phase1") {
		new Projectile("Resources/Images/boss_projectile.png", 20, Vector2((x + size), y), Vector2(2, 0), Vector2(-1, 0), "bossProjectile");
		new Projectile("Resources/Images/boss_projectile.png", 20, Vector2(x, (y + size)), Vector2(0, 2), Vector2(0, 0), "bossProjectile");
		new Projectile("Resources/Images/boss_projectile.png", 20, Vector2((x - size), y), Vector2(-2, 0), Vector2(0, 0), "bossProjectile");
		new Projectile("Resources/Images/boss_projectile.png", 20, Vector2(x, (y - (size + 1))), Vector2(0, -2), Vector2(0, 0), "bossProjectile");
		theSwitchboard.DeferredBroadcast(new Message("attack"), 0.5);
	} else if (m->GetMessageName() == "phase2") {
		switch (this->_flag) {
			case 0: this->createProjectile(Vector2(x, (y + size)), Vector2(0, 2)); break;
			case 1: this->createProjectile(Vector2(x + size, (y + size)), Vector2(1, 1)); break;
			case 2: this->createProjectile(Vector2(x + size, (y)), Vector2(2, 0)); break;
			case 3: this->createProjectile(Vector2(x + size, (y - size)), Vector2(1, -1)); break;
			case 4: this->createProjectile(Vector2(x, (y - size - 1)), Vector2(0, -2)); break;
			case 5: this->createProjectile(Vector2(x - size, (y - size)), Vector2(-1, -1)); break;
			case 6: this->createProjectile(Vector2(x - size, (y)), Vector2(-2, 0)); break;
			case 7: this->createProjectile(Vector2(x - size, (y + size)), Vector2(-1, 1)); break;
		}
		this->_flag = (this->_flag == 7 ? 0 : this->_flag + 1);
		theSwitchboard.DeferredBroadcast(new Message("attack"), 0.05);
	}
}

void		Boss::createProjectile(Vector2 force, Vector2 init) {
	new Projectile("Resources/Images/boss_projectile.png", 20, force, init, Vector2(0, 0), "bossProjectile");
}

void		Boss::AnimCallback(String s) {
	if (s == "idle") {
		this->_setCategory("breath");
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot, 
			this->_getAttr("beginFrame").asInt(), this->_getAttr("endFrame").asInt(), "reverseIdle");
	} else if (s == "reverseIdle") {
		this->_setCategory("breath");
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot, 
			this->_getAttr("endFrame").asInt(), this->_getAttr("beginFrame").asInt(), "idle");
	}
}

void		Boss::BeginContact(Elements *elem, b2Contact *contact) {
	Weapon* w = static_cast<Weapon*>(elem);
	Projectile* p = static_cast<Projectile*>(elem);
	Characters *h = Game::currentGame->getHero();
	std::string		type = elem->getAttribute("type");
	int				damage = 0, crit;

	this->_lastHitID = (this->_lastHitID == elem->getId() ? -1 : elem->getId());
	if (this->_lastHitID == -1)
		return ;
	if (type == "HeroWeaponHitBox") {
		damage = w->getDamage(); crit = w->getCritRate();
	} else if (type == "HeroProjectile") {
		damage = p->getDamage(); crit = p->getCritRate();
	} else if (type == "ShockWave") {
		damage = p->getDamage(); crit = 0;
	} else if (type == "Hero" && h->getCharging() == true) {
		damage = h->getWeapon()->getDamage(); crit = h->getWeapon()->getCritRate();
	}
	if (damage) {
		if (!this->_inactive) {
			theSwitchboard.Broadcast(new Message("attack"));
			this->_inactive = 1;
		}
		damage += h->buff.bonusDmg;
		crit += h->buff.critBuff;
		if (crit && (rand() % (crit + 1) == crit)) {
			damage *= 2;
			Game::getHUD()->setText("Crit ! (" + std::to_string(damage) + ")", this, Vector3(255, 0, 0), 1, 0);
		} else
			Game::getHUD()->setText(std::to_string(damage), this, Vector3(1, 1, 1), 1, 0);
		this->_hp -= damage;
		this->_setCategory("jump");
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot, 
			this->_getAttr("endFrame").asInt(), this->_getAttr("beginFrame").asInt(), "idle");
		this->lifeBar();
	}
}

void		Boss::EndContact(Elements *elem, b2Contact *contact) {
	;
}

void		Boss::lifeBar(void) {
	int		x, y, j;
	float	percent;
	std::list<HUDActor *>::iterator		it;
	std::string							tmp;

	x = theCamera.GetWindowWidth() / 2 - (100);
	y = theCamera.GetWindowHeight() / 5 - 5;
	for (it = this->_lifeList.begin(); it != this->_lifeList.end(); it++)
		theWorld.Remove(*it);
	this->_lifeList.clear();
	percent = this->_hp * 100 / this->_maxHp;
	if (percent > 0) {
		for (j = 0; j <= percent; j++, x += 2) {
			if (j == 0) { tmp = "begin"; }
			else if (j == percent) { tmp = "end"; }
			else { tmp = "middle"; }
			this->_lifeList.push_back(this->_h->addImage("Resources/Images/HUD/hp_" + tmp + ".png", x, y, Vector2(2, 15), 100));
		}
		if (percent < 100) {
			for (; percent < 100; percent++, x += 2) {
				this->_lifeList.push_back(this->_h->addImage("Resources/Images/HUD/hp_empty_middle.png", x, y, Vector2(2, 15), 100));
			}
			this->_lifeList.push_back(this->_h->addImage("Resources/Images/HUD/hp_empty_end.png", x, y, Vector2(2, 15), 100));
		}
	}
}
