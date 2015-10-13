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
	this->_characLvl = Quit::level;
	this->_descriptionBackground = this->_iconBackground = this->_icon = nullptr;
	this->_levelsBackground = this->_lvlUp = nullptr;
	// THIS IS TMP; DO NOT JUDGE ME
	Elements		*tmp;
	tmp = new Elements(); tmp->addAttribute("Name", "Bow_000"); this->_equipSelection["Weapon"] = tmp;
	tmp = new Elements(); tmp->addAttribute("Name", "Ring_000"); this->_equipSelection["ring"] = tmp;
	tmp = new Elements(); tmp->addAttribute("Name", "Armor_000"); this->_equipSelection["Armor"] = tmp;
	// END OF NASTY CODE. Well, actually no. But u know what i mean
	theSwitchboard.SubscribeTo(this, "enterPressed");
	theSwitchboard.SubscribeTo(this, "chooseEquipment");
	theSwitchboard.SubscribeTo(this, "returnPressed");
	this->_skillsLvl = std::vector<int>(4); this->_finalSkillChoices = std::vector<std::string>(4);
	std::fill(this->_finalSkillChoices.begin(), this->_finalSkillChoices.end(), "");
	this->_finalSkillTargets = std::vector<Elements *>(4);
	this->_skillsChoices = std::vector<std::list<Elements *> >(4);
	this->_skillsLvl[0] = 5; this->_skillsLvl[1] = 15; this->_skillsLvl[2] = 25; this->_skillsLvl[3] = 50;
	this->_finalSkillChoices[0] = "blink"; this->_finalSkillChoices[1] = "rapidFire";
	this->_getSkills();
	this->_kitchen();
}

MenuCharacter::~MenuCharacter(void) {

}

void	MenuCharacter::init(void) {
	this->AnimCallback("base");
}

void	MenuCharacter::actionCallback(std::string name, int status) {
}

void	MenuCharacter::BeginContact(Elements *elem, b2Contact *contact) {
	if (!elem)
		return ;
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
	if (!elem)
		return ;
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
		this->unsubscribeFromAll();
		theSwitchboard.UnsubscribeFrom(this, "enterPressed");
		theSwitchboard.UnsubscribeFrom(this, "chooseEquipment");
		theSwitchboard.UnsubscribeFrom(this, "returnPressed");

		Game::isInMenu = 0;
		Game::menuCharacter = this;
		Game::asToStart = 1;
	} else if (name == "skills") {
		if (status == 1)
			this->_showTextInfo("Press enter to choose your skills");
		else if (!status) {
			Game::getHUD()->removeText("Press enter to choose your skills");
			theWorld.Remove(this->_image);
			this->_image = nullptr;
		}
	}
}

void	MenuCharacter::ReceiveMessage(Message *m) {
	if (m->GetMessageName() != "jumpPressed" && m->GetMessageName() != "jumpRealeased")
		Characters::ReceiveMessage(m);
	if (m->GetMessageName() == "enterPressed") {
		if (this->_currentTrigger == "chooseCharacter") {
			if (this->_isBlock) {
				this->ClearSpriteInfo();
				this->LoadSpriteFrames("Resources/Images/Menu/"+ this->_choicePointer->getAttribute("type") +"/perso_000.png");
				if (this->_character != this->_choicePointer->getAttribute("type")) {
					this->_character = this->_choicePointer->getAttribute("type");
					this->_hideKitchen(1);
					this->_kitchen();
					Game::getHUD()->removeText("Press enter to choose your skills");
				}
				theCamera.MoveTo(Vector3(Game::currentGame->getCurrentMap().getXMid(),
							Game::currentGame->getCurrentMap().getYMid() + 1.8, 18.002), 1, true);
				this->_isBlock = 0;
				this->_ringList.clear();
				this->_weaponList.clear();
				this->_armorList.clear();
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
				elem->addAttribute("type", "Archer");
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
			this->_image = nullptr;
		} else if (this->_currentTrigger == "equipment" && this->_chooseEquipment == 1) {
			this->_updateSelection();
		} else if (this->_currentTrigger == "skills") {
			theCamera.MoveTo(Vector3(131, -19.5, 12.5), 0.5, true, "chooseSkills");
			Game::getHUD()->removeText("Press enter to choose your skills");
			theWorld.Remove(this->_image);
			this->_image = nullptr;
			this->_currentTrigger = "skillsChoices";
			this->_isBlock = 1;
			this->_choicePointer = *(this->_skillsChoices[0].begin());
			this->_makeSkillChoice();
			this->_levels();
		} else if (this->_currentTrigger == "skillsChoices") {
			Elements	*tmp = new Elements();
			int		max, i, current = atoi(this->_choicePointer->getAttribute("palier").c_str());

			tmp->SetSize(3); tmp->SetSprite("Resources/Images/kitchen_target.png");
			tmp->SetPosition(this->_choicePointer->GetPosition().X, this->_choicePointer->GetPosition().Y);
			theWorld.Add(tmp);
			if (this->_finalSkillTargets[current])
				theWorld.Remove(this->_finalSkillTargets[current]);
			this->_finalSkillTargets[current] = tmp;
			this->_finalSkillChoices[current] = this->_choicePointer->getAttribute("skill");
			for (max = 0; this->_skillsChoices[max].size() > 0; max++);
			for (i = 0; i < this->_finalSkillChoices.size() && this->_finalSkillChoices[i] != ""; i++);
			if (max == i)
				this->_hideKitchen();
			else
				this->_forward(1);

		}
	} else if (m->GetMessageName() == "chooseEquipment") {
		this->_equipmentChoose();
	} else if (m->GetMessageName() == "returnPressed" && this->_chooseEquipment == 1) {
		this->_cleanCloset();
	} else if (m->GetMessageName() == "jumpPressed" && this->_currentTrigger == "skillsChoices") {
		if (Quit::gold >= (this->_characLvl * 4))
			this->_levels();
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
			if (this->_currentTrigger == "skillsChoices") {
				int		current = atoi(this->_choicePointer->getAttribute("palier").c_str());

				if (this->_skillsChoices[current + 1].size() != 0) {
					std::list<Elements *> 				lst = this->_skillsChoices[current];
					std::list<Elements *>::iterator		it = std::find(lst.begin(), lst.end(), this->_choicePointer);
					int									distance = std::distance(lst.begin(), it);

					it = this->_skillsChoices[current + 1].begin();
					std::advance(it, distance);
					this->_choicePointer = *(it);
					this->_makeSkillChoice();
				}
			} else {
				std::list<Elements *>::iterator	it = std::find(this->_choices.begin(), this->_choices.end(), this->_choicePointer);
				if ((++it) != this->_choices.end()) {
					this->_choicePointer = *it;
					this->_makeItChoice();
				}
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
			if (this->_currentTrigger == "skillsChoices") {
				int		current = atoi(this->_choicePointer->getAttribute("palier").c_str());

				if (current != 0) {
					std::list<Elements *> 				lst = this->_skillsChoices[current];
					std::list<Elements *>::iterator		it = std::find(lst.begin(), lst.end(), this->_choicePointer);
					int									distance = std::distance(lst.begin(), it);

					it = this->_skillsChoices[current - 1].begin();
					std::advance(it, distance);
					this->_choicePointer = *(it);
					this->_makeSkillChoice();
				}
			} else {
				std::list<Elements *>::iterator	it = std::find(this->_choices.begin(), this->_choices.end(), this->_choicePointer);
				if (it != this->_choices.begin()) {
					it--;
					this->_choicePointer = *it;
					this->_makeItChoice();
				}
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
	} else if (status == 1 && this->_currentTrigger == "skillsChoices") {
		std::list<Elements *> 				lst = this->_skillsChoices[atoi(this->_choicePointer->getAttribute("palier").c_str())];
		std::list<Elements *>::iterator		it = std::find(lst.begin(), lst.end(), this->_choicePointer);

		if (it != lst.begin()) {
			this->_choicePointer = *(--it);
			this->_makeSkillChoice();
		}
	}
}

void	MenuCharacter::_down(int status) {
	if (status == 1 && this->_currentTrigger == "equipment") {
		std::list<Elements *>::iterator		it = std::find(this->_backCloset.begin(), this->_backCloset.end(), this->_closetChoice);
		if ((++it) != this->_backCloset.end()) {
			this->_closetChoice = *(it);
			this->_closetBackChoiceUpdate();
		}
	} else if (status == 1 && this->_currentTrigger == "skillsChoices") {
		std::list<Elements *> 				lst = this->_skillsChoices[atoi(this->_choicePointer->getAttribute("palier").c_str())];
		std::list<Elements *>::iterator		it = std::find(lst.begin(), lst.end(), this->_choicePointer);

		if (++it != lst.end()) {
			this->_choicePointer = *(it);
			this->_makeSkillChoice();
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
	if (this->_character == "Archer")
		elem->PlaySpriteAnimation(0.3, SAT_OneShot, 1, 4);
	else if (this->_character == "Warrior")
		elem->PlaySpriteAnimation(0.3, SAT_OneShot, 5, 8);
	this->_closet = elem;
}

void		MenuCharacter::_closeCloset(void) {
	if (this->_character == "Archer")
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
		this->_ringList = (new RingList())->get3Starters(1, this->_character);
		this->_weaponList = (new WeaponList())->get3Starters(1, this->_character);
		this->_armorList = (new ArmorList())->get3Starters(1, this->_character);
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
	tmp->addAttribute("Name", this->_choicePointer->getAttribute("Name"));
	tmp->SetPosition(66, -9 + (-5 * std::stoi(this->_closetChoice->getAttribute("number"))));
	tmp2->SetPosition(66, -9 + (-5 * std::stoi(this->_closetChoice->getAttribute("number"))));

	if (this->_closetChoice->getAttribute("number") == "1") {
		if (this->_equipSelection["ring"]) {
			theWorld.Remove(this->_equipSelection["ring"]);
		} else {
			tmp2->SetSprite("Resources/Images/Menu/icon_yellow.png");
			theWorld.Add(tmp2);
			this->_equipSelectionBack.push_back(tmp2);
		}
		this->_equipSelection["ring"] = tmp;
	} else if (this->_closetChoice->getAttribute("number") == "2") {
		if (this->_equipSelection["Armor"]) {
			theWorld.Remove(this->_equipSelection["Armor"]);
		} else {
			tmp2->SetSprite("Resources/Images/Menu/icon_grey.png");
			theWorld.Add(tmp2);
			this->_equipSelectionBack.push_back(tmp2);
		}
		this->_equipSelection["Armor"] = tmp;
	} else if (this->_closetChoice->getAttribute("number") == "3") {
		if (this->_equipSelection["Weapon"]) {
			theWorld.Remove(this->_equipSelection["Weapon"]);
		} else {
			tmp2->SetSprite("Resources/Images/Menu/icon_red.png");
			theWorld.Add(tmp2);
			this->_equipSelectionBack.push_back(tmp2);
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
		hud->setText(item->getDisplayName(), (ww - (ww / 7)) - (item->getDisplayName().length() / 2 * 6), 
				(wh / 3), Vector3(0.68, 0.68, 0.68), 1, "BigGamefont");
		this->_currentItemInfo->textInfo.push_back(item->getDisplayName());
		this->_flavorInfo(item->getFlavor());
	} else if (this->_closetChoice->getAttribute("number") == "2") {
		Armor	*item = new Armor(obj->getAttribute("Name"));
		name = item->getSprite();
		hud->setText(item->getDisplayName(), (ww - (ww / 7)) - (item->getDisplayName().length() / 2 * 6), 
				(wh / 3), Vector3(0.68, 0.68, 0.68), 1, "BigGamefont");
		this->_currentItemInfo->textInfo.push_back(item->getDisplayName());
		this->_flavorInfo(item->getFlavor());
	} else if (this->_closetChoice->getAttribute("number") == "3") {
		Weapon	*item = new Weapon(obj->getAttribute("Name"));
		name = item->getSprite();
		hud->setText(item->getDisplayName(), (ww - (ww / 7)) - (item->getDisplayName().length() / 2 * 6), 
				(wh / 3), Vector3(0.68, 0.68, 0.68), 1, "BigGamefont");
		this->_currentItemInfo->textInfo.push_back(item->getDisplayName());
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
	std::map<std::string, Elements *>::iterator		it2;

	theCamera.MoveTo(Vector3(Game::currentGame->getCurrentMap().getXMid(),
				Game::currentGame->getCurrentMap().getYMid(), 18.502), 1, true);
	for (it = this->_backCloset.begin(); it != this->_backCloset.end(); it++)
		theWorld.Remove(*it);
	for (it = this->_choices.begin(); it != this->_choices.end(); it++)
		theWorld.Remove(*it);
	for (it = this->_backChoices.begin(); it != this->_backChoices.end(); it++)
		theWorld.Remove(*it);
	for (it = this->_equipSelectionBack.begin(); it != this->_equipSelectionBack.end(); it++)
		theWorld.Remove(*it);
	for (it2 = this->_equipSelection.begin(); it2 != this->_equipSelection.end(); it2++)
		theWorld.Remove(it2->second);
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
}

void		MenuCharacter::_changeKitchen(void) {
	std::list<Elements *>::iterator		it;
	int									i;
	HUDWindow	*h = Game::getHUD();
	std::list<std::string>::iterator	it2;

	for (it2 = this->_description.begin(); it2 != this->_description.end(); it2++)
		h->removeText(*it2);
	if (this->_target != nullptr)
		theWorld.Remove(this->_target);
	this->_target = nullptr;
	this->_isBlock = 0;
	theWorld.Remove(this->_descriptionBackground);
	theWorld.Remove(this->_iconBackground);
	theWorld.Remove(this->_icon);
	theWorld.Remove(this->_lvlUp);
	theWorld.Remove(this->_levelsBackground);
	Game::getHUD()->removeText(std::to_string(this->_characLvl));
	Game::getHUD()->removeText(std::to_string(this->_characLvl * 4));
	Game::getHUD()->removeText(std::to_string(Quit::gold));
	this->_descriptionBackground = this->_iconBackground = this->_levelsBackground = this->_lvlUp = nullptr;
	this->_getSkills();
	this->_kitchen();
}

void		MenuCharacter::_kitchen(void) {
	Json::ValueIterator		it, it2;
	float						x, y, i;
	Elements				*tmp;

	std::cout << this->_character << std::endl;
	for (it = this->_skillTree.begin(), x = 125.75, i = 0; it != this->_skillTree.end(); it++, x += 3.75, i++) {
		for (it2 = (*it)[this->_character].begin(), y = -16.75; it2 != (*it)[this->_character].end(); it2++, y -= 3.5) {
			tmp = new Elements();
			if (i == 3)
				y -= 3.5;
			tmp->SetPosition(x, y); tmp->SetSprite("Resources/Images/Skills/" + it2->asString() + ".png"); tmp->SetSize(1.6);
			tmp->addAttribute("skill", it2->asString()); tmp->addAttribute("palier", std::to_string(i));
			tmp->SetLayer(100);
			theWorld.Add(tmp); this->_kitchenSkills.push_back(tmp);
			if (this->_characLvl < this->_skillsLvl[i]) {
				tmp = new Elements(); tmp->SetLayer(101);
				tmp->SetPosition(x, y); tmp->SetSprite("Resources/Images/kitchen_gray.png"); tmp->SetSize(3);
				theWorld.Add(tmp); this->_kitchenSkills.push_back(tmp);
			} else {
				this->_skillsChoices[i].push_back(tmp);
			}
		}
	}
}

void		MenuCharacter::_hideKitchen(int reload) {
	std::list<Elements *>::iterator		it;
	int									i;
	HUDWindow	*h = Game::getHUD();
	std::list<std::string>::iterator	it2;

	for (it2 = this->_description.begin(); it2 != this->_description.end(); it2++)
		h->removeText(*it2);
	if (this->_target != nullptr)
		theWorld.Remove(this->_target);
	this->_target = nullptr;
	if (!reload) {
		theCamera.MoveTo(Vector3(Game::currentGame->getCurrentMap().getXMid(),
			Game::currentGame->getCurrentMap().getYMid(), 18.502), 1, true);
		this->_currentTrigger = "skills";
		this->_showTextInfo("Press enter to choose your skills");
		this->_isBlock = 0;
	} else {
		for (it = this->_kitchenSkills.begin(); it != this->_kitchenSkills.end(); theWorld.Remove((*it)), it++);
		for (i = 0; i < this->_skillsChoices.size(); i++) {
			for (it = this->_skillsChoices[i].begin(); it != this->_skillsChoices[i].end(); theWorld.Remove(*it), it++);
			this->_skillsChoices[i].clear();
		}
		this->_skillsChoices.clear();
	}
	theWorld.Remove(this->_descriptionBackground);
	theWorld.Remove(this->_iconBackground);
	theWorld.Remove(this->_icon);
	theWorld.Remove(this->_lvlUp);
	theWorld.Remove(this->_levelsBackground);
	Game::getHUD()->removeText(std::to_string(this->_characLvl));
	Game::getHUD()->removeText(std::to_string(this->_characLvl * 4));
	Game::getHUD()->removeText(std::to_string(Quit::gold));
	this->_descriptionBackground = this->_iconBackground = this->_levelsBackground = this->_lvlUp = nullptr;
}

void		MenuCharacter::_levels(void) {

	if (this->_lvlUp == nullptr && this->_levelsBackground == nullptr) {
		Elements	*tmp = new Elements();

		tmp->SetPosition(144.3, -19.5); tmp->SetSprite("Resources/Images/kitchen_levels_background.png");
		tmp->SetSize(8, 10); theWorld.Add(tmp); this->_levelsBackground = tmp;
		tmp = new Elements();
		tmp->SetPosition(144.35, -18.6); tmp->SetSprite("Resources/Images/level_up.png");
		tmp->SetSize(3); theWorld.Add(tmp); this->_lvlUp = tmp;
	} else {
		Game::getHUD()->removeText(std::to_string(this->_characLvl));
		Game::getHUD()->removeText(std::to_string(this->_characLvl * 4));
		Game::getHUD()->removeText(std::to_string(Quit::gold));
		Quit::gold -= this->_characLvl * 4;
		this->_characLvl++;
	}

	Game::getHUD()->setText(std::to_string(this->_characLvl), 866 - std::to_string(this->_characLvl).size() * 3, 365, Vector3(0, 0, 0), 1, "BigGamefont");
	Game::getHUD()->setText(std::to_string(this->_characLvl * 4), 830, 490, Vector3(246, 255, 0), 1, "BigGamefont");
	Game::getHUD()->setText(std::to_string(Quit::gold), 930, 490, Vector3(1, 1, 1), 1, "BigGamefont");
}

void		MenuCharacter::_makeSkillChoice(void) {
	HUDWindow	*h = Game::getHUD();
	std::list<std::string>::iterator	it;

	for (it = this->_description.begin(); it != this->_description.end(); it++)
		h->removeText(*it);

	if (this->_target != nullptr)
		theWorld.Remove(this->_target);
	if (this->_choicePointer) {
		this->_target = new Actor();
		this->_target->SetSize(3); this->_target->SetSprite("Resources/Images/kitchen_target.png");
		this->_target->SetPosition(this->_choicePointer->GetPosition().X, this->_choicePointer->GetPosition().Y);
		theWorld.Add(this->_target);

	if (this->_descriptionBackground == nullptr) {
		this->_descriptionBackground = new Elements();
		this->_descriptionBackground->SetPosition(118, -19.5); this->_descriptionBackground->SetSize(8, 10);
		this->_descriptionBackground->SetSprite("Resources/Images/kitchen_skills_background.png");
		theWorld.Add(this->_descriptionBackground);
	}
	if (this->_iconBackground == nullptr) {
		this->_iconBackground = new Elements();
		this->_iconBackground->SetPosition(118, -17.5); this->_iconBackground->SetSize(3);
		this->_iconBackground->SetSprite("Resources/HUD/spell.png");
		theWorld.Add(this->_iconBackground);
	}
	if (this->_icon != nullptr)
		theWorld.Remove(this->_icon);
		this->_icon = new Elements(); this->_icon->SetPosition(118, -17.5); this->_icon->SetSize(2);
		this->_icon->SetSprite("Resources/Images/Skills/" + this->_choicePointer->getAttribute("skill") + ".png");
		theWorld.Add(this->_icon);
		h->setText(this->_skills[this->_choicePointer->getAttribute("skill")].get("displayName", "NAME").asString(), 
				110, 240, Vector3(0, 0, 0), 1);
		this->_description.push_back(this->_skills[this->_choicePointer->getAttribute("skill")].get("displayName", "NAME").asString());
		std::string tmp = this->_skills[this->_choicePointer->getAttribute("skill")].get("description", "DEFAULT").asString();
		int			x, y, Y;

		for (x = y = 0, Y = 370; x < tmp.size(); x++) {
			if (tmp[x] == '\n') {
				h->setText(tmp.substr(y, (x - y)), 50, Y, Vector3(0, 0, 0), 1);
				this->_description.push_back(tmp.substr(y, (x - y)));
				y = ++x;
				Y += 15;
			}
		}
		h->setText(tmp.substr(y, (x - y)), 50, Y, Vector3(0, 0, 0), 1);
		this->_description.push_back(tmp.substr(y, (x - y)));
	}
}

void		MenuCharacter::_getSkills(void) {
	std::ifstream		moves("Resources/Elements/SpecialMoves.json"), attacks("Resources/Elements/SpecialAttack.json"),
		tree("Resources/Elements/Skills.json");
	std::string			moveFile, attackFile, treeFile, tmp;
	Json::Reader		reader;
	Json::Value			jsonTmp;
	Json::ValueIterator		it;

	if (moves.is_open() && attacks.is_open() && tree.is_open()) {
		for (; std::getline(moves, tmp); )
			moveFile += tmp;
		for (; std::getline(attacks, tmp); )
			attackFile += tmp;
		for (; std::getline(tree, tmp); )
			treeFile += tmp;
	} else {
		Log::error("Cannot find the Special Moves / Attacks file.");
	}
	reader.parse(moveFile, jsonTmp, false);
	for (it = jsonTmp.begin(); it != jsonTmp.end(); it++) {
		this->_skills[it.key().asString()] = *it;
	}
	reader.parse(attackFile, jsonTmp, false);
	for (it = jsonTmp.begin(); it != jsonTmp.end(); it++)
		this->_skills[it.key().asString()] = (*it);
	reader.parse(treeFile, this->_skillTree, false);
}

std::string		MenuCharacter::getHeroType(void) { return this->_character; };
Weapon			*MenuCharacter::getWeapon(void) { return new Weapon(this->_equipSelection["Weapon"]->getAttribute("Name")); };
Ring			*MenuCharacter::getRing(void) { return new Ring(this->_equipSelection["ring"]->getAttribute("Name")); };
Armor			*MenuCharacter::getArmor(void) { return new Armor(this->_equipSelection["Armor"]->getAttribute("Name")); };
std::vector<std::string>	MenuCharacter::getSkills(void) { return this->_finalSkillChoices; };
int							MenuCharacter::getLevel(void) { return this->_characLvl; };
