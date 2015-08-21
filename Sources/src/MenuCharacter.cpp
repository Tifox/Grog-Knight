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
 * File: MenuCharacter.hpp
 * @date 2015-07-23 14:45
 * @author Louis Solofrizzo <louis@ne02ptzero.me>
 */

#include "MenuCharacter.hpp"

MenuCharacter::MenuCharacter(void) : Characters("MenuCharacter") {
	this->addAttribute("type", "Hero");
	this->addAttribute("physic", "1");
	this->addAttribute("hero", "1");
	this->_currentTrigger = "";
	this->_target = this->_target2 = nullptr;
	this->_currentItemInfo = nullptr;
	this->_isBlock = 0;
	this->_chooseEquipment = 0;
	this->_character = "Warrior";
	theSwitchboard.SubscribeTo(this, "enterPressed");
	theSwitchboard.SubscribeTo(this, "chooseEquipment");
	theSwitchboard.SubscribeTo(this, "returnPressed");
}

MenuCharacter::~MenuCharacter(void) {

}

void	MenuCharacter::init(void) {
	this->AnimCallback("base");
}

void	MenuCharacter::actionCallback(std::string name, int status) {
}

void	MenuCharacter::BeginContact(Elements *elem, b2Contact *contact) {
	contact->SetEnabled(false);
	if (elem->getAttribute("trigger") != "") {
		if (this->_currentTrigger == elem->getAttribute("trigger"))
			this->_callTrigger(elem->getAttribute("trigger"), 0);
		else
			this->_callTrigger(elem->getAttribute("trigger"), 1);
	} if (elem->getAttribute("triggerOn") != "") {
		this->trigger(elem->getAttribute("triggerOn"), 1);
	} if (elem->getAttribute("triggerOff") != "") {
		this->trigger(elem->getAttribute("triggerOff"), 0);
	}
}

void	MenuCharacter::EndContact(Elements *elem, b2Contact *contact) {
}

void	MenuCharacter::trigger(std::string name, int status) {
	if (status == 1)
		this->_currentTrigger = name;
	else
		this->_currentTrigger = "";
	if (name == "chooseCharacter") {
		if (status == 1) {
			this->_showTextInfo("Press enter to choose a new character");
		} else {
			Game::getHUD()->removeText("Press enter to choose a new character");
			theWorld.Remove(this->_image);
			this->_image = nullptr;
		}
	} else if (name == "equipment") {
		if (status == 1) {
			this->_showTextInfo("Press enter to choose your equipment");
			this->_openCloset();
		} else {
			Game::getHUD()->removeText("Press enter to choose your equipment");
			theWorld.Remove(this->_image);
			this->_image = nullptr;
			this->_closeCloset();
		}
	} else if (name == "startGame") {
		theSwitchboard.UnsubscribeFrom(this, "chooseEquipment");
		theSwitchboard.UnsubscribeFrom(this, "returnPressed");
		theSwitchboard.UnsubscribeFrom(this, "enterPressed");
		Game::isInMenu = 0;
		Game::asToStart = 1;
	}
}

void	MenuCharacter::ReceiveMessage(Message *m) {
	Characters::ReceiveMessage(m);
	if (m->GetMessageName() == "enterPressed") {
		if (this->_currentTrigger == "chooseCharacter") {
			if (this->_isBlock) {
				this->ClearSpriteInfo();
				this->LoadSpriteFrames("Resources/Images/Menu/"+ this->_choicePointer->getAttribute("type") +"/perso_000.png");
				this->_character = this->_choicePointer->getAttribute("type");
				theCamera.MoveTo(Vector3(Game::currentGame->maps->getMapXY()[Game::currentY][Game::currentX].getXMid(),
				Game::currentGame->maps->getMapXY()[Game::currentY][Game::currentX].getYMid() + 1.8, 18.002), 1, true);
				this->_isBlock = 0;
			   std::list<Elements *>::iterator	it;
				for (it = this->_choices.begin(); it != this->_choices.end(); it++)
					theWorld.Remove(*it);
				theWorld.Remove(this->_target);
				this->_target = nullptr;
				this->_choices.clear();
				this->_choicePointer = nullptr;
			} else {
				Elements *elem = new Elements();
				elem->SetSprite("Resources/Images/Menu/perso_1.png");
				elem->SetSize(3);
				elem->SetPosition(32, -23);
				elem->addAttribute("type", "Warrior");
				theWorld.Add(elem);
				this->_choices.push_back(elem);
				this->_choicePointer = elem;

				elem = new Elements();
				elem->SetPosition(36, -23);
				elem->SetSize(3);
				elem->addAttribute("type", "BowGurl");
				elem->SetSprite("Resources/Images/Menu/perso_2.png");
				theWorld.Add(elem);
				this->_choices.push_back(elem);
				theCamera.MoveTo(Vector3(34, -23, 5), 1, true);
				this->_makeItChoice();
				this->_isBlock = 1;
			}
			} else if (this->_currentTrigger == "equipment" && this->_chooseEquipment == 0) {
			theCamera.MoveTo(Vector3(78.5, -19.5, 12.5), 0.5, true, "chooseEquipment");
			Game::getHUD()->removeText("Press enter to choose your equipment");
			theWorld.Remove(this->_image);
			this->_isBlock = 1;
			this->_chooseEquipment = 1;
		} else if (this->_currentTrigger == "equipment" && this->_chooseEquipment == 1) {
			this->_updateSelection();
		}
	} else if (m->GetMessageName() == "chooseEquipment") {
		this->_equipmentChoose();
	} else if (m->GetMessageName() == "returnPressed" && this->_chooseEquipment == 1) {
		this->_cleanCloset();
	}
}

void	MenuCharacter::AnimCallback(String s) {
	if (s == "base") {
		this->_setCategory("breath");
		if (this->_latOrientation == LEFT) {
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
					this->_getAttr("beginFrame_left").asInt(),
					this->_getAttr("endFrame_left").asInt(), "base");
		} else if (this->_latOrientation == RIGHT) {
			this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_OneShot,
					this->_getAttr("beginFrame_right").asInt(),
					this->_getAttr("endFrame_right").asInt(), "base");
		}
	}
}

void	MenuCharacter::_forward(int status) {
	if (!this->_isBlock) {
		this->_setCategory("forward");
		if (this->_forwardFlag == true && status == 1)
			return ;
		this->_forwardFlag = true;

		if (status == 1) {
			if (this->_isRunning == 0 || this->_isRunning == 2) {
				this->_orientation = RIGHT;
				this->_latOrientation = RIGHT;
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
					this->_getAttr("beginFrame").asInt(), this->_getAttr("endFrame").asInt());
			}
			this->_isRunning = 1;
			Game::startRunning(this);
		} else if (status == 0) {
			Game::stopRunning(this);
			this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
			this->_isRunning = 0;
			this->AnimCallback("base");
		} else {
			this->GetBody()->SetLinearVelocity(b2Vec2(this->_getAttr("force").asFloat(), this->GetBody()->GetLinearVelocity().y));
			this->_isRunning = 1;
		}
	} else {
		if (status == 1) {
			std::list<Elements *>::iterator	it = std::find(this->_choices.begin(), this->_choices.end(), this->_choicePointer);
			if ((++it) != this->_choices.end()) {
				this->_choicePointer = *it;
				this->_makeItChoice();
			}
		}
	}
}

void	MenuCharacter::_backward(int status) {
	if (!this->_isBlock) {
		this->_setCategory("backward");
		if (this->_backwardFlag == true && status == 1)
			return ;
		this->_backwardFlag = true;

		if (status == 1) {
			if (this->_isRunning == 0 || this->_isRunning == 1) {
				this->_orientation = LEFT;
				this->_latOrientation = LEFT;
				this->PlaySpriteAnimation(this->_getAttr("time").asFloat(), SAT_Loop,
					this->_getAttr("beginFrame").asInt(), this->_getAttr("endFrame").asInt());
			}
			this->_isRunning = 2;
			Game::startRunning(this);
		} else if (status == 0) {
			Game::stopRunning(this);
			this->GetBody()->SetLinearVelocity(b2Vec2(0, this->GetBody()->GetLinearVelocity().y));
			this->_isRunning = 0;
			this->AnimCallback("base");
		} else {
			this->GetBody()->SetLinearVelocity(b2Vec2(-(this->_getAttr("force").asFloat()), this->GetBody()->GetLinearVelocity().y));
			this->_isRunning = 2;
		}
	} else {
		if (status == 1) {
			std::list<Elements *>::iterator	it = std::find(this->_choices.begin(), this->_choices.end(), this->_choicePointer);
			if (it != this->_choices.begin()) {
				it--;
				this->_choicePointer = *it;
				this->_makeItChoice();
			}
		}
	}
}

void	MenuCharacter::_up(int status) {
	if (status == 1 && this->_currentTrigger == "equipment") {
		std::list<Elements *>::iterator		it = std::find(this->_backCloset.begin(), this->_backCloset.end(), this->_closetChoice);
		if (it != this->_backCloset.begin()) {
			this->_closetChoice = *(--it);
			this->_closetBackChoiceUpdate();
		}
	}
}

void	MenuCharacter::_down(int status) {
	if (status == 1 && this->_currentTrigger == "equipment") {
		std::list<Elements *>::iterator		it = std::find(this->_backCloset.begin(), this->_backCloset.end(), this->_closetChoice);
		if (it != this->_backCloset.end()) {
			this->_closetChoice = *(++it);
			this->_closetBackChoiceUpdate();
		}
	}
}


void	MenuCharacter::_showTextInfo(std::string text, std::string switchboard) {
	int x = theCamera.GetWindowWidth(), y = theCamera.GetWindowHeight();
	this->_image = Game::getHUD()->addImage("Resources/Images/HUD/talk.png", x / 2, y - (y / 15), Vector2(x / 3, y / 10), 100);
	Game::getHUD()->setText(text, (x / 2) - (x / 6) + (x / 20), y - (y / 15), Vector3(0, 0, 1), 1);
}

void	MenuCharacter::_makeItChoice(void) {
	if (this->_target != nullptr)
		theWorld.Remove(this->_target);

	Actor	*target = new Actor();
	target->SetSprite("Resources/Images/HUD/selecItem.png");
	target->SetSize(4);
	target->SetLayer(100);
	target->SetPosition(this->_choicePointer->GetPosition().X, this->_choicePointer->GetPosition().Y);
	theWorld.Add(target);
	if (this->_currentTrigger == "equipment") {
		this->_showSelectInfo(this->_choicePointer);
	}
	this->_target = target;
}

void		MenuCharacter::_openCloset(void) {
	Elements	*elem = new Elements();
	elem->LoadSpriteFrames("Resources/Images/Menu/dressing_door_000.png");
	elem->SetSize(36, 22);
	elem->SetPosition(79, -19.5);
	theWorld.Add(elem);
	if (this->_character == "BowGurl")
		elem->PlaySpriteAnimation(0.3, SAT_OneShot, 1, 4);
	else if (this->_character == "Warrior")
		elem->PlaySpriteAnimation(0.3, SAT_OneShot, 5, 8);
	this->_closet = elem;
}

void		MenuCharacter::_closeCloset(void) {
	if (this->_character == "BowGurl")
		this->_closet->PlaySpriteAnimation(0.3, SAT_OneShot, 4, 1, "closeCloset");
	else if (this->_character == "Warrior")
		this->_closet->PlaySpriteAnimation(0.3, SAT_OneShot, 8, 5, "closeCloset");
}

void		MenuCharacter::_equipmentChoose(void) {
	int			i, y;
	Elements	*back;

	Game::getHUD()->setText("Equipment", 50.5, 110, Vector3(32, 24, 32), 1, "smallTitle");
	for (i = 1, y = -14; i < 4; i++, y -= 5) {
		back = new Elements();
		back->SetSize(3);
		back->SetPosition(66, y);
		back->SetSprite("Resources/Images/Menu/icon_"+ std::to_string(i) +".png");
		back->addAttribute("number", std::to_string(i));
		theWorld.Add(back);
		this->_backCloset.push_back(back);
	}
	// Pool
	if (!this->_ringList.size()) {
		this->_ringList = (new RingList())->get3Starters(1);
		this->_weaponList = (new WeaponList())->get3Starters(1);
		this->_armorList = (new ArmorList())->get3Starters(1);
	}
	this->_closetChoice = *(this->_backCloset.begin());
	this->_closetBackChoiceUpdate();
}

void		MenuCharacter::_closetBackChoiceUpdate(void) {
	if (this->_target2 != nullptr)
		theWorld.Remove(this->_target2);
	this->_target2 = new Actor();
	this->_target2->SetSize(4);
	this->_target2->SetPosition(this->_closetChoice->GetPosition().X, this->_closetChoice->GetPosition().Y);
	this->_target2->SetSprite("Resources/Images/HUD/selecItem2.png");
	theWorld.Add(this->_target2);

	if (this->_choices.size() > 0) {
		std::list<Elements *>::iterator		it;

		for (it = this->_choices.begin(); it != this->_choices.end(); it++)
			theWorld.Remove(*it);
		for (it = this->_backChoices.begin(); it != this->_backChoices.end(); it++)
			theWorld.Remove(*it);
		this->_choices.clear();
		this->_backChoices.clear();
	}

	Elements		*tmp;
	int				x;
	// This is nasty as fuck. Gotta find another way for this
	if (this->_closetChoice->getAttribute("number") == "1") {
		std::list<Ring *>::iterator		it;

		for (it = this->_ringList.begin(), x = 74.5; it != this->_ringList.end(); it++, x += 5) {
			tmp = new Elements();
			tmp->SetSize(3);
			tmp->SetPosition(x, -14);
			tmp->SetSprite("Resources/Images/Menu/icon_yellow.png");
			theWorld.Add(tmp);
			this->_backChoices.push_back(tmp);

			tmp = new Elements();
			tmp->SetSize(2.5);
			tmp->SetPosition(x, -14);
			tmp->SetSprite((*it)->getSprite());
			tmp->addAttribute("Sprite", (*it)->getSprite());
			tmp->addAttribute("Name", (*it)->getName());
			theWorld.Add(tmp);
			this->_choices.push_back(tmp);
		}
	} else if (this->_closetChoice->getAttribute("number") == "2") {
		std::list<Armor *>::iterator		it;

		for (it = this->_armorList.begin(), x = 74.5; it != this->_armorList.end(); it++, x += 5) {
			tmp = new Elements();
			tmp->SetSize(3);
			tmp->SetPosition(x, -14);
			tmp->SetSprite("Resources/Images/Menu/icon_grey.png");
			theWorld.Add(tmp);
			this->_backChoices.push_back(tmp);

			tmp = new Elements();
			tmp->SetSize(2.5);
			tmp->SetPosition(x, -14);
			tmp->SetSprite((*it)->getSprite());
			tmp->addAttribute("Sprite", (*it)->getSprite());
			tmp->addAttribute("Name", (*it)->getName());
			theWorld.Add(tmp);
			this->_choices.push_back(tmp);
		}
	} else if (this->_closetChoice->getAttribute("number") == "3") {
		std::list<Weapon *>::iterator		it;

		for (it = this->_weaponList.begin(), x = 74.5; it != this->_weaponList.end(); it++, x += 5) {
			tmp = new Elements();
			tmp->SetSize(3);
			tmp->SetPosition(x, -14);
			tmp->SetSprite("Resources/Images/Menu/icon_red.png");
			theWorld.Add(tmp);
			this->_backChoices.push_back(tmp);

			tmp = new Elements();
			tmp->SetSize(2.5);
			tmp->SetPosition(x, -14);
			tmp->SetSprite((*it)->getSprite());
			tmp->addAttribute("Sprite", (*it)->getSprite());
			tmp->addAttribute("Name", (*it)->getName());
			theWorld.Add(tmp);
			this->_choices.push_back(tmp);
		}
	}
	this->_choicePointer= *(this->_choices.begin());
	this->_makeItChoice();
}

void			MenuCharacter::_updateSelection(void) {
	Elements	*tmp, *tmp2;
	tmp = new Elements(); tmp2 = new Elements();
	tmp->SetSize(2.5);
	tmp2->SetSize(3);
	tmp->SetSprite(this->_choicePointer->getAttribute("Sprite"));
	tmp->SetPosition(66, -9 + (-5 * std::stoi(this->_closetChoice->getAttribute("number"))));
	tmp2->SetPosition(66, -9 + (-5 * std::stoi(this->_closetChoice->getAttribute("number"))));

	if (this->_closetChoice->getAttribute("number") == "1") {
		if (this->_equipSelection["ring"]) {
			theWorld.Remove(this->_equipSelection["ring"]);
		} else {
			tmp2->SetSprite("Resources/Images/Menu/icon_yellow.png");
			theWorld.Add(tmp2);
		}
		this->_equipSelection["ring"] = tmp;
	} else if (this->_closetChoice->getAttribute("number") == "2") {
		if (this->_equipSelection["Armor"]) {
			theWorld.Remove(this->_equipSelection["Armor"]);
		} else {
			tmp2->SetSprite("Resources/Images/Menu/icon_grey.png");
			theWorld.Add(tmp2);
		}
		this->_equipSelection["Armor"] = tmp;
	} else if (this->_closetChoice->getAttribute("number") == "3") {
		if (this->_equipSelection["Weapon"]) {
			theWorld.Remove(this->_equipSelection["Weapon"]);
		} else {
			tmp2->SetSprite("Resources/Images/Menu/icon_red.png");
			theWorld.Add(tmp2);
		}
		this->_equipSelection["Weapon"] = tmp;
	}
	theWorld.Add(tmp);
}

void		MenuCharacter::_showSelectInfo(Elements *obj) {
	Actor	*tmp;
	std::string		name;
	HUDWindow		*hud = Game::getHUD();
	int				ww = theCamera.GetWindowWidth(), wh = theCamera.GetWindowHeight();

	if (this->_currentItemInfo != nullptr) {
		std::list<std::string>::iterator	it;

		theWorld.Remove(this->_currentItemInfo->background);
		theWorld.Remove(this->_currentItemInfo->sprite);
		for (it = this->_currentItemInfo->textInfo.begin();
			it != this->_currentItemInfo->textInfo.end(); it++)
			hud->removeText(*it);
	}

	this->_currentItemInfo = new MenuCharacter::itemInfo();
	// Background of the item
	tmp = new Actor();
	tmp->SetPosition(92, -13);
	tmp->SetSprite("Resources/Images/Menu/itemInfoBackground.png");
	tmp->SetSize(3);
	theWorld.Add(tmp);
	this->_currentItemInfo->background = tmp;
	if (this->_closetChoice->getAttribute("number") == "1") {
		Ring	*item = new Ring(obj->getAttribute("Name"));
		name = item->getSprite();
		hud->setText(item->getName(), (ww - (ww / 7)) - (item->getName().length() / 2 * 6), 
			(wh / 3), Vector3(0.68, 0.68, 0.68), 1, "BigGamefont");
		this->_currentItemInfo->textInfo.push_back(item->getName());
		this->_flavorInfo(item->getFlavor());
	} else if (this->_closetChoice->getAttribute("number") == "2") {
		Armor	*item = new Armor(obj->getAttribute("Name"));
		name = item->getSprite();
		hud->setText(item->getName(), (ww - (ww / 7)) - (item->getName().length() / 2 * 6), 
			(wh / 3), Vector3(0.68, 0.68, 0.68), 1, "BigGamefont");
		this->_currentItemInfo->textInfo.push_back(item->getName());
		this->_flavorInfo(item->getFlavor());
	} else if (this->_closetChoice->getAttribute("number") == "3") {
		Weapon	*item = new Weapon(obj->getAttribute("Name"));
		name = item->getSprite();
		hud->setText(item->getName(), (ww - (ww / 7)) - (item->getName().length() / 2 * 6), 
			(wh / 3), Vector3(0.68, 0.68, 0.68), 1, "BigGamefont");
		this->_currentItemInfo->textInfo.push_back(item->getName());
		this->_flavorInfo(item->getFlavor());
	}

	// Icon of item
	tmp = new Actor();
	tmp->SetPosition(92, -13);
	tmp->SetSprite(name);
	tmp->SetSize(2.5);
	theWorld.Add(tmp);
	this->_currentItemInfo->sprite = tmp;
}

void		MenuCharacter::_flavorInfo(std::string n) {
	int		i;
	std::string					tmp;
	int				ww = theCamera.GetWindowWidth(), wh = theCamera.GetWindowHeight();
	int				y = (wh / 3) + 20, x = (ww - (ww / 4.5));
	HUDWindow		*hud = Game::getHUD();

	for (i = 0; i < n.length(); i++) {
		if (n[i] == '\n') {
			hud->setText(tmp, x, y, Vector3(0.75, 0.75, 0.75), 1);
			this->_currentItemInfo->textInfo.push_back(tmp);
			y += 20;
			tmp = "";
		} else
			tmp += n[i];
	}
	hud->setText(tmp, x, y, Vector3(0.75, 0.75, 0.75), 1);
	this->_currentItemInfo->textInfo.push_back(tmp);

}

void		MenuCharacter::_cleanCloset(void) {
	std::list<Elements *>::iterator		it;
	HUDWindow							*hud = Game::getHUD();

	theCamera.MoveTo(Vector3(Game::currentGame->maps->getMapXY()[Game::currentY][Game::currentX].getXMid(),
		Game::currentGame->maps->getMapXY()[Game::currentY][Game::currentX].getYMid(), 18.502), 1, true);
	for (it = this->_backCloset.begin(); it != this->_backCloset.end(); it++)
		theWorld.Remove(*it);
	for (it = this->_choices.begin(); it != this->_choices.end(); it++)
		theWorld.Remove(*it);
	for (it = this->_backChoices.begin(); it != this->_backChoices.end(); it++)
		theWorld.Remove(*it);
	theWorld.Remove(this->_target);
	theWorld.Remove(this->_target2);
	if (this->_currentItemInfo != nullptr) {
		std::list<std::string>::iterator	it;

		theWorld.Remove(this->_currentItemInfo->background);
		theWorld.Remove(this->_currentItemInfo->sprite);
		for (it = this->_currentItemInfo->textInfo.begin();
			it != this->_currentItemInfo->textInfo.end(); it++)
			hud->removeText(*it);
	}
	this->_target = this->_target2 = nullptr;
	hud->removeText("Equipment");
	this->_choices.clear();
	this->_backCloset.clear();
	this->_backChoices.clear();
	this->_choicePointer = nullptr;
	this->_isBlock = 0;
	this->_chooseEquipment = 0;
	//this->_showTextInfo("Press enter to choose a new character");
}
