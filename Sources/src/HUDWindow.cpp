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

//! Constructor
/**
 * Basic constructor
 */
HUDWindow::HUDWindow(void) : HUDActor() {
	RegisterFont("Resources/font.ttf", 14, "Gamefont");
	RegisterFont("Resources/font.ttf", 14, "Gamefont");
	RegisterFont("Resources/font.ttf", 10, "MediumGamefont");
	RegisterFont("Resources/Fonts/fail.otf", 80, "dead");
	RegisterFont("Resources/Fonts/Market_Deco.ttf", 80, "title");
	RegisterFont("Resources/Fonts/Market_Deco.ttf", 40, "smallTitle");
	this->_hearts.clear();
	return;
}

//! Destructor
/**
 * Basic destructor
 */
HUDWindow::~HUDWindow(void) {
	return;
}

//! Set text function
/**
 * Set a text in the window
 * @param str The text message
 * @param x X position
 * @param y Y position
 */
HUDWindow::Text		*HUDWindow::setText(std::string str, int x, int y) {
	HUDWindow::Text		*t = new HUDWindow::Text();
	t->str = str;
	t->x = x;
	t->y = y;
	t->colorR = t->colorG = t->colorB = t->colorA = 1;
	t->font = "Gamefont";
	t->toFollow = nullptr;
	this->_text.push_back(t);
	return t;
}

//! Set text function
/**
 * Set a text in the window
 * @param str The text message
 * @param x X position
 * @param y Y position
 * @param color The color of the text (See Angel2d's Vector3 for more info)
 * @param alpha The transparency.
 */
HUDWindow::Text		*HUDWindow::setText(std::string str, int x, int y, Vector3 color, int alpha) {
	HUDWindow::Text		*t = new HUDWindow::Text();
	t->str = str;
	t->x = x;
	t->y = y;
	t->colorR = color.X;
	t->colorG = color.Y;
	t->colorB = color.Z;
	t->colorA = alpha;
	t->font = "Gamefont";
	t->toFollow = nullptr;
	this->_text.push_back(t);
	return t;
}

//! Set text function
/**
 * Set a text in the window
 * @param str The text message
 * @param x X position
 * @param y Y position
 * @param color The color of the text (See Angel2d's Vector3 for more info)
 * @param alpha The transparency.
 * @param font The font name
 */
HUDWindow::Text		*HUDWindow::setText(std::string str, int x, int y, Vector3 color, int alpha, std::string font) {
	HUDWindow::Text		*t = new HUDWindow::Text();
	t->str = str;
	t->x = x;
	t->y = y;
	t->colorR = color.X;
	t->colorG = color.Y;
	t->colorB = color.Z;
	t->colorA = alpha;
	t->font = font;
	t->toFollow = nullptr;
	this->_text.push_back(t);
	return t;
}

//! Characters-follow Text
/**
 * Set a Text on a Character
 * @param str The Text
 * @param toFollow The Character
 * @param color The RGB Color
 * @param isFading 0 / 1 If the text has to fade
 * @param isTalk 0 / 1 If the text is a dialog
 * @note isFading and isTalk can't be true at the same time
 */
HUDWindow::Text		*HUDWindow::setText(std::string str, Characters *toFollow, 
		Vector3 color, int isFading, int isTalk) {
	HUDWindow::Text		*t = new HUDWindow::Text();

	t->str = str;
	t->toFollow = toFollow;
	t->colorR = color.X;
	t->colorG = color.Y;
	t->colorB = color.Z;
	t->colorA = 1;
	t->isFading = isFading;
	t->isTalk = isTalk;
	t->font = "Gamefont";
	t->y = 0;
	if (isTalk) {
		Elements	*test;

		test = new Elements();
		test->SetPosition(Game::currentGame->getHero()->GetBody()->GetWorldCenter().x, 
			(Game::currentGame->getHero()->GetBody()->GetWorldCenter().y + 1));
		test->SetSize(2.5, 0.7);
		test->SetSprite("Resources/Images/HUD/talk.png");
		test->SetDrawShape(ADS_Square);
		test->SetFixedRotation(true);
		test->SetLayer(1500);
		test->SetDensity(0.001f);
		test->SetRestitution(0);
		test->SetFriction(0);
		test->SetIsSensor(true);
		test->SetLayer(1500);
		test->InitPhysics();
		b2DistanceJointDef jointDef1;
		b2DistanceJointDef jointDef2;
		jointDef1.Initialize(Game::currentGame->getHero()->GetBody(), test->GetBody(), 
				b2Vec2(Game::currentGame->getHero()->GetBody()->GetWorldCenter().x + 0.4f, 
				Game::currentGame->getHero()->GetBody()->GetWorldCenter().y + 0.4f), test->GetBody()->GetWorldCenter());
		jointDef1.collideConnected = false;
		jointDef2.Initialize(Game::currentGame->getHero()->GetBody(), test->GetBody(), 
			b2Vec2(Game::currentGame->getHero()->GetBody()->GetWorldCenter().x - 0.4f, test->GetBody()->GetWorldCenter().y - 0.4f), test->GetBody()->GetWorldCenter());
		jointDef2.collideConnected = false;
		b2DistanceJoint *joint1 = (b2DistanceJoint*)theWorld.GetPhysicsWorld().CreateJoint(&jointDef1);
		b2DistanceJoint *joint2 = (b2DistanceJoint*)theWorld.GetPhysicsWorld().CreateJoint(&jointDef2);
		theWorld.Add(test);
		this->_dialog[t->str] = test;
	}
	this->_text.push_back(t);
	return t;
}

//! Remove Text
/**
 * This function remove the asked text from the screen.
 * @param str The text content message
 */
void	HUDWindow::removeText(std::string str) {
	std::list<HUDWindow::Text *>::iterator	i;

	for (i = this->_text.begin(); i != this->_text.end(); i++) {
		if ((*i)->str == str)
			break ;
	}
	if (i != this->_text.end()) {
		if (this->_dialog[str]) {
			Game::addToDestroyList(this->_dialog[str]);
			this->_dialog.erase(this->_dialog.find(str));
		}
		this->_text.erase(i);
	}
}

//! Remove a text with an object
/**
 * Remove a text from the screen with a HUDWindow::Text
 * @param text The object of the text
 */
void	HUDWindow::removeText(HUDWindow::Text *text) {
	std::list<HUDWindow::Text *>::iterator	i;

	for (i = this->_text.begin(); i != this->_text.end(); i++) {
		if ((*i) == text)
			break ;
	}
	if (i != this->_text.end()) {
		this->_text.erase(i);
	}
}

//! Callback for Display text
/**
 * Display a text in the HUD
 * This function is an intern callback between Game and HUDWindow. You problably don't want to call it.
 */
void	HUDWindow::displayText(void) {
	std::list<HUDWindow::Text *>::iterator	i;

	for (i = this->_text.begin(); i != this->_text.end(); i++) {
		glColor4f((*i)->colorR, (*i)->colorG, (*i)->colorB, (*i)->colorA);
		if ((*i)->toFollow == nullptr)
			DrawGameText((*i)->str, (*i)->font, (*i)->x, (*i)->y, theCamera.GetRotation());
		else {
			int		x, y, mult = 6;
			Map		m = Game::currentGame->maps->_XYMap[Game::currentY][Game::currentX];

			x = ((((*i)->toFollow->GetBody()->GetWorldCenter().x + 0.5) - m.getXStart()) * 40) - 40;
			y = -((((*i)->toFollow->GetBody()->GetWorldCenter().y - 0.5) - m.getYStart()) * 40) + 50;
			if ((*i)->isFading) {
				DrawGameText((*i)->str, (*i)->font, x, y - (*i)->y, theCamera.GetRotation());
				(*i)->y += 1;
				(*i)->colorA -= 0.05f;
			} else if ((*i)->isTalk) {
				DrawGameText((*i)->str, (*i)->font, x - 5, y - (*i)->y + 5, theCamera.GetRotation());
			} else {
				DrawGameText((*i)->str, (*i)->font, x, y - (*i)->y, theCamera.GetRotation());
			}
		}
	}
}

//! Add an Image
/**
 * Add an image in the HUD
 * @param path The path of the img
 * @param x X position
 * @param y Y position
 */
HUDActor	*HUDWindow::addImage(std::string path, int x, int y) {
	HUDActor *tmp = new HUDActor();
	tmp->SetSprite(path);
	tmp->SetPosition(x, y);
	tmp->SetSize(50.0f);
	tmp->SetDrawShape(ADS_Square);
	tmp->SetLayer(100);
	theWorld.Add(tmp);
	return tmp;
}

//! Add an image
/**
 * Add an image in the HUD
 * @param path The path of the img
 * @param x X position
 * @param y Y position
 * @param size Size, in float.
 */
HUDActor	*HUDWindow::addImage(std::string path, int x, int y, float size) {
	HUDActor *tmp = new HUDActor();
	tmp->SetSprite(path);
	tmp->SetPosition(x, y);
	tmp->SetSize(size);
	tmp->SetDrawShape(ADS_Square);
	tmp->SetLayer(100);
	theWorld.Add(tmp);
	return tmp;
}

//! Add an image
/**
 * Add an image in the HUD
 * @param path The path of the img
 * @param x X position
 * @param y Y position
 * @param size Size, in float.
 * @param layer The desired layer
 */
HUDActor	*HUDWindow::addImage(std::string path, int x, int y, float size, int layer) {
	HUDActor *tmp = new HUDActor();
	tmp->SetSprite(path);
	tmp->SetPosition(x, y);
	tmp->SetSize(size);
	tmp->SetDrawShape(ADS_Square);
	tmp->SetLayer(layer);
	theWorld.Add(tmp);
	return tmp;
}

//! Add an image
/**
 * Add an image in the HUD
 * @param path The path of the img
 * @param x X position
 * @param y Y position
 * @param size Size, X Y in float (Vector2).
 * @param layer The desired layer
 */
HUDActor	*HUDWindow::addImage(std::string path, int x, int y, Vector2 size, int layer) {
	HUDActor *tmp = new HUDActor();
	std::cout << x << ":" << y << std::endl;
	tmp->SetSprite(path);
	tmp->SetPosition(x, y);
	tmp->SetSize(size.X, size.Y);
	tmp->SetDrawShape(ADS_Square);
	tmp->SetLayer(layer);
	theWorld.Add(tmp);
	return tmp;
}




//! Display HP function
/**
 * Life is life (lalala)
 * This function take care of the heart, in the HUD.
 * @param int The life number
 * @todo Empty heart, half-heart.
 */
void	HUDWindow::life(int life) {
	int		x, v, sLife = life, y, size;
	std::list<HUDActor *>::iterator	i;
	int		index;
	HUDActor *tmp;

	y = theCamera.GetWindowHeight() / 20 * 0.9;
	size = theCamera.GetWindowWidth() / 20 * 0.6;
	for (i = this->_hearts.begin(), index = 0; i != this->_hearts.end(); i++, index++)
		theWorld.Remove(*(i));
	this->_hearts.clear();
	for (x = theCamera.GetWindowWidth() / 20 * 3; life > 0; x += theCamera.GetWindowWidth() / 35) {
		if (x == theCamera.GetWindowWidth() / 20 * 3) {
			this->_hearts.push_back(this->addImage("Resources/Images/HUD/hp.png", (x - theCamera.GetWindowWidth() / 30), y, size - 2));
		} if (life >= 25) {
			this->_hearts.push_back(this->addImage("Resources/Images/HUD/heart.png", x, y, size, 100));
			life -= 25;
		}
	}
	if (sLife < this->_maxHP) {
		for (v = 0; (this->_maxHP - sLife) > v; v += 25)
			this->_hearts.push_back(this->addImage("Resources/Images/HUD/empty_heart.png", x, y, size, 100));
	}
}

//! Display mana function
/**
 * Show the mana in the HUD
 * Same way as HUDWindow::life, this function handle the sprites, and the Empty mana
 * @param mana The Mana point.
 */
void	HUDWindow::mana(int mana) {
	std::list<HUDActor *>::iterator	i;
	float								x = theCamera.GetWindowWidth() / 20 * 3.05;
	int									y, max = this->_maxMana;
	float								 yHeight = theCamera.GetWindowHeight() / 20 * 1.56, size = theCamera.GetWindowWidth() / 20 * 0.25;

	for (i = this->_mana.begin(); i != this->_mana.end(); i++)
		theWorld.Remove(*(i));
	this->_mana.clear();
	this->_mana.push_back(this->addImage("Resources/Images/HUD/mp.png", (x - theCamera.GetWindowWidth() / 30), yHeight, size * 2));
	x -= theCamera.GetWindowWidth() / 80 * 0.8;
	this->_mana.push_back(this->addImage("Resources/Images/HUD/mp_bar_first.png", x, yHeight, size));
	x += theCamera.GetWindowWidth() / 80;
	for (y = 0; y < (mana - 1); y += 10, x += theCamera.GetWindowWidth() / 80) {
		this->_mana.push_back(this->addImage("Resources/Images/HUD/mp_bar_mid.png", x, yHeight, size));
	}
	if (y == max)
		this->_mana.push_back(this->addImage("Resources/Images/HUD/mp_bar_full_end.png", x, yHeight, size));
	else {
		for (; y < max; y += 10, x += theCamera.GetWindowWidth() / 80)
			this->_mana.push_back(this->addImage("Resources/Images/HUD/mp_bar_empty.png", x, yHeight, size));
		this->_mana.push_back(this->addImage("Resources/Images/HUD/mp_bar_empty_end.png", x, yHeight, size));
	}
}

//! Display gold function
/**
 * Make it rain baby
 * Like this all API, evrything his handled. Don't worry.
 * @param gold The gold number
 */
void	HUDWindow::gold(int gold) {
	float								x = theCamera.GetWindowWidth() / 20 * 2.37;
	int									y = theCamera.GetWindowHeight() / 20 * 2.2;
	float								size = theCamera.GetWindowWidth() / 20 * 0.41;

	this->addImage("Resources/Images/HUD/xp.png", x, y, Vector2(size + 3.5, size), 100);
	this->addImage("Resources/Images/HUD/gold.png", x + (theCamera.GetWindowWidth() / 40 * 1.2), y, size);
	this->_gold = this->setText(std::to_string(gold), x + theCamera.GetWindowWidth() / 40 * 2, y + theCamera.GetWindowHeight() / 20 * 0.15, Vector3(246.0f, 255.0f, 0.0f), 1);
}

//! MAJ gold function
/**
 * New gold update function
 * Remove the old gold, and display the new one
 * @param gold The new gold number
 */
void	HUDWindow::updateGold(int gold) {
	float								x = theCamera.GetWindowWidth() / 20 * 2.37;
	int									y = theCamera.GetWindowHeight() / 20 * 2.2;

	this->removeText(this->_gold);
	this->_gold = this->setText(std::to_string(gold), x + theCamera.GetWindowWidth() / 40 * 2, y + theCamera.GetWindowHeight() / 20 * 0.15, Vector3(246.0f, 255.0f, 0.0f), 1);
}

//! Display items (Weapon)
/**
 * Display the weapon function.
 * This function is a callback call in Character::equipWeapon.
 * So you won't problably call this function !
 * @param w The Weapon equipped.
 */
void	HUDWindow::items(Weapon *w) {
	this->addImage("Resources/HUD/weapon_stuff.png", (theCamera.GetWindowWidth() / 20 * 11), theCamera.GetWindowHeight() / 20, theCamera.GetWindowWidth() / 20);
	this->addImage(w->getSprite(), (theCamera.GetWindowWidth() / 20 * 11), theCamera.GetWindowHeight() / 20, theCamera.GetWindowWidth() / 20 * 0.7);
}

//! Display items (Armor)
/**
 * Display the armor function.
 * This function is a callback call in Character::equipArmor.
 * So you won't problably call this function !
 * @param a The Armor equipped.
 */
void	HUDWindow::items(Armor *a) {
	this->addImage("Resources/HUD/normal_stuff.png", (theCamera.GetWindowWidth() / 20 * 9.8), theCamera.GetWindowHeight() / 20, theCamera.GetWindowWidth() / 20);
	this->addImage(a->getSprite(), (theCamera.GetWindowWidth() / 20 * 9.8), theCamera.GetWindowHeight() / 20, theCamera.GetWindowWidth() / 20 * 0.7);
}

//! Display items (Ring)
/**
 * Display the ring function.
 * This function is a callback call in Character::equipRing.
 * So you won't problably call this function !
 * @param r The Ring equipped.
 */
void	HUDWindow::items(Ring *r) {
	this->addImage("Resources/HUD/normal_stuff.png", (theCamera.GetWindowWidth() / 20 * 8.5), theCamera.GetWindowHeight() / 20, theCamera.GetWindowWidth() / 20);
	this->addImage(r->getSprite(), (theCamera.GetWindowWidth() / 20 * 8.5), theCamera.GetWindowHeight() / 20, theCamera.GetWindowWidth() / 20 * 0.7);
}

//! Display armor 
/**
 * Display the armor function.
 * This function is a callback call in Character::setArmor.
 * So you won't problably call this function !
 * @todo The description here is a lie. But it's gonna append !
 */
void	HUDWindow::armor(void) {
	this->addImage("Resources/Images/HUD/item_background.png", 710, 50, 40.0f);
	this->addImage("Resources/Images/HUD/armor.png", 710, 50, 30.0f);
}

//! Display boots function
/**
 * Show the actual boots in the HUDWindow
 * @todo No callback & object for now, but have to do it !
 */
void	HUDWindow::boots(void) {
	this->addImage("Resources/Images/HUD/item_background.png", 660, 50, 40.0f);
	this->addImage("Resources/Images/HUD/boots.png", 660, 50, 30.0f);
}

//! Show the consumable function
/**
 * Display the consumable (potions, etc) in the HUD
 * @todo Same here, no callback or object.
 */
void	HUDWindow::consumable(std::map<int, std::string> items) {
	int		i, x, y, size;
	std::list<HUDActor *>::iterator		it;

	for (it = this->_bag.begin(); it != this->_bag.end(); it++)
		theWorld.Remove((*it));

	x = theCamera.GetWindowWidth() / 20 * 8.3;
	y = theCamera.GetWindowHeight() / 20 * 2.3;
	size = theCamera.GetWindowWidth() / 20 * 0.5;
	for (i = 0; i < 4; i++, x += theCamera.GetWindowWidth() / 20) {
		if (items[i] != "") {
			HUDActor	*tmp;
			if (Game::wList->checkExists(items[i])) {
				Weapon *w = new Weapon(Game::wList->getWeapon(items[i]));
				tmp = this->addImage(w->getSprite(), x, y, size);
			} else if (Game::aList->checkExists(items[i])) {
				Armor* w = new Armor(Game::aList->getArmor(items[i]));
				tmp = this->addImage(w->getSprite(), x, y, size);
			} else if (Game::rList->checkExists(items[i])) {
				Ring* w = new Ring(Game::rList->getRing(items[i]));
				tmp = this->addImage(w->getSprite(), x, y, size);
			}
			this->_bag.push_back(tmp);
		}
	}
}

//! Init the background of the minimap
void	HUDWindow::initMinimapBackground(void) {
	HUDActor *tmp;

	tmp = new HUDActor();
	tmp->SetSize(201, 100);
	tmp->SetPosition(theCamera.GetWindowWidth() - 101, 50);
	tmp->SetSprite("Resources/Images/HUD/minimap_background.png");
	tmp->SetDrawShape(ADS_Square);
	theWorld.Add(tmp);
}

void	HUDWindow::_drawDoor(Vector2 size, Vector2 position) {
	HUDActor	*t;

	t = new HUDActor();
	t->SetColor(1, 1, 1);
	t->SetDrawShape(ADS_Square);
	t->SetPosition(position.X, position.Y);
	t->SetSize(size.X, size.Y);
	t->SetLayer(100);
	theWorld.Add(t);
	this->_minimap.push_back(t);
}

//! Display the minimap in the HUD
/**
 * This function is not finished at all.
 * So don't use it.
 * Thank's.
 * @todo Make this function work normally.
 */
void	HUDWindow::minimap(void) {
	int		x, y, x2, y2;
	HUDActor *tmp;
	std::list<HUDActor *>::iterator		it;

	for (it = this->_minimap.begin(); it != this->_minimap.end(); it++)
		theWorld.Remove(*it);

	x = theCamera.GetWindowWidth() - 180;
	y = 18;
	for (x2 = Game::currentX - 2, y2 = Game::currentY - 1; ; x2++, x += 41) {
		if (x2 == Game::currentX + 4) {
			y2++;
			x2 = Game::currentX - 2;
			y += 28;
			x = theCamera.GetWindowWidth() - 180;
			if (y2 == Game::currentY + 2)
				break;
		}
		if (Game::currentGame->maps->getMapXY()[y2][x2].getIsUsed()) {
			tmp = new HUDActor();
			tmp->SetSize(40, 27);
			tmp->SetPosition(x, y);
			if (x2 == Game::currentX && y2 == Game::currentY)
				tmp->SetColor(0, 1, 0);
			else
				tmp->SetColor(1, 1, 1);
			tmp->SetDrawShape(ADS_Square);
			tmp->SetLayer(100);
			theWorld.Add(tmp);
			this->_minimap.push_back(tmp);

			if (Game::currentGame->maps->getMapXY()[y2][x2 + 1].getXStart())
				this->_drawDoor(Vector2(1, 5), Vector2(x + 1 + (40 / 2), y));
			if (Game::currentGame->maps->getMapXY()[y2][x2 - 1].getXStart())
				this->_drawDoor(Vector2(1, 5), Vector2(x - (40 / 2), y));
			if (Game::currentGame->maps->getMapXY()[y2 - 1][x2].getXStart())
				this->_drawDoor(Vector2(5, 1), Vector2(x, y - (27 / 2) - 1));
			if (Game::currentGame->maps->getMapXY()[y2 + 1][x2].getXStart())
				this->_drawDoor(Vector2(5, 1), Vector2(x, y + (27 / 2) + 1));
		}
	}
}

//! Bag display function
/**
 * This function display an empty bag at the beggining of the game
 */
void	HUDWindow::bag(void) {
	int		i, x, available;

	available = this->_g->getHero()->getMaxInventory();
	for (i = 0, x = theCamera.GetWindowWidth() / 20 * 8.3; i < 4; i++, x += theCamera.GetWindowWidth() / 20) {
		if (i >= available)
			this->addImage("Resources/HUD/bag_unavailable.png", x, theCamera.GetWindowHeight() / 20 * 2.3, theCamera.GetWindowWidth() / 20 * 0.65);
		else
			this->addImage("Resources/HUD/bag_empty.png", x, theCamera.GetWindowHeight() / 20 * 2.3, theCamera.GetWindowWidth() / 20 * 0.65);
	}
}

void	HUDWindow::showHud(void) {
	this->showBackgrounds();
	this->character();
	this->spells();
	this->bag();
	this->minimap();
}

void	HUDWindow::showBackgrounds(void) {
	int		x, y, height, length;

	x = theCamera.GetWindowWidth() / 5 * 3;
	y = (theCamera.GetWindowHeight() / 10);
	this->addImage("Resources/HUD/bg1.png", x - ((theCamera.GetWindowWidth() / 5) * 1.5), y - (y / 4),
			Vector2(x, (y * 1.5) ), 2);
	this->addImage("Resources/HUD/bg2.png", x + (theCamera.GetWindowWidth() / 10), y - (y / 4), 
			Vector2(theCamera.GetWindowWidth() / 5 * 1.5, y * 1.3), 1);
	this->addImage("Resources/HUD/bg3.png", (theCamera.GetWindowWidth() - theCamera.GetWindowWidth() / 10), y - (y / 4), 
			Vector2(theCamera.GetWindowWidth() / 5, y * 1.5), 2);
}

void	HUDWindow::character(void) {
	this->addImage("Resources/HUD/perso_bg.png", (theCamera.GetWindowWidth() / 20) * 1.1, theCamera.GetWindowHeight() / 20 * 1.5,
			theCamera.GetWindowWidth() / 13, 3);
	this->addImage("Resources/HUD/perso_pin.png", theCamera.GetWindowWidth() / 20 * 0.4, theCamera.GetWindowHeight() / 20 * 0.8, 
			theCamera.GetWindowWidth() / 32, 5);
	this->addImage(this->_g->getHero()->getAttribute("spritesFrame"), theCamera.GetWindowWidth() / 20 * 1.1, theCamera.GetWindowHeight() / 20 * 1.5,
			theCamera.GetWindowWidth() / 18, 4);
	HUDActor	*bag = new HUDActor();
	this->setText("Lvl " + std::to_string(this->_g->getHero()->getLevel()), theCamera.GetWindowWidth() / 20 * 0.2, theCamera.GetWindowHeight() / 20 * 0.9, Vector3(1, 1, 1), 1, "MediumGamefont");
}

void	HUDWindow::spells(void) {
	this->addImage("Resources/HUD/spells_bg.png", (theCamera.GetWindowWidth() / 20) * 7, theCamera.GetWindowHeight() / 20 * 1.5, 
			Vector2(theCamera.GetWindowWidth() / 15, theCamera.GetWindowHeight() / 10 * 1.2), 5);
}

void	HUDWindow::setGame(Game *g) { this->_g = g; };
void	HUDWindow::setMaxMana(int m) { this->_maxMana = m; };
void	HUDWindow::setMaxHP(int h) { this->_maxHP = h; };
