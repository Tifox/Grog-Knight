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
 * File: Hero.cpp
 * Creation: 2015-02-14 10:49
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "Hero.hpp"

//! Constructor
/**
 * This constructor is making some subscribtions for himself.
 */
Hero::Hero(std::string name) : Characters(name) {
	theSwitchboard.SubscribeTo(this, "canMove");
	theSwitchboard.SubscribeTo(this, "endInvincibility");
	theSwitchboard.SubscribeTo(this, "enableAttackHitbox");
	theSwitchboard.SubscribeTo(this, "disableAttackHitbox");
	theSwitchboard.SubscribeTo(this, "equipSelectedItem");
	theSwitchboard.SubscribeTo(this, "dropItem");
	theSwitchboard.SubscribeTo(this, "attackReady");
	theSwitchboard.SubscribeTo(this, "speMoveReady");
	theSwitchboard.SubscribeTo(this, "speAttReady");
	theSwitchboard.SubscribeTo(this, "changeCharacter");
	theSwitchboard.SubscribeTo(this, "lockTarget");
	theSwitchboard.SubscribeTo(this, "unlockTarget");
	theSwitchboard.SubscribeTo(this, "cycleInventory");
	theSwitchboard.SubscribeTo(this, "chooseItem1");
	theSwitchboard.SubscribeTo(this, "chooseItem2");
	theSwitchboard.SubscribeTo(this, "chooseItem3");
	theSwitchboard.SubscribeTo(this, "chooseItem4");
	theSwitchboard.SubscribeTo(this, "endStomp");
	this->addAttribute("type", "Hero");
	this->_eqMove = new SpecialMoves(static_cast<Characters*>(this));
	this->_eqAtt = new SpecialAttack(static_cast<Characters*>(this));

	this->_inventory = new Inventory(this->_getAttr("starting", "inventorySlots").asInt());
	return ;
}

Hero::Hero(Hero *obj) : Characters(obj->getAttribute("class")) {
	theSwitchboard.SubscribeTo(this, "canMove");
	theSwitchboard.SubscribeTo(this, "endInvincibility");
	theSwitchboard.SubscribeTo(this, "enableAttackHitbox");
	theSwitchboard.SubscribeTo(this, "disableAttackHitbox");
	theSwitchboard.SubscribeTo(this, "equipSelectedItem");
	theSwitchboard.SubscribeTo(this, "dropItem");
	theSwitchboard.SubscribeTo(this, "attackReady");
	theSwitchboard.SubscribeTo(this, "speMoveReady");
	theSwitchboard.SubscribeTo(this, "speAttReady");
	theSwitchboard.SubscribeTo(this, "changeCharacter");
	theSwitchboard.SubscribeTo(this, "lockTarget");
	theSwitchboard.SubscribeTo(this, "unlockTarget");
	theSwitchboard.SubscribeTo(this, "cycleInventory");
	theSwitchboard.SubscribeTo(this, "chooseItem1");
	theSwitchboard.SubscribeTo(this, "chooseItem2");
	theSwitchboard.SubscribeTo(this, "chooseItem3");
	theSwitchboard.SubscribeTo(this, "chooseItem4");
	theSwitchboard.SubscribeTo(this, "endStomp");
	this->addAttribute("type", "Hero");

	delete obj->getEqMove();
	delete obj->getEqAtt();
	this->_eqMove = new SpecialMoves(static_cast<Characters*>(this));
	this->_eqAtt = new SpecialAttack(static_cast<Characters*>(this));

	this->_inventory = obj->getInventory();
}

//! Destructor
/**
 * Basic Destructor
 */
Hero::~Hero(void) {
	return ;
}

//! Init Animation function
/**
 * This function making the first animation call.
 */
void	Hero::init(void) {
	this->AnimCallback("base");
}

//! Action callback function
/**
 * Mother's callback for actions
 * See Characters::ReceiveMessage for more information.
 * @param name The action name
 * @param status The key status (1 | 0)
 * @sa Characters::ReceiveMessage
 * @todo Extract the values from the consumable
 */
void	Hero::actionCallback(std::string name, int status) {
	std::string 	orientation;
	std::string 	o2;
	float				x = 2, y = 1;
	std::string type = this->_weapon->getType();

	if (this->_latOrientation == RIGHT) {
		orientation = "Right";
	} else if (this->_latOrientation == LEFT) {
		orientation = "Left";
	}
	this->_setCategory(type);
	if (name == "attack" && status == 0 && this->_canAttack == true &&
		this->_fullChargedAttack == false && this->_isLoadingAttack == 0 &&
		this->_isAttacking == 1) {
		this->_canAttack = false;
		this->changeSizeTo(Vector2(this->_getAttr("x").asFloat(), this->_getAttr("y").asFloat()));
		if (this->_orientation == UP) {
			this->changeSizeTo(Vector2(this->_getAttr("upX").asFloat(), this->_getAttr("upY").asFloat()));
			o2 = "Up";
		} else if (this->_orientation == DOWN) {
			this->changeSizeTo(Vector2(this->_getAttr("downX").asFloat(), this->_getAttr("downY").asFloat()));
			o2 = "Down";
		}
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
								  this->_getAttr("beginFrame" + orientation + o2).asInt(),
								  this->_getAttr("endFrame" +  orientation + o2).asInt(), "base");
	} else if (name == "attack" && status == 0 &&
			   this->_canAttack == true &&
			   this->_fullChargedAttack == true) {
		this->_isLoadingAttack = 0;
		this->_fullChargedAttack = false;
		this->changeSizeTo(Vector2(this->_getAttr("loadX").asFloat(), this->_getAttr("loadY").asFloat()));
		this->_canAttack = false;
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
								  this->_getAttr("holdFrameLoad" + orientation).asInt(),
								  this->_getAttr("endFrameLoad" + orientation).asInt(), "base");
	} else if (name == "loadAttack_charge") {
//			else if (type == "Axe")
//				this->changeSizeTo(Vector2(2, 1.5));
//	}
		this->changeSizeTo(Vector2(this->_getAttr("loadX").asFloat(), this->_getAttr("loadY").asFloat()));
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
								  this->_getAttr("beginFrameLoad" + orientation).asInt(),
								  this->_getAttr("holdFrameLoad" + orientation).asInt());
	} else if (name == "dash") {
		if (this->_latOrientation == RIGHT) {
			orientation = "right";
		} else if (this->_latOrientation == LEFT)
			orientation = "left";
		this->changeSizeTo(Vector2(2, 1));
		this->_setCategory("dash");
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
								  this->_getAttr("beginFrame_" + orientation).asInt(),
								  this->_getAttr("endFrame_" + orientation).asInt() - 2, "endDash");
	} else if (name == "stomp") {
		if (this->_latOrientation == RIGHT) {
			orientation = "right";
		} else if (this->_latOrientation == LEFT)
			orientation = "left";
		this->PlaySpriteAnimation(this->_getAttr("stomp", "time").asFloat(), SAT_OneShot,
								  this->_getAttr("stomp", "beginFrame_" + orientation).asInt(),
								  this->_getAttr("stomp", "endFrame_" + orientation).asInt() - 2, "base");
	}
	return;
}

//! Begin collision function
/**
 * Collision begin callback
 * /!\ This function is called just before a collision
 * @param elem The other Element
 * @param contact The Box2D contact object
 */
void	Hero::BeginContact(Elements* elem, b2Contact *contact) {
	Characters::BeginContact(elem, contact);
	if (!elem)
		return ;
	if (elem->getAttribute("type") == "Enemy" && elem->isDead() == false) {
		if (this->_invincibility == false)
			this->_takeDamage(elem);
		else {
			this->_enemiesTouched.push_back(elem);
			if (this->_isStomping == true) {
				this->GetBody()->SetLinearVelocity(b2Vec2(0, 3));
				theSwitchboard.Broadcast(new Message("stompEnd"));
				new Weapon(this->_weapon, this, 1, "stompArea");
				new Weapon(this->_weapon, this, -1, "stompArea");
			}
		}
	} else if (elem->getAttribute("type") == "projectile" && elem->getAttribute("spe") != "throwSpear") {
		this->_takeDamage(elem);
	}
	else if (elem->getAttribute("type") == "Object") {
		if (elem->getAttribute("type2") == "Consumable") {
			if (elem->getAttribute("type3") == "HP") {
				if (this->_hp != this->_maxHp) {
					elem->ChangeColorTo(Color(0,0,0,0), 0);
					Game::addToDestroyList(elem);
					this->setHP(this->getHP() + stoi(elem->getAttribute("value")));
					Game::currentGame->tooltip->tip(elem, this);
					Game::getHUD()->life(this->getHP());
				}
			} if (elem->getAttribute("type3") == "gold") {
				elem->ChangeColorTo(Color(0,0,0,0), 0);
				Game::addToDestroyList(elem);
				this->_gold += stoi(elem->getAttribute("value"));
				Game::currentGame->tooltip->tip(elem, this);
				Game::getHUD()->updateGold(this->getGold());
			}
		}
		else if (elem->getAttribute("type2") == "Equipment") {
			Game::currentGame->tooltip->clearInfo();
			Game::currentGame->tooltip->info(elem);
			this->_item = elem;
		}
	}
	else if (elem->getAttribute("boss") == "true") {
		this->_takeDamage(elem);
	}
	if (elem->getAttribute("type") == "ground" &&
		elem->getAttribute("speType") == "spikes") {
		if (this->_isStomping == true) {
			theSwitchboard.Broadcast(new Message("stompEnd"));
			this->_invincibility = false;
		}
		if (this->_invincibility == false)
			this->_takeDamage(elem);
		else
			this->_enemiesTouched.push_back(elem);
	}
	else if (elem->getAttribute("type") == "shopItem") {
		Game::currentGame->tooltip->clearInfo();
		Game::currentGame->tooltip->info(elem);
		this->_shopItem = elem->getAttribute("name");
		this->_shopItemNumber = atoi(elem->getAttribute("number").c_str());
		this->_shopItemPrice = atoi(elem->getAttribute("price").c_str());
		Game::currentGame->getShopkeeper()->displayText("This one is for " + std::to_string(this->_shopItemPrice) + "g");
	} else if (elem->getAttribute("type") == "Chest") {
		this->_isTouchingChest = true;
	} else if (elem->getAttribute("type") == "Door") {
		if (elem->getAttribute("type2") == "Boss") {
			this->_isTouchingBossDoor = true;
		} else if (elem->getAttribute("type2") == "Secret") {
			this->_isTouchingSecretDoor = true;
		}
	} else if (elem->getAttribute("type") == "Dealer") {
		this->_isTouchingDealer = true;
	}
}

//! End collision function
/**
 * End contact callback.
 * /!\ This function is actually called after a collision.
 * @param elem The other Element.
 * @param contact The Box2D contact object
 */
void	Hero::EndContact(Elements *elem, b2Contact *contact) {
	Characters::EndContact(elem, contact);
	if (!elem)
		return ;
	if (elem->getAttribute("type") == "Object") {
		if (elem->getAttributes()["type2"] == "Equipment") {
			this->_item = nullptr;
		}
	}
	if (elem->getAttribute("type") == "shopItem") {
		Game::currentGame->tooltip->clearInfo(0);
		this->_shopItem = "";
		this->_shopItemNumber = 0;
		this->_shopItemPrice = 0;
		Game::currentGame->getShopkeeper()->removeText();
	}
	if (elem->getAttribute("type") == "Enemy" ||
		elem->getAttribute("speType") == "spikes") {
		this->_enemiesTouched.remove(elem);
	}
	if (elem->getAttribute("type2") == "Equipment") {
		Game::currentGame->tooltip->clearInfo(0);
	}
	if (elem->getAttribute("type") == "Chest") {
		this->_isTouchingChest = false;
	} else if (elem->getAttribute("type") == "Door") {
		if (elem->getAttribute("type2") == "Boss") {
			this->_isTouchingBossDoor = false;
		} else if (elem->getAttribute("type2") == "Secret") {
			this->_isTouchingSecretDoor = false;
		}
	}
	if (elem->getAttribute("type") == "Dealer") {
		this->_isTouchingDealer = false;
	}
}

//! Function called when the hero is taking damage
/**
 * Called by BeginContact mostly
 * @param the elem that has damaged hero
 * @todo monster damage should not be hard-written to 25
 */
void	Hero::_takeDamage(Elements* elem) {
	int damage;

	this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
	Game::stopRunning(this);
	this->_isRunning = 0;
	this->_isLoadingAttack = 0;
	this->_isAttacking = 0;
	this->_fullChargedAttack = false;
	this->_attackPressed = 0;
	this->_isJump = 1;
	if (elem->getAttribute("speType") == "spikes") {
		damage = 25;
	} else if (elem->getAttribute("boss") == "true")
		damage = 0;
	else
		damage = atoi(elem->getAttribute("damage").c_str());
	damage -= this->buff.dmgReduc;
	if (damage < 0)
		damage = 0;
	if (this->getAttribute("class") == "Warrior")
		this->changeSizeTo(Vector2(1, 1));
	if (this->_invincibility == false) {
		this->_canMove = 0;
		this->setHP(this->getHP() - damage);
		Game::getHUD()->setText(std::to_string(damage), this, Vector3(1, 0, 0), 1, 0);
		theSwitchboard.DeferredBroadcast(new Message("canMove"), 0.4f);
		theSwitchboard.DeferredBroadcast(new Message("endInvincibility"), 1.5f);
		Game::getHUD()->life(this->getHP());
	}
	if (this->GetBody()->GetWorldCenter().x >= elem->GetBody()->GetWorldCenter().x) {
		this->ApplyLinearImpulse(Vector2(4, 4), Vector2(0, 0));
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
								  this->_getAttr("takeDamage", "beginFrame_left").asInt(),
								  this->_getAttr("takeDamage", "endFrame_left").asInt(),
								  "takeDamage");
	}
	else if (this->GetBody()->GetWorldCenter().x < elem->GetBody()->GetWorldCenter().x) {
		this->ApplyLinearImpulse(Vector2(-4, 4), Vector2(0, 0));
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
								  this->_getAttr("takeDamage", "beginFrame_right").asInt(),
								  this->_getAttr("takeDamage", "endFrame_right").asInt(),
								  "takeDamage");
	}
	this->SetColor(1,0,0,0.8f);
	theSwitchboard.SubscribeTo(this, "colorDamageBlink1");
	theSwitchboard.SubscribeTo(this, "colorDamageBlink2");
	theSwitchboard.DeferredBroadcast(new Message("colorDamageBlink1"), 0.1f);
	this->_invincibility = true;
}

void	Hero::setStartingValues(Hero *tmp) {
	this->_setCategory("starting");
	if (tmp == nullptr) {
		this->equipWeapon(Game::menuCharacter->getWeapon());
		this->equipArmor(Game::menuCharacter->getArmor());
		this->equipRing(Game::menuCharacter->getRing());
		this->_speMove = Game::menuCharacter->getSkills()[0];
		this->_speAtt = Game::menuCharacter->getSkills()[1];
	} else {
		this->equipWeapon(tmp->getWeapon());
		this->equipArmor(tmp->getArmor());
		this->equipRing(tmp->getRing());
		this->_speMove = tmp->getSpeMove();
		this->_speAtt = tmp->getSpeAtt();
	}
   /* if (Game::menuCharacter->getSkills()[0] != "") {*/
		//Log::info("Special Move: " + Game::menuCharacter->getSkills()[0]);
	//}
	//if (Game::menuCharacter->getSkills()[1] != "") {
		//Log::info("Special Attack: " + Game::menuCharacter->getSkills()[1]);
	/*}*/
}

Inventory	*Hero::getInventory(void) { return this->_inventory; };
SpecialMoves *Hero::getEqMove(void) { return this->_eqMove; };
SpecialAttack *Hero::getEqAtt(void) { return this->_eqAtt; };
void		Hero::setInventory(Inventory *i) { this->_inventory = i; };
