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
 * File: Elements.cpp
 * Creation: 2015-02-13 07:39
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "../inc/Elements.hpp"

/**
 * Main constructor
 */
Elements::Elements(void) : PhysicsActor() {
	this->setId(Game::getNextId());
	std::cout << "L" << this->getId() << std::endl;
	Game::addElement(*this);
	return ;
}

/**
 * Main constructor
 */
Elements::Elements(int id) :  PhysicsActor() {
	this->setId(Game::getNextId());
	std::cout << ";" << this->getId() << std::endl;
	Game::addElement(*this);
	return ;
}

/**
 * Copy constructor
 * @param: obj (Elements & obj)
 */
Elements::Elements(Elements & obj) : PhysicsActor(obj) {
	this->_attributes = obj.getAttributes();
	this->setId(Game::getNextId());
	Game::addElement(*this);
	std::cout << "'" << this->getId() << std::endl;
}

/**
 * Basic destructor
 */
Elements::~Elements(void) {
}

/**
 * Add an attribute to the Element
 * @param: name (std::string)
 * @param: value (std::string)
 */
void	Elements::addAttribute(std::string name, std::string value) {
	this->_attributes[name] = value;
}

/**
 * Get an attribute value
 * @param: name (std::string)
 * @return: this->_attributes[name]
 */
std::string	Elements::getAttribute(std::string name) {
	if (this->_attributes.find(name) != this->_attributes.end())
		return this->_attributes[name];
	return "";
}

/**
 * Display the Element on the screen
 */
void	Elements::display(void) {
	this->SetPosition(this->_XStartPos, this->_YStartPos);
	this->Tag(this->getAttribute("type"));
	//std::cout << "Add an object at: " << this->_XStartPos << ", " << this->_YStartPos << std::endl;
	this->Tag(this->getAttribute("type"));
	if (this->getAttribute("sprite") != "")
		this->SetSprite(this->getAttribute("sprite"));
	else if (this->getAttribute("spritesFrame") != "") {
		this->LoadSpriteFrames(this->getAttribute("spritesFrame"));
	} else
		this->SetColor(0, 0, 0, 0);
	this->SetSize(1.0f);
	this->SetDrawShape(ADS_Square);
	if (this->getAttribute("type") == "wall" || this->getAttribute("type") == "ground") {
		this->SetDensity(0);
		this->SetFriction(1);
		this->SetRestitution(0);
	}
	if (this->getAttribute("hero") == "1") {
		this->SetShapeType(PhysicsActor::SHAPETYPE_HERO);
	} else {
		this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	}

	if (this->getAttribute("physic") != "")
		this->InitPhysics();
	theWorld.Add(this);
}

/* SETTERS */

void	Elements::setXStart(float X) { this->_XStartPos = X; };
void	Elements::setYStart(float Y) { this->_YStartPos = Y; };

/* GETTERS */
std::map<std::string, std::string>	Elements::getAttributes(void) { return this->_attributes; };
int									Elements::getOrientationX(void) { return this->_orientation; }
int									Elements::getOrientationY(void) { return this->_up; }
