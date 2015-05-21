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
 * File: Bonus.cpp
 * Creation: 2015-05-20 12:49
 * Manon Budin <mbudin@student.42.fr>
 */

 #include "Bonus.hpp"

 Bonus::Bonus(BonusType type, float buffAmount) : _type(type), _buffAmount(buffAmount) {
 	return;
 }

 Bonus::~Bonus(void) {
 	return;
 }

Bonus::BonusType		Bonus::parseType(std::string type)
{
	if (type == "hp_buff") { 
		return BonusType::HP_BUFF;
	} else if (type == "mana_buff") {
		return BonusType::MANA_BUFF;
	} else if (type == "speed_buff") {
		return BonusType::SPEED_BUFF;
	}
	return BonusType::NONE;
}

/* GETTERS */

float 					Bonus::getAmount(void) { return this->_buffAmount; }
Bonus::BonusType 		Bonus::getType(void) { return this->_type; }

