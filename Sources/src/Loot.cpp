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
 * File: Loot.cpp
 * Creation: 2015-04-14 14:49
 * Vincent Rey <vrey@student.42.fr>
 */

# include "Loot.hpp"

//! Constructor
/**
 * Constructor taking an enemy as argument to determine the kind of loot created
 * Currently: checks first if drops a consumable - if no consumable checks for
 * equipment drop
 */

Loot::Loot(Characters* c) {
	// int i;
	// if (rand() % 100 <= c->_getAttr("loot", "consumableRate").asInt()) {
	// 	if (rand() % 2 == 0) {
	 //		new Consumable("HP", c->_getAttr("loot", "HPReward").asString(), c);
	//    } else
	//		new Consumable("gold", c->_getAttr("loot", "XPReward").asString(), c);
	// } else if (rand() % 100 <= c->_getAttr("loot", "equipmentRate").asInt()) {
	// int rant;
	// rant = rand() % 3;
	// 	if (rant == 1)
	 		new Equipment(Game::wList->getWeaponRandom(c->_getAttr("loot", "EqReward").asInt()), c);
	// 	else if (rant == 0)
	//  		new Equipment(Game::aList->getArmorRandom(c->_getAttr("loot", "EqReward").asInt()), c);
	// 	else
	// 		new Equipment(Game::rList->getRingRandom(c->_getAttr("loot", "EqReward").asInt()), c);
	// }
}

Loot::Loot(Characters* c, Weapon *toDrop) {
		new Equipment(Game::wList->getWeapon(toDrop->getName()), c);
}

Loot::Loot(Characters* c, Armor *toDrop) {
		new Equipment(Game::aList->getArmor(toDrop->getName()), c);
}

Loot::Loot(Characters* c, Ring *toDrop) {
		new Equipment(Game::rList->getRing(toDrop->getName()), c);
}
