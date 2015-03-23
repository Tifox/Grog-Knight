
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
Elements::Elements(int id) :  PhysicsActor() {
	this->setId(Game::getNextId());
	Game::addElement(*this);
	return ;
}

/**
 * Main constructor
 */
Elements::Elements(void) :  PhysicsActor() {
	this->setId(Game::getNextId());
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
 * Remove an attribute form the list
 * @param: name (std::string)
 */
void	Elements::removeAttr(std::string name) {
	if (this->_attributes.find(name) != this->_attributes.end())
		this->_attributes.erase(this->_attributes.find(name));
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

void	Elements::setFrameSprite(int frame) {
	int imgHeight = this->_height;
	int imgWidth = this->_width;
	int	cutWidth = 16, cutHeight = 16;
	float nbPerRow = imgWidth / cutWidth;
	float nbPerColumn = imgHeight / cutHeight;
	float fY = (1.0f / float(imgHeight)) * float(cutHeight);
	float fX = (1.0f / float(imgWidth)) * float(cutWidth);
	int	i;
	float tY = 1.0f - fY;
	float tX = fX;
	float tX2 = 0.0f;
	float tY2 = 1.0f;
	for (i = 0; i < frame; i++) {
		tX += fX;
		tX2 += fX;
		if (tX >= 1.0f) {
			tX = 0.0f;
			tX2 = fX;
			tY -= fY;
			tY2 -= fY;
		}
	}
	this->SetUVs(Vector2(tX, tY), Vector2(tX2, tY2));
}

/**
 * Display the Element on the screen
 */
void	Elements::display(void) {
	this->SetPosition(this->_XStartPos, this->_YStartPos);
	this->Tag(this->getAttribute("type"));
	if (this->getAttribute("sprite") != "")
		this->SetSprite(this->getAttribute("sprite"));
	else if (this->getAttribute("spritesFrame") != "") {
		this->LoadSpriteFrames(this->getAttribute("spritesFrame"));
	} else if (this->getAttribute("spriteMap") != "") {
		this->SetSprite(this->getAttribute("image"));
		this->setFrameSprite(this->_frame - 2);
	} else
		this->SetColor(0, 0, 0, 0);
	this->SetSize(1.0f);
	this->SetDrawShape(ADS_Square);
	if (this->getAttribute("type") == "wall" || this->getAttribute("type") == "ground") {
		this->SetDensity(0);
		this->SetFriction(1);
		this->SetRestitution(0);
	}
	if (this->_hitboxType == "special") {
		this->SetShapeType(PhysicsActor::SHAPETYPE_LOADED);
 		Game::hList->checkExists(this->_hitbox);
 		this->setBox(Game::hList->getHitbox(this->_hitbox));
	} else {
		this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	}

	if (this->getAttribute("physic") != "") {
		this->InitPhysics();
//		std::cout << "Init phy" << std::endl;
	}
	theWorld.Add(this);
}

/* SETTERS */

void	Elements::setXStart(float X) { this->_XStartPos = X; };
void	Elements::setYStart(float Y) { this->_YStartPos = Y; };
void	Elements::setWidth(int w) { this->_width = w; };
void	Elements::setHeight(int h) { this->_height = h; };
void	Elements::setCutWidth(int w) { this->_cutWidth = w; };
void	Elements::setCutHeight(int h) { this->_cutHeight = h; };
void	Elements::setFrame(int n) { this->_frame = n; };
void	Elements::setHitbox(std::string n) { this->_hitbox = n; this->_hitboxType = "special"; };

/* GETTERS */
std::map<std::string, std::string>	Elements::getAttributes(void) { return this->_attributes; };
