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
HUDWindow::HUDWindow(void) : HUDActor(), _cooldownAtt(0), _cooldownMove(0) {
	std::string			file;
	std::stringstream 	buffer;
	std::ifstream		fd;
	Json::Reader		read;

	RegisterFont("Resources/font.ttf", 14, "Gamefont");
	RegisterFont("Resources/font.ttf", 20, "BigGamefont");
	RegisterFont("Resources/font.ttf", 10, "MediumGamefont");
	RegisterFont("Resources/Fonts/fail.otf", 80, "dead");
	RegisterFont("Resources/Fonts/Market_Deco.ttf", 80, "title");
	RegisterFont("Resources/Fonts/Market_Deco.ttf", 40, "smallTitle");
	theSwitchboard.SubscribeTo(this, "speAttCooldownEnd");
	theSwitchboard.SubscribeTo(this, "speMoveCooldownEnd");
	this->_hearts.clear();
	fd.open("Resources/Dialogs/dialogs.json");
	buffer << fd.rdbuf();
	read.parse(buffer.str(), this->_dialogs);
	this->_text.clear();
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
	t->isFading = 0;
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
HUDWindow::Text		*HUDWindow::setText(std::string str, int x, int y, Vector3 color, float alpha) {
	HUDWindow::Text		*t = new HUDWindow::Text();
	t->str = str;
	t->x = x;
	t->y = y;
	t->colorR = color.X;
	t->colorG = color.Y;
	t->colorB = color.Z;
	t->colorA = alpha;
	t->font = "Gamefont";
	t->isFading = 0;
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
	t->isFading = 0;
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
		Vector3 color, int isFading, int isTalk, int isInMenu) {
	HUDWindow::Text		*t = new HUDWindow::Text();

	t->str = str;
	if (t->toFollow)
		t->toFollow = toFollow;
	else
		t->toFollow = nullptr;
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
		if (Game::isInMenu)
			test->SetSize(str.length() / 2.7, toFollow->GetSize().X / 2);
		else
			test->SetSize(str.length() / 4, toFollow->GetSize().X / 2);
		test->SetPosition(toFollow->GetBody()->GetWorldCenter().x, 
				(toFollow->GetBody()->GetWorldCenter().y + toFollow->GetSize().Y));
		test->SetSprite("Resources/Images/HUD/talk.png");
		test->SetDrawShape(ADS_Square);
		test->SetFixedRotation(true);
		test->SetLayer(1500);
		test->SetDensity(0.001f);
		test->SetRestitution(0);
		test->SetFriction(0);
		test->SetIsSensor(true);
		test->addAttribute("isPhysic", "1");
		t->joint1 = t->joint2 = nullptr;
		Game::bodiesToCreate.push_back(test);
		t->toFollow = toFollow;
		this->_dialog[t->str] = test;
		theSwitchboard.DeferredBroadcast(new Message("remove" + t->str), 4);
	}
	this->_text.push_back(t);
	return t;
}


//! Remove Text
/**
 * This function remove the asked text from the screen.
 * @param str The text content message
 */
void	HUDWindow::removeText(std::string str, int last) {
	std::list<HUDWindow::Text *>::iterator	i, t = this->_text.end();

	for (i = this->_text.begin(); i != this->_text.end(); i++) {
		if ((*i)->str == str) {
			if (last) {
				this->_text.erase(i);
			} else
				break ;
		}
	}
	if (i != this->_text.end()) {
		if (this->_dialog[str]) {
			if (theWorld.GetPhysicsWorld().IsLocked() == false) {
				theWorld.GetPhysicsWorld().DestroyJoint((*i)->joint1);
				theWorld.GetPhysicsWorld().DestroyJoint((*i)->joint2);
				this->_dialog[str]->GetBody()->SetActive(false);
				theWorld.GetPhysicsWorld().DestroyBody(this->_dialog[str]->GetBody());
				theWorld.Remove(this->_dialog[str]);
				this->_dialog[str]->ChangeColorTo(Color(0, 0, 0, 0), 0);
			} else {
				this->_dialog[str]->ChangeColorTo(Color(0, 0, 0, 0), 0);
				Game::jointList.push_back((*i)->joint1);
				Game::jointList.push_back((*i)->joint2);
				Game::addToDestroyList(this->_dialog[str]);
			}
			this->_dialog.erase(this->_dialog.find(str));
		}
		this->_text.erase(i);
	}
}

void	HUDWindow::updateText(std::string org, std::string newStr) {
	std::list<HUDWindow::Text *>::iterator	i;

	for (i = this->_text.begin(); i != this->_text.end(); i++) {
		if ((*i)->str == org)
			break ;
	}
	if (i != this->_text.end()) {
		(*i)->str = newStr;
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
		if ((*i)->isFading)
			(*i)->colorA -= 0.02f;
		if ((*i)->colorA < 0) {
			this->removeText((*i)->str); return ;
		}
		if ((*i)->toFollow == nullptr)
			DrawGameText((*i)->str, (*i)->font, (*i)->x, (*i)->y, theCamera.GetRotation());
		else {
			int		x, y, mult = 6;
			Map		m = Game::currentGame->getCurrentMap();

			if ((*i)->toFollow->GetBody()) {
				if ((*i)->colorA < 0) {
					this->removeText((*i)->str); return ;
				}
				Vector2 v, pos = (*i)->toFollow->GetPosition();
				pos.Y += (*i)->toFollow->GetSize().Y;
				v = MathUtil::WorldToScreen(pos);
				x = v.X - (*i)->str.length() * 2.5;
				y = v.Y;
				if ((*i)->isFading) {
					DrawGameText((*i)->str, (*i)->font, x, y - (*i)->y, theCamera.GetRotation());
					(*i)->y += 1;
				} else if ((*i)->isTalk) {
					if ((*i)->joint1 == nullptr && (*i)->joint2 == nullptr) {
						theSwitchboard.SubscribeTo(this, "remove" + (*i)->str);
						Elements *test = this->_dialog[(*i)->str];
						b2DistanceJointDef jointDef1;
						b2DistanceJointDef jointDef2;
						jointDef1.Initialize((*i)->toFollow->GetBody(), test->GetBody(), 
						b2Vec2((*i)->toFollow->GetBody()->GetWorldCenter().x + 0.4f, 
							(*i)->toFollow->GetBody()->GetWorldCenter().y + 0.4f), test->GetBody()->GetWorldCenter());
						jointDef1.collideConnected = false;
						jointDef2.Initialize((*i)->toFollow->GetBody(), test->GetBody(), 
							b2Vec2((*i)->toFollow->GetBody()->GetWorldCenter().x - 0.4f, test->GetBody()->GetWorldCenter().y - 0.4f), test->GetBody()->GetWorldCenter());
						jointDef2.collideConnected = false;
						b2DistanceJoint *joint1 = (b2DistanceJoint*)theWorld.GetPhysicsWorld().CreateJoint(&jointDef1);
						b2DistanceJoint *joint2 = (b2DistanceJoint*)theWorld.GetPhysicsWorld().CreateJoint(&jointDef2);
						(*i)->joint1 = joint1;
						(*i)->joint2 = joint2;
					}
					DrawGameText((*i)->str, (*i)->font, x - 5, y - (*i)->y + 5, theCamera.GetRotation());
				} else {
					DrawGameText((*i)->str, (*i)->font, x, y - (*i)->y, theCamera.GetRotation());
				}
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
	this->_allElems.push_back(tmp);
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
	this->_allElems.push_back(tmp);
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
	this->_allElems.push_back(tmp);
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
	tmp->SetSprite(path);
	tmp->SetPosition(x, y);
	tmp->SetSize(size.X, size.Y);
	tmp->SetDrawShape(ADS_Square);
	tmp->SetLayer(layer);
	theWorld.Add(tmp);
	this->_allElems.push_back(tmp);
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
	int		x, v, y, size, j;
	float percent;
	std::list<HUDActor *>::iterator	i;
	int		index;

	if (!Game::deadWaiting) {
		y = theCamera.GetWindowHeight() / 20 * 1;
		size = theCamera.GetWindowWidth() / 20 * 0.6;
		for (i = this->_hearts.begin(), index = 0; i != this->_hearts.end(); i++, index++) {
			(*(i))->ChangeColorTo(Color(0, 0, 0, 0), 0);
			theWorld.Remove(*(i));
		}
		this->_hearts.clear();
		percent = life * 100 / this->_maxHP;
		x = theCamera.GetWindowWidth() / 20 * 3;
		this->_hearts.push_back(this->addImage("Resources/Images/HUD/hp.png", (x - theCamera.GetWindowWidth() / 30), y, size - 2));
		x += size - 2;
		if (percent > 0)
			this->_hearts.push_back(this->addImage("Resources/Images/HUD/hp_begin.png", 
						((x - theCamera.GetWindowWidth() / 30)), y, Vector2(5, 15), 100));
		x += 1.3;
		for (j = 1; j < percent; x += 1.3, j++)
			this->_hearts.push_back(this->addImage("Resources/Images/HUD/hp_middle.png", (x - theCamera.GetWindowWidth() / 30), y, Vector2(1.3, 15), 100));
		this->_hearts.push_back(this->addImage("Resources/Images/HUD/hp_end.png", 
					((x - theCamera.GetWindowWidth() / 30) + 1.3), y, Vector2(5, 15), 100));
		if (percent < 100) {
			for (; percent < 100; percent++, x += 1.3) {
				this->_hearts.push_back(this->addImage("Resources/Images/HUD/hp_empty_middle.png", (x - theCamera.GetWindowWidth() / 30), y, Vector2(1.3, 15), 100));
			}
			this->_hearts.push_back(this->addImage("Resources/Images/HUD/hp_empty_end.png", 
						((x - theCamera.GetWindowWidth() / 30) + 1.3), y, Vector2(5, 15), 100));
		}
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
	int									y = theCamera.GetWindowHeight() / 20 * 1.9;
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
	Elements		*w;

	for (it = this->_bag.begin(); it != this->_bag.end(); it++)
		theWorld.Remove((*it));

	x = theCamera.GetWindowWidth() / 20 * 8.3;
	y = theCamera.GetWindowHeight() / 20 * 2.3;
	size = theCamera.GetWindowWidth() / 20 * 0.5;
	for (i = 0; i < 4; i++, x += theCamera.GetWindowWidth() / 20) {
		if (items[i] != "") {
			HUDActor	*tmp;
			if (Game::wList->checkExists(items[i])) {
				w = new Weapon(Game::wList->getWeapon(items[i]));
				tmp = this->addImage(w->getAttribute("sprite"), x, y, size);
			} else if (Game::aList->checkExists(items[i])) {
				w = new Armor(Game::aList->getArmor(items[i]));
				tmp = this->addImage(w->getAttribute("sprite"), x, y, size);
			} else if (Game::rList->checkExists(items[i])) {
				w = new Ring(Game::rList->getRing(items[i]));
				tmp = this->addImage(w->getAttribute("sprite"), x, y, size);
			}
			this->_bag.push_back(tmp);
			if (i == this->_g->getHero()->getInventory()->getNumFocus()) {
				Game::currentGame->tooltip->clearInfo();
				Game::currentGame->tooltip->info(w);
				tmp = this->addImage("Resources/Images/HUD/selecItem.png", x, y, size + 15);
				this->_bag.push_back(tmp);
			}
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
	this->_allElems.push_back(t);
	this->_minimap.push_back(t);
}

//! Display the minimap in the HUD
void	HUDWindow::minimap(void) {
	int		x, y, x2, y2;
	HUDActor *tmp;
	std::list<HUDActor *>::iterator		it;
	std::map<std::string, int>			doors;

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
			if (x2 == Game::currentX && y2 == Game::currentY) {
				tmp->SetColor(0, 1, 0);
			}
			else
				tmp->SetColor(1, 1, 1);
			tmp->SetDrawShape(ADS_Square);
			tmp->SetLayer(100);
			theWorld.Add(tmp);
			this->_allElems.push_back(tmp);
			this->_minimap.push_back(tmp);

			doors = Game::currentGame->maps->getMapXY()[y2][x2].doors;
			if (doors.find("right") != doors.end())
				this->_drawDoor(Vector2(1, 5), Vector2(x + 1 + (40 / 2), y));
			if (doors.find("left") != doors.end())
				this->_drawDoor(Vector2(1, 5), Vector2(x - (40 / 2), y));
			if (doors.find("up") != doors.end())
				this->_drawDoor(Vector2(5, 1), Vector2(x, y - (27 / 2) - 1));
			if (doors.find("down") != doors.end())
				this->_drawDoor(Vector2(5, 1), Vector2(x, y + (27 / 2) + 1));
		}
	}
}

//! Display an entire level IG
void	HUDWindow::bigMap(void) {
	std::vector<std::vector<Map> >		map = Game::currentGame->maps->getMapXY();
	HUDActor							*tmp;
	std::list<HUDActor *>::iterator		it;
	int			x, y, x2, y2;
	static int			doNotDelete = 0;

	if (HUDWindow::isToggled == 0) {
		this->_doNotDelete = 1;
		for (y = 0, y2 = (theCamera.GetWindowHeight() / 2) - 100; y < map.size(); y++, y2 += 28) {
			for (x = 0, x2 = (theCamera.GetWindowWidth() / 2) - 100; x < map[y].size(); x++, x2 += 41) {
				if (map[y][x].getIsUsed()) {
					tmp = new HUDActor();
					tmp->SetSize(40, 27);
					tmp->SetPosition(x2, y2);
					if (x == Game::currentX && y == Game::currentY) {
						this->_currentObjectMap = tmp;
						tmp->SetColor(0, 1, 0, 0);
					} if (map[y][x].getSpecial() != "" && map[y][x].getSpecial() != "starter") {
						this->setText(std::string(1, map[y][x].getSpecial()[0] - 32), x2 - 2, y2 + 1, Vector3(1, 0, 0), 1);
					} if (Game::currentGame->getHero()->_totem != nullptr) {
						if (std::to_string(x) == Game::currentGame->getHero()->_totem->getAttribute("currentX") 
								&& std::to_string(y) == Game::currentGame->getHero()->_totem->getAttribute("currentY")) {
							this->setText("To", x2 - 2, y2 + 1, Vector3(1, 0, 0), 1);
						}
					} else
						tmp->SetColor(1, 1, 1, 0);
					tmp->SetDrawShape(ADS_Square);
					tmp->SetLayer(100);
					theWorld.Add(tmp);
					this->_bigMapList.push_back(tmp);
				}
			}
		}
		for (it = this->_bigMapList.begin(); it != this->_bigMapList.end(); it++) {
			if ((*it) == this->_currentObjectMap)
				(*it)->ChangeColorTo(Color(0, 1, 0, 0.5), 1);
			else
				(*it)->ChangeColorTo(Color(1, 1, 1, 0.5), 1);
		}
		HUDWindow::isToggled = 1;
	} else {
		this->deleteBigMap(0);
		HUDWindow::isToggled = 0;
		this->_doNotDelete = 0;
	}
}

void	HUDWindow::deleteBigMap(int n) {
	std::list<HUDActor *>::iterator		it;

	if (n == 0) {
		for (it = this->_bigMapList.begin(); it != this->_bigMapList.end(); it++) {
			if ((*it) == this->_currentObjectMap)
				(*it)->ChangeColorTo(Color(0, 1, 0, 0), 1);
			else
				(*it)->ChangeColorTo(Color(1, 1, 1, 0), 1);
		}
		theSwitchboard.DeferredBroadcast(new Message("deleteMapPressed"), 1);
		this->removeText("To");
		this->removeText("B");
		this->removeText("S");
		this->removeText("D");
	} else {
		if (this->_doNotDelete == 0) {
			for (it = this->_bigMapList.begin(); it != this->_bigMapList.end(); it++)
				theWorld.Remove(*it);
			this->_bigMapList.clear();
		}
	}
}

void	HUDWindow::updateBigMap(void) {
	if (HUDWindow::isToggled == 1) {
		std::vector<std::vector<Map> >		map = Game::currentGame->maps->getMapXY();
		HUDActor							*tmp;
		int			x, y, x2, y2;
		std::list<HUDActor *>::iterator		it;

		for (it = this->_bigMapList.begin(); it != this->_bigMapList.end(); it++) {
			if ((*it)->GetPosition() == this->_currentObjectMap->GetPosition() &&
					(*it) != this->_currentObjectMap)
				theWorld.Remove(*it);
		}
		this->_currentObjectMap->ChangeColorTo(Color(1, 1, 1, 0.5), 0.5);
		for (y = 0, y2 = (theCamera.GetWindowHeight() / 2) - 100; y < map.size(); y++, y2 += 28) {
			for (x = 0, x2 = (theCamera.GetWindowWidth() / 2) - 100; x < map[y].size(); x++, x2 += 41) {
				if (x == Game::currentX && y == Game::currentY) {
					tmp = new HUDActor();
					tmp->SetSize(40, 27);
					tmp->SetPosition(x2, y2);
					this->_currentObjectMap = tmp;
					tmp->SetColor(0, 1, 0, 0);
					tmp->SetDrawShape(ADS_Square);
					tmp->SetLayer(100);
					theWorld.Add(tmp);
					this->_bigMapList.push_back(tmp);
					tmp->ChangeColorTo(Color(0, 1, 0, 0.5), 0.5);
					if (map[y][x].getSpecial() != "" && map[y][x].getSpecial() != "starter")
						this->setText(std::string(1, map[y][x].getSpecial()[0] - 32), x2 - 2, y2 + 1, Vector3(1, 0, 0), 1);
				}
			}
		}
	}
}

void		HUDWindow::addTotemToBigMap(void) {
	if (HUDWindow::isToggled) {
		std::vector<std::vector<Map> >		map = Game::currentGame->maps->getMapXY();
		int			x, y, x2, y2;

		for (y = 0, y2 = (theCamera.GetWindowHeight() / 2) - 100; y < map.size(); y++, y2 += 28) {
			for (x = 0, x2 = (theCamera.GetWindowWidth() / 2) - 100; x < map[y].size(); x++, x2 += 41) {
				if (x == Game::currentX && y == Game::currentY) {
					this->setText("T", x2 - 2, y2 + 1, Vector3(1, 0, 0), 1);
				}
			}
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
	this->setText(std::to_string(this->_g->getHero()->getLevel()), theCamera.GetWindowWidth() / 20 * 0.3, theCamera.GetWindowHeight() / 20 * 0.9, Vector3(1, 1, 1), 1, "MediumGamefont");
}

void	HUDWindow::spells(void) {
	this->addImage("Resources/HUD/spells_bg.png", (theCamera.GetWindowWidth() / 20) * 7, theCamera.GetWindowHeight() / 20 * 1.5, 
			Vector2(theCamera.GetWindowWidth() / 15 * 0.8, theCamera.GetWindowHeight() / 10 * 1.2), 15);
	this->addImage("Resources/HUD/spell_bg.png", (theCamera.GetWindowWidth() / 20) * 7 + 3, theCamera.GetWindowHeight() / 20, 
			Vector2(theCamera.GetWindowWidth() / 25), 15);
	this->addImage("Resources/Images/Skills/" + Game::currentGame->getHero()->getSpeMove() + ".png", (theCamera.GetWindowWidth() / 20) * 7 + 1, theCamera.GetWindowHeight() / 20 * 0.9, 
			Vector2(theCamera.GetWindowWidth() / 35), 15);
	this->setText("T", (theCamera.GetWindowWidth() / 20) * 7.28, theCamera.GetWindowHeight() / 20 * 1.5, Vector3(0.2, 0.12, 0), 1, "MediumGamefont");
	this->addImage("Resources/HUD/spell_bg.png", (theCamera.GetWindowWidth() / 20) * 7 + 3, theCamera.GetWindowHeight() / 20 * 2.1, 
			Vector2(theCamera.GetWindowWidth() / 25), 15);
	if (Game::currentGame->getHero()->getSpeAtt() != "")
		this->addImage("Resources/Images/Skills/" + Game::currentGame->getHero()->getSpeAtt() + ".png", (theCamera.GetWindowWidth() / 20) * 7 + 1, theCamera.GetWindowHeight() / 20 * 1.9, 
			Vector2(theCamera.GetWindowWidth() / 35), 15);
	this->setText("Y", (theCamera.GetWindowWidth() / 20) * 7.28, theCamera.GetWindowHeight() / 20 * 2.57, Vector3(0.2, 0.12, 0), 1, "MediumGamefont");
}

void	HUDWindow::spellText(void) {
	if (Game::started && Game::isInMenu == 0) {
		this->setText("Y", (theCamera.GetWindowWidth() / 20) * 7.28, theCamera.GetWindowHeight() / 20 * 2.57, Vector3(0.2, 0.12, 0), 1, "MediumGamefont");
		this->setText("T", (theCamera.GetWindowWidth() / 20) * 7.28, theCamera.GetWindowHeight() / 20 * 1.5, Vector3(0.2, 0.12, 0), 1, "MediumGamefont");
	}
}

void	HUDWindow::clearHUD(void) {
	std::list<HUDActor *>::iterator		it;

	for (it = this->_allElems.begin(); it != this->_allElems.end(); it++) {
		theWorld.Remove(*it);
	}
	this->removeText(this->_gold);
	if (this->_g)
		this->removeText(std::to_string(this->_g->getHero()->getLevel()));
}

void	HUDWindow::speAttCooldown(int time) {
	HUDActor	*h = new HUDActor();

	h->SetPosition((theCamera.GetWindowWidth() / 20) * 7 + 2, theCamera.GetWindowHeight() / 20 * 2.05);
	h->SetSize(theCamera.GetWindowWidth() / 27);
	h->SetLayer(100);
	h->SetSprite("Resources/HUD/spell_cooldown.png");
	theWorld.Add(h);
	this->_attackCooldown = h;
	this->_cooldownAtt = time + 1;
	theSwitchboard.SubscribeTo(this, "second");
	this->ReceiveMessage(new Message("second"));
}

void	HUDWindow::speMoveCooldown(int time) {
	HUDActor	*h = new HUDActor();

	h->SetPosition((theCamera.GetWindowWidth() / 20) * 7 + 2, theCamera.GetWindowHeight() / 21);
	h->SetSize(theCamera.GetWindowWidth() / 27);
	h->SetLayer(100);
	h->SetSprite("Resources/HUD/spell_cooldown.png");
	theWorld.Add(h);
	this->_moveCooldown = h;
	this->_cooldownMove = time + 1;
	theSwitchboard.SubscribeTo(this, "second");
	this->ReceiveMessage(new Message("second"));
}


void	HUDWindow::ReceiveMessage(Message *m) {
	static size_t		lastTimestamp = 0;
	static HUDWindow::Text	*attText = nullptr;
	static HUDWindow::Text	*moveText = nullptr;

	if (m->GetMessageName() == "speAttCooldownEnd") {
	} else if (m->GetMessageName() == "speMoveCooldownEnd") {
	} else if (m->GetMessageName() == "second") {
		if (lastTimestamp != time(NULL)) {
			lastTimestamp = time(NULL);
			if (attText)
				this->removeText(attText);
			attText = nullptr;
			if (this->_cooldownAtt >= 2) {
				attText = this->setText(std::to_string(--this->_cooldownAtt), (theCamera.GetWindowWidth() / 20) * 7, theCamera.GetWindowHeight() / 20 * 2.1, Vector3(1, 0, 0), 1);
				theSwitchboard.DeferredBroadcast(new Message("second"), 1);
			}
			if (moveText)
				this->removeText(moveText);
			moveText = nullptr;
			if (this->_cooldownMove >= 2) {
				moveText = this->setText(std::to_string(--this->_cooldownMove), (theCamera.GetWindowWidth() / 20) * 7, theCamera.GetWindowHeight() / 20, Vector3(1, 0, 0), 1);
				theSwitchboard.DeferredBroadcast(new Message("second"), 1);
			}
			if (this->_cooldownAtt < 1 && this->_cooldownMove < 1) {
				theSwitchboard.UnsubscribeFrom(this, "second");
			}
		}
	} else if (m->GetMessageName() == "enterPressed") {
		if (this->_isInDialog) {
			this->dialog(this->_currentDialog);
		}
	} else if (m->GetMessageName().substr(0, 6) == "remove") {
		std::string		message = m->GetMessageName().substr(6, m->GetMessageName().length() - 6);
		this->removeText(message);
	}
}

void	HUDWindow::dialog(std::string name) {
	static		int		isCreated = 0;
	if (isCreated == 0) {
		if (std::find(this->_dialogPassed.begin(), this->_dialogPassed.end(), name) != this->_dialogPassed.end())
			return ;
		this->_createDialog();
		isCreated = 1;
		this->_isInDialog = 1;
		this->_currentDialog = name;
		theSwitchboard.SubscribeTo(this, "enterPressed");
		this->_it = this->_dialogs[name].begin();
		this->_dialogPassed.push_back(name);
	} else {
		this->removeText(this->_it->asString());
		this->_it++;
	}
	if (this->_it == this->_dialogs[name].end()) {
		isCreated = 0;
		this->_cleanDialog();
		theSwitchboard.UnsubscribeFrom(this, "enterPressed");
		if (name == "drink")
			theSwitchboard.DeferredBroadcast(new Message("startIntro"), 1);
	} else {
		this->setText(this->_it->asString(), 400, theCamera.GetWindowHeight() - 100, Vector3(0, 0, 0), 1, "BigGamefont");
	}
}

void	HUDWindow::_createDialog(void) {
	Game::currentGame->getHero()->unsubscribeFromAll();
	HUDActor	*fade = new HUDActor(), *charac = new HUDActor(), *dialog = new HUDActor();
	fade->SetSize(theCamera.GetWindowWidth(), theCamera.GetWindowHeight());
	fade->SetPosition(theCamera.GetWindowWidth() / 2, theCamera.GetWindowHeight() / 2);
	fade->SetColor(0, 0, 0, 0.3);
	fade->SetLayer(200);
	theWorld.Add(fade);
	this->_fade = fade;
	charac->SetSize(400);
	charac->SetPosition(200, theCamera.GetWindowHeight() - 200);
	charac->LoadSpriteFrames(Game::currentGame->getHero()->getAttribute("spritesFrame"));
	charac->SetLayer(202);
	charac->PlaySpriteAnimation(0.3, SAT_Loop, Game::currentGame->getHero()->_getAttr("breath", "beginFrame_right").asInt(), Game::currentGame->getHero()->_getAttr("breath", "endFrame_right").asInt());
	theWorld.Add(charac);
	this->_perso = charac;
	dialog->SetSize(theCamera.GetWindowWidth() * 1.5, 200);
	dialog->SetPosition(100, theCamera.GetWindowHeight() - 100);
	dialog->SetSprite("Resources/Images/big_dialog.png");
	dialog->SetLayer(201);
	theWorld.Add(dialog);
	this->_backDialog = dialog;
	this->setText("Press <Enter>", theCamera.GetWindowWidth() / 2 + 50, theCamera.GetWindowHeight() - 20, Vector3(0, 0, 0), 0.3f);
}

void	HUDWindow::_cleanDialog(void) {
	Game::currentGame->getHero()->subscribeToAll();
	theWorld.Remove(this->_fade);
	theWorld.Remove(this->_perso);
	theWorld.Remove(this->_backDialog);
	this->removeText("Press <Enter>");
}

void	HUDWindow::setGame(Game *g) { this->_g = g; };
void	HUDWindow::setMaxHP(int h) { this->_maxHP = h; };
HUDActor	*HUDWindow::getAttCooldown(void) { return this->_attackCooldown; };
HUDActor	*HUDWindow::getMoveCooldown(void) { return this->_moveCooldown; };

int		HUDWindow::isToggled = 0;
