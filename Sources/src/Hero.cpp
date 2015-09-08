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
	theSwitchboard.SubscribeTo(this, "changeCharacter");
	theSwitchboard.SubscribeTo(this, "lockTarget");
	theSwitchboard.SubscribeTo(this, "unlockTarget");
	theSwitchboard.SubscribeTo(this, "cycleInventory");
	theSwitchboard.SubscribeTo(this, "chooseItem1");
	theSwitchboard.SubscribeTo(this, "chooseItem2");
	theSwitchboard.SubscribeTo(this, "chooseItem3");
	theSwitchboard.SubscribeTo(this, "chooseItem4");
	this->addAttribute("type", "Hero");
	this->_inventory = new Inventory(this->_getAttr("starting", "inventorySlots").asInt());
	return ;
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
	float				x = 2, y = 1;
	if (name == "attack" && status == 0 && this->_canAttack == true &&
		this->_fullChargedAttack == false && this->_isLoadingAttack == 0 &&
		this->_isAttacking == 1) {
		this->_canAttack = false;
		if (this->_orientation == RIGHT) {
			orientation = "right";
		} else if (this->_orientation == LEFT) {
			orientation = "left";
		} else if (this->_orientation == UP) {
			x = 1.5f; y = 2;
			orientation = "up";
		} else if (this->_orientation == DOWN) {
			x = 1; y = 2.5f;
			orientation = "down";
		}
		if (this->getAttribute("class") == "Warrior")
			this->changeSizeTo(Vector2(x, y));
		this->_setCategory("attack");

		std::string type = this->_weapon->getType();

		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
								  this->_getAttr("beginFrame" + type + "_" + orientation).asInt(),
								  this->_getAttr("endFrame" + type + "_" + orientation).asInt(), "base");

	} else if (name == "attack" && status == 0 &&
			   this->_canAttack == true &&
			   this->_fullChargedAttack == true) {
		if (this->_orientation == RIGHT) {
			orientation = "right";
		} else if (this->_orientation == LEFT) {
			orientation = "left";
		} else if (this->_orientation == UP) {
			x = 1.5f; y = 2;
			orientation = "up";
		} else if (this->_orientation == DOWN) {
			x = 1; y = 2.5f;
			orientation = "down";
		}
		this->_setCategory("loadAttack_done");
		this->_isLoadingAttack = 0;
		this->_fullChargedAttack = false;
		if (this->getAttribute("class") == "Warrior")
			this->changeSizeTo(Vector2(2, 2));
		this->_canAttack = false;
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
								  this->_getAttr("beginFrame_" + orientation).asInt(),
								  this->_getAttr("endFrame_" + orientation).asInt(), "base");
	} else if (name == "loadAttack_charge") {
		if (this->_latOrientation == RIGHT) {
			orientation = "right";
		} else if (this->_latOrientation == LEFT)
			orientation = "left";
		this->_setCategory("loadAttack_charge");
		if (this->getAttribute("class") == "Warrior")
			this->changeSizeTo(Vector2(2, 2));
		this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
								  this->_getAttr("beginFrame_" + orientation).asInt(),
								  this->_getAttr("endFrame_" + orientation).asInt());
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
	if (elem->getAttribute("type") == "Enemy" && elem->isDead() == false) {
		if (this->_invincibility == false)
			this->_takeDamage(elem);
		else {
			this->_enemiesTouched.push_back(elem);
			if (this->_isStomping == true) {
				this->GetBody()->SetLinearVelocity(b2Vec2(0, 3));
				theSwitchboard.Broadcast(new Message("stompEnd"));
				new Weapon(this->_weapon, this, 1);
				new Weapon(this->_weapon, this, -1);
			}
		}
	}
	else if (elem->getAttribute("type") == "Object") {
		if (elem->getAttribute("type2") == "Consumable") {
			if (elem->getAttribute("type3") == "HP") {
				if (this->_hp != this->_maxHp) {
					Game::addToDestroyList(elem);
					this->setHP(this->getHP() + stoi(elem->getAttribute("value")));
					Game::currentGame->tooltip->tip(elem, this);
					Game::getHUD()->life(this->getHP());
				}
			} else if (elem->getAttribute("type3") == "mana") {
				if (this->_mana < this->_maxMana) {
					Game::addToDestroyList(elem);
					this->setMana(this->getMana() + stoi(elem->getAttribute("value")));
					Game::currentGame->tooltip->tip(elem, this);
					Game::getHUD()->mana(this->getMana());
				}
			}
			if (elem->getAttribute("type3") == "gold") {
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
	if (elem->getAttribute("type") == "ground" &&
		elem->getAttribute("speType") == "spikes") {
		if (this->_isStomping == true) {
			theSwitchboard.Broadcast(new Message("stompEnd"));
			this->_invincibility = false;
			new Weapon(this->_weapon, this, 1);
			new Weapon(this->_weapon, this, 1);
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
			std::cout << "not here" << std::endl;
		} else if (elem->getAttribute("type2") == "Secret") {
			std::cout << "yes pls" << std::endl;
			this->_isTouchingSecretDoor = true;
		}
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
}

//! Function called when the hero is taking damage
/**
 * Called by BeginContact mostly
 * @param the elem that has damaged hero
 * @todo monster damage should not be hard-written to 25
 */
void	Hero::_takeDamage(Elements* elem) {
  this->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
  Game::stopRunning(this);
  this->_isRunning = 0;
  this->_isLoadingAttack = 0;
  this->_isAttacking = 0;
  this->_fullChargedAttack = false;
  this->_attackPressed = 0;
  this->_isJump = 1;
  if (this->getAttribute("class") == "Warrior")
	  this->changeSizeTo(Vector2(1, 1));
  if (this->_invincibility == false) {
	  this->_canMove = 0;
	  this->setHP(this->getHP() - 25);
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

void	Hero::setStartingValues(void) {
	this->_setCategory("starting");
	this->equipWeapon(Game::menuCharacter->getWeapon());
	this->equipArmor(Game::menuCharacter->getArmor());
	this->equipRing(Game::menuCharacter->getRing());
	this->_speMove = this->_getAttr("specialMove").asString();
}
