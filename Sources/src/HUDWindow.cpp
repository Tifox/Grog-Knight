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
 * File: HUDWindow.cpp
 * Creation: 2015-03-14 05:35
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "HUDWindow.hpp"

/**
 * Basic constructor
 */
HUDWindow::HUDWindow(void) : HUDActor() {
	return;
}

/**
 * Basic destructor
 */
HUDWindow::~HUDWindow(void) {
	return;
}

/**
 * Set a text in the window
 * @param: str (std::string)
 * @param: x (int)
 * @param: y (int)
 */
void	HUDWindow::setText(std::string str, int x, int y) {
	HUDWindow::Text		*t = new HUDWindow::Text();
	t->str = str;
	t->x = x;
	t->y = y;
	t->colorR = t->colorG = t->colorB = t->colorA = 1;
	this->_text.push_back(t);
}

/**
 * Set a text in the window
 * @param: str (std::string)
 * @param: x (int)
 * @param: y (int)
 * @param: color (Vector3)
 * @param: alpha (int)
 * @note: This function is an overload of ^
 */
void	HUDWindow::setText(std::string str, int x, int y, Vector3 color, int alpha) {
	HUDWindow::Text		*t = new HUDWindow::Text();
	t->str = str;
	t->x = x;
	t->y = y;
	t->colorR = color.X;
	t->colorG = color.Y;
	t->colorB = color.Z;
	t->colorA = alpha;
	this->_text.push_back(t);
}

void	HUDWindow::removeText(std::string str) {
	std::list<HUDWindow::Text *>::iterator	i;

	for (i = this->_text.begin(); i != this->_text.end(); i++) {
		if ((*i)->str == str)
			break ;
	}
	if (i != this->_text.end())
		this->_text.erase(i);
}

/**
 * Display a text in the HUD
 */
void	HUDWindow::displayText(void) {
	std::list<HUDWindow::Text *>::iterator	i;

	for (i = this->_text.begin(); i != this->_text.end(); i++) {
		glColor4f((*i)->colorR, (*i)->colorG, (*i)->colorB, (*i)->colorA);
		DrawGameText((*i)->str, "Gamefont", (*i)->x, (*i)->y, theCamera.GetRotation());
	}
}

/**
 * Add an image in the HUD
 * @param: path (std::string)
 * @param: x (int)
 * @param: y (int)
 */
void	HUDWindow::addImage(std::string path, int x, int y) {
	HUDActor *tmp = new HUDActor();
	tmp->SetSprite(path);
	tmp->SetPosition(x, y);
	tmp->SetSize(50.0f);
	tmp->SetDrawShape(ADS_Square);
	tmp->SetLayer(100);
	theWorld.Add(tmp);
}

/**
 * Add an image in the HUD
 * @param: path (std::string)
 * @param: x (int)
 * @param: y (int)
 * @param: size (float)
 * @note: This function is an overload of ^
 */
void	HUDWindow::addImage(std::string path, int x, int y, float size) {
	HUDActor *tmp = new HUDActor();
	tmp->SetSprite(path);
	tmp->SetPosition(x, y);
	tmp->SetSize(size);
	tmp->SetDrawShape(ADS_Square);
	tmp->SetLayer(100);
	theWorld.Add(tmp);
}

/**
 * Life is life (lalala)
 */
void	HUDWindow::life(int life) {
	int		x;
	std::list<HUDActor *>::iterator	i;
	int		index;
	HUDActor *tmp;

	for (i = this->_hearts.begin(), index = 0; i != this->_hearts.end(); i++, index++)
		theWorld.Remove(*(i));
	this->_hearts.clear();
	for (x = 200; life > 0; x += 25) {
		if (x == 200) {
			this->addImage("Resources/Images/HUD/hp.png", (x - 30), 35, 28.0f);
		} if (life >= 25) {
			tmp = new HUDActor();
			tmp->SetSprite("Resources/Images/HUD/heart.png");
			tmp->SetPosition(x, 35);
			tmp->SetSize(30.0f);
			theWorld.Add(tmp);
			this->_hearts.push_back(tmp);
			life -= 25;
		}
	}
}

/**
 * Show the mana in the HUD
 */
void	HUDWindow::mana(int mana) {
	std::list<HUDActor *>::iterator	i;
	int								x = 200, y, max = 90;

	for (i = this->_mana.begin(); i != this->_mana.end(); i++)
		theWorld.Remove(*(i));
	this->_mana.clear();
	this->addImage("Resources/Images/HUD/mp.png", (x - 30), 60, 26.0f);
	this->addImage("Resources/Images/HUD/mp_bar_first.png", x, 60, 12.0f);
	x += 11;
	for (y = 0; y < (mana - 1); y += 10, x += 11) {
		this->addImage("Resources/Images/HUD/mp_bar_mid.png", x, 60, 12.0f);
	}
	if (y == max)
		this->addImage("Resources/Images/HUD/mp_bar_full_end.png", x, 60, 12.0f);
	else
		this->addImage("Resources/Images/HUD/mp_bar_end.png", x, 60, 12.0f);
}

/**
 * Make it rain baby
 * @param: gold (int)
 */
void	HUDWindow::gold(int gold) {
	this->addImage("Resources/Images/HUD/xp.png", 340, 50, 20.0f);
	this->addImage("Resources/Images/HUD/gold.png", 360, 50, 20.0f);
	RegisterFont("Resources/font.ttf", 14, "Gamefont");
	this->setText(std::to_string(gold), 380, 55, Vector3(246.0f, 255.0f, 0.0f), 1);
}

void	HUDWindow::items(Weapon *w) {
	this->addImage("Resources/Images/HUD/weapon_background.png", 770, 50, 60.0f);
	this->addImage(w->getSprite(), 770, 50, 40.0f);
}

void	HUDWindow::armor(void) {
	this->addImage("Resources/Images/HUD/item_background.png", 710, 50, 40.0f);
	this->addImage("Resources/Images/HUD/armor.png", 710, 50, 30.0f);
}

void	HUDWindow::boots(void) {
	this->addImage("Resources/Images/HUD/item_background.png", 660, 50, 40.0f);
	this->addImage("Resources/Images/HUD/boots.png", 660, 50, 30.0f);
}

void	HUDWindow::consumable(void) {
	this->addImage("Resources/Images/HUD/consumable_background.png", 500, 50, 40.0f);
	this->addImage("Resources/Images/HUD/potion_life.png", 500, 50, 25.0f);
	this->addImage("Resources/Images/HUD/consumable_background.png", 550, 50, 40.0f);
	this->addImage("Resources/Images/HUD/potion_life.png", 550, 50, 25.0f);
	this->addImage("Resources/Images/HUD/consumable_background.png", 600, 50, 40.0f);
	this->addImage("Resources/Images/HUD/potion_mana.png", 600, 50, 25.0f);
}

void	HUDWindow::minimap(void) {
	HUDActor *minimap = new HUDActor();

	minimap->SetSize(200, 100);
	minimap->SetPosition(theCamera.GetWindowWidth() - 100, 50);
	minimap->SetColor(1, 0, 0);
	minimap->SetDrawShape(ADS_Square);
	theWorld.Add(minimap);
}

void	HUDWindow::setGame(Game *g) { this->_g = g; };
