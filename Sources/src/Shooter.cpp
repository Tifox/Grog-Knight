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
 * File: Shooter.cpp
 * Creation: 2015-02-23 16:14
 * Matthieu Maudet <mmaudet@student.42.fr>
 */

# include "../inc/Shooter.hpp"
# include "../inc/Projectile.hpp"
# include "../inc/Elements.hpp"


/*
** Default constructor
*/

Shooter::Shooter(void) {
	this->_damage = 10;
	this->_rate = 1;
	this->_orientation = 1;
	this->_straight = true;
	this->_spriteName = "PleaseChangeMe";
}

Shooter::~Shooter(void) {}

void Shooter::fire(int x, int y) {
	std::cout << "FIRE !" << std::endl;
	Projectile *projectile = new Projectile(x, y);
}
