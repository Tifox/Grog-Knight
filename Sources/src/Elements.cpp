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
 * @date 2015-02-13 07:39
 * @author Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "../inc/Elements.hpp"

//! Main constructor
/**
 * @param id The future id of the element
 * @fixme This function is outdated due to recent changes in the API.
 */
Elements::Elements(int id) : PhysicsActor(), _isAdded(0) {
	this->setId(Game::getNextId());
	Game::addElement(*this);
	return ;
}

//! Base constructor
Elements::Elements(void) : PhysicsActor(), _isAdded(0) {
	this->setId(Game::getNextId());
	Game::addElement(*this);
	return ;
}

//! Copy constructor
/**
 * The constructor by Copy, thank's to the Coplien's form.
 * @param obj The object we have to copy.
 */
Elements::Elements(Elements & obj) : PhysicsActor(obj) {
	this->_attributes = obj.getAttributes();
	this->setId(obj.getId());
	this->_isAdded = obj.isAdded();
	Game::addElement(*this);
}

//! Basic destructor
Elements::~Elements(void) {
}

//! Add an attribute.
/**
 * Add an attribute to the Element
 * @param name Name of the attribute
 * @param value Value of the attribute
 * @fixme This method of attribute is outdated due to the new Json::Value politic in the API, more flexible.
 */
void	Elements::addAttribute(std::string name, std::string value) {
	this->_attributes[name] = value;
}

//! Remove an attribute
/**
 * Remove an attribute form the list
 * @param name The name of the attribute we have to remove
 */
void	Elements::removeAttr(std::string name) {
	if (this->_attributes.find(name) != this->_attributes.end())
		this->_attributes.erase(this->_attributes.find(name));
}

//! A getter for attribute
/**
 * Get an attribute value
 * @param name The Name of the attribute
 * @return The value of attribute if found, else ""
 */
std::string	Elements::getAttribute(std::string name) {
	if (this->_attributes.find(name) != this->_attributes.end())
		return this->_attributes[name];
	return "";
}

//! Set frame in a spriteFile
/**
 * Set a specific frame from a sprite file.
 * This function is currently used only by the the background elements.
 * @param frame The frame to display (Reading size left to right)
 * @fixme The height and width of the cutting size is write down in the code (16x16).
 */
void	Elements::setFrameSprite(int frame) {
	int imgHeight = this->_height;
	int imgWidth = this->_width;
	int	cutWidth = this->_cutWidth, cutHeight = this->_cutHeight;
	float nbPerRow = imgWidth / cutWidth;
	float nbPerColumn = imgHeight / cutHeight;
	float fY = (1.0f / float(imgHeight)) * float(cutHeight);
	float fX = (1.0f / float(imgWidth)) * float(cutWidth);
	int	i;
	float tY = 1.0f - fY;
	float tX = 0.0f;
	float tX2 = fX;
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

//! Display an element
/**
 * Load physics on element, and add him to the World.
 * @todo This function is quite nasty, maybe a rework on it should be better.
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
		this->setFrameSprite(this->_frame - 1);
	} else
		this->SetColor(0, 0, 0, 0);
	this->SetDrawShape(ADS_Square);
	if (this->getAttribute("type") == "wall" || this->getAttribute("type") == "ground") {
		this->SetDensity(0);
		this->SetFriction(1);
		this->SetRestitution(0);
	}
	else if (this->getAttribute("type") == "Hero")
	  this->SetFriction(0.15f);
	if (this->_hitboxType == "special") {
		this->SetShapeType(PhysicsActor::SHAPETYPE_LOADED);
 		this->setBox(Game::hList->getHitbox(this->_hitbox));
	} else if (this->getAttribute("hitbox") == "circle") {
		this->SetShapeType(PhysicsActor::SHAPETYPE_CIRCLE);
		this->SetDensity(0);
		this->SetIsSensor(true);
	} else {
		this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	}

	if (this->getAttribute("speType") == "water") {
		this->SetIsSensor(true);
		this->SetFixedRotation(true);
	}
	if (this->getAttribute("layer") != "") {
		this->SetLayer(std::stoi(this->getAttribute("layer")));
	}
	if (this->getAttribute("physic") != "") {
		this->InitPhysics();
	}
	if (this->getAttribute("transparency") != "")
		this->SetColor(0, 0, 0, 0);
	if (this->getAttribute("animate") != "") {
		this->_animIt = this->_animationList.begin();
		this->PlaySpriteAnimation((*this->_animIt)->time, SAT_OneShot, (*this->_animIt)->frame, (*this->_animIt)->frame, "baseAnimation");
	}
	this->_isAdded = 1;
	theWorld.Add(this);
}

/* SETTERS */

void	Elements::setXStart(float X) { this->_XStartPos = X; };
void	Elements::setYStart(float Y) { this->_YStartPos = Y; };
int		Elements::getXStart(void) { return this->_XStartPos; };
int		Elements::getYStart(void) { return this->_YStartPos; };
void	Elements::setWidth(int w) { this->_width = w; };
void	Elements::setHeight(int h) { this->_height = h; };
void	Elements::setCutWidth(int w) { this->_cutWidth = w; };
void	Elements::setCutHeight(int h) { this->_cutHeight = h; };
void	Elements::setFrame(int n) { this->_frame = n; };
void	Elements::setHitbox(std::string n) { this->_hitbox = n; this->_hitboxType = "special"; };
int		Elements::isAdded(void) { return this->_isAdded; };

/* GETTERS */
std::map<std::string, std::string>	Elements::getAttributes(void) { return this->_attributes; };
bool								Elements::isDead(void) { return this->_isDead; }

//! Animation callback
/**
 * The animation callback function
 * This function is only used here for block / background purposes,
 * And be overriden in Characters childs.
 * @param: s The Animation name.
 */
void	Elements::AnimCallback(String s) {
	if (s == "baseAnimation") {
		this->_animIt++;
		if (this->_animIt == this->_animationList.end())
			this->_animIt = this->_animationList.begin();
		this->PlaySpriteAnimation((*this->_animIt)->time, SAT_OneShot, (*this->_animIt)->frame, (*this->_animIt)->frame, "baseAnimation");
		this->setFrameSprite((*this->_animIt)->frame);
	} else if (s == "closeCloset") {
		theWorld.Remove(this);
		Game::currentGame->getHero()->removeAttr("closetReady");
	} else if (s == "closetReady") {
		Game::currentGame->getHero()->addAttribute("closetReady", "1");
	}
}

void	Elements::addAnimation(int frame, float time) {
	Animation	*anim = new Animation();
	anim->frame = frame;
	anim->time = time;
	this->_animationList.push_back(anim);
}

