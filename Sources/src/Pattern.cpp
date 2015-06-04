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
 * File: Pattern.cpp
 * Creation: 2015-05-03 10:22
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "Pattern.hpp"

//! Basic constructor
/**
 * This a void constructor, useless ftm.
 */
Pattern::Pattern(void) {

}

//! Main constructor
/**
 * That's the main constructor for a pattern, using the pattern name in argue.
 * @param name The name of the Pattern.
 */
Pattern::Pattern(std::string name) : _name(name), _orientation(1) {
	;
}

//! Copy constructor
/**
 * Copy constructor of Pattern (Coplien o/)
 */
Pattern::Pattern(Pattern & p) {
	this->_name = p.getName();
	this->_enemy = p.getEnemy();
}

//! Destructor
Pattern::~Pattern(void) {

}

//! Basic pattern tick.
/**
 * This function is virtual, just here for structure purposes.
 * If you looking for the actual code of a pattern, go if the Pattern child itself.
 */
void	Pattern::tick(Map m) {
	this->_x = (this->_enemy->GetBody()->GetWorldCenter().x + 0.5) - m.getXStart();
	this->_y = -((this->_enemy->GetBody()->GetWorldCenter().y - 0.5) - m.getYStart());
}

/* SETTERS */

void	Pattern::setEnemy(Enemy *e) { this->_enemy = e; };

/* GETTERS */

std::string		Pattern::getName(void) { return this->_name; };
Enemy			*Pattern::getEnemy(void) { return this->_enemy; };
