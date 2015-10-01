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
 * File: Map.hpp
 * Creation: 2015-03-02 19:10
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "Map.hpp"

//! Basic constructor
Map::Map(void) : _mapCount(0), _isUsed(0) {
	return ;
}

//! Main constructor
/**
 * @param name The name of the map
 */
Map::Map(std::string name) : _name(name), _mapCount(0), _isUsed(0) {
	return ;
}

//! Basic destructor
Map::~Map(void) {
	return ;
}

/* SETTERS */

void	Map::setHeight(int h) { this->_height = h; };
void	Map::setWidth(int w) { this->_width = w; };
void	Map::setXMid(int h) { this->_midX = h; };
void	Map::setYMid(int w) { this->_midY = w; };
void	Map::setImage(std::string n) { this->_image = n; };
void	Map::setTileHeight(int h) { this->_tileHeight = h; };
void	Map::setTileWidth(int w) { this->_tileWidth = w; };
void	Map::setImageHeight(int h) { this->_imageHeight = h; };
void	Map::setImageWidth(int w) { this->_imageWidth = w; };
void	Map::setProperties(std::map<int, std::map<std::string, Json::Value> > p) { this->_properties = p; };
void	Map::addElement(Elements * e) { ; };
void	Map::addMapElement(int n) { ; };
void	Map::setLayer(int n) {this->_layer = n; };
void	Map::setMap(std::vector<int> map) { this->_map.push_back(map); };
void	Map::setXStart(int x) { this->_xStart = x; };
void	Map::setYStart(int y) { this->_yStart = y; };
void	Map::setUsed(int n) { this->_isUsed = n; };

/* GETTERS */

int		Map::getHeight(void) { return this->_height; };
int		Map::getWidth(void) { return this->_width; };
int		Map::getXMid(void) { return (this->_xStart + (this->_width / 2)); };
int		Map::getYMid(void) { return this->_yStart - (this->_height / 2); };
int		Map::getXStart(void) { return this->_xStart; };
int		Map::getYStart(void) { return this->_yStart; };
int		Map::getIsUsed(void) { return this->_isUsed; };
std::list<Enemy *>	Map::getEnemies(void) { return this->_enemies; };
std::vector<std::vector<int> >	Map::getPhysicMap(void) { return this->_physicMap; };
std::vector<std::vector<Elements*> >	Map::getObjectMap(void) { return this->_objectMap; };

//! Display the map
/**
 * Iterate and display the element of a map.
 * Lot's of stuff in this function, but the code's pretty clear, see source for more information
 */
Map		Map::display(void) {
	float						x, y;
	std::list<std::vector <int> >::iterator		layers;
	std::vector<int>::iterator	it;
	Elements					*elem;
	int							v = 0;

	LevelGenerator				*lg;

	if (Game::currentGame != nullptr)
		lg = Game::currentGame->levelGenerator;
	else
		lg = nullptr;
	if (lg != nullptr) {
		std::cout << lg->getSpecialRoom(Game::currentY, Game::currentX) << std::endl;
	}
	// Allocation for the _physicMap
	if (!this->_isUsed) {
		for (v = 0; v < this->_height; v++) {
			this->_physicMap.push_back(std::vector<int>(this->getWidth()));
			this->_objectMap.push_back(std::vector<Elements*>(this->getWidth()));
		}
	}
	for (layers = this->_map.begin(), v = 0; layers != this->_map.end(); layers++, v++) {
		x = this->_xStart;
		y = this->_yStart;
		Enemy	*tmp;

		for (it = (*layers).begin(); it != (*layers).end(); it++, x++) {
			if ((x - this->_xStart) >= this->_width) {
				x = this->_xStart;
				y--;
			}
			if (*(it) != 0) {
				elem = new Elements();
				elem->removeAttr("physic");
				elem->addAttribute("image", this->_image);
				elem->setFrame(*(it));
				elem->setXStart(x);
				elem->setYStart(y);
				elem->setCutWidth(this->_tileWidth);
				elem->setCutHeight(this->_tileHeight);
				elem->setWidth(this->_imageWidth);
				elem->setHeight(this->_imageHeight);
				elem->addAttribute("type", "ground");
				elem->addAttribute("spriteMap", "TRUE");
				elem->SetLayer(v);
				if (this->_properties.find(*it) == this->_properties.end()) {
					elem->addAttribute("physic", "TRUE");
					this->_physicMap[-(y - this->_yStart)][x - this->_xStart] = 1;
					this->_objectMap[-(y - this->_yStart)][x - this->_xStart] = elem;
				} else {
					std::map<std::string, Json::Value>::iterator	it2;
					int												isPhysic = 1, isAnimated = 0;

					for (it2 = this->_properties.find(*it)->second.begin();
							it2 != this->_properties.find(*it)->second.end(); it2++) {
						if (it2->first == "physic") {
							isPhysic = 0;
						} else if (it2->first == "hitbox") {
							elem->setHitbox(it2->second.asString());
						} else if (it2->first == "animate") {
							isAnimated = 1;
							elem->addAttribute(it2->first, it2->second.asString());
						} else {
							elem->addAttribute(it2->first, it2->second.asString());
						}
					}
					if (isPhysic == 1) {
						elem->addAttribute("physic", "TRUE");
						this->_objectMap[-(y - this->_yStart)][x - this->_xStart] = elem;
						this->_physicMap[-(y - this->_yStart)][x - this->_xStart] = 1;
					} else {
						this->_objectMap[-(y - this->_yStart)][x - this->_xStart] = nullptr;
						this->_physicMap[-(y - this->_yStart)][x - this->_xStart] = 0;
					}
					if (isAnimated == 1) {
						int		v = atoi(elem->getAttribute("next").c_str()), next, count;
						float	time;

						elem->addAnimation(v, std::atof(elem->getAttribute("time").c_str()));
						v++;
						for (count = 0; v != *(it); count++) {
							next = time = -1;
							if (count >= 30) {
								Log::error("Seems like the frame " + std::to_string(*it) + 
									" does not have an animation loop end...");
							}
								for (it2 = this->_properties.find(v)->second.begin();
										it2 != this->_properties.find(v)->second.end(); it2++) {
									if (it2->first == "time")
										time = std::atof(it2->second.asString().c_str());
									if (it2->first == "next") {
										next = atoi(it2->second.asString().c_str());
									}
								}
								if (time == -1 || next == -1)
									Log::warning("An animated element ("+ std::to_string(v) +") is missing the params {next, time}");
							   else
									elem->addAnimation(next, time);
								v = next + 1;
						}
					}
				}
				if (elem->getAttribute("spawnEnemy") != "") {
					if (!this->_isUsed) {
						int level = Game::World;
						if (elem->getAttribute("enemyLevel") == "+1") {
							level++;
						}
						else if (elem->getAttribute("enemyLevel") == "-1")
							level--;
						if (level < 0)
							level = 0;
						if (elem->getAttribute("isFlying") != "")
							tmp = new Enemy(Game::eList->getEnemyRandom(level, "true"));
						else
							tmp = new Enemy(Game::eList->getEnemyRandom(level, "false"));
						PassivePattern	*p = new PassivePattern();
						tmp->setXStart(x);
						tmp->setYStart(y);
						tmp->setMap(this);
						p->setEnemy(tmp);
						tmp->setPattern(p);
						tmp->init();
						tmp->GetBody()->ApplyLinearImpulse(b2Vec2(0, 3), b2Vec2(0, 0));
						this->_enemies.push_back(tmp);
					} else {
						std::list<Enemy *>::iterator		en;
						for (en = this->_enemies.begin(); en != this->_enemies.end(); en++) {
							if (!(*en)->dead()) {
								(*en)->GetBody()->SetActive(true);
								(*en)->GetBody()->SetTransform(b2Vec2((*en)->getXStart(), (*en)->getYStart()),
									(*en)->GetBody()->GetAngle());
							}
						}
					}
				} else if (elem->getAttribute("spawnShop") != "") {
					// Check if shop spawn
					if (Game::currentGame->getShopkeeper() == nullptr) {
						Shopkeeper	*shop;
						Game::spawnShop = Vector2(x, y);
						shop = new Shopkeeper("Shopkeeper");
						shop->spawn();
						Game::currentGame->setShopkeeper(shop);
					}
				} else if (elem->getAttribute("spawnDealer") != "") {
					// Check if dealer is already spawn
					// Tag map as Dealer host
					// Pause Dealer Object in quit
					if (Game::dealer == nullptr) {
						Game::spawnDealer = Vector2(x, y);
						Game::dealer = new Dealer("Dealer");
						Game::dealer->spawn();
					}
				} else if (elem->getAttribute("spawnChest") != "") {
					// Check if chest is already spawn
					// Tag map as Chest host
					// Pause Chest Object in quit
					if (Game::chest->isSpawn == 0) {
						Game::spawnChest = Vector2(x, y);
						Game::chest->spawn();
					}
				} else if (elem->getAttribute("spawnDoor") != "") {
					if (elem->getAttribute("doorType") == "boss") {
						if (Game::bossDoor == nullptr) {
							Game::spawnBossDoor = Vector2(x, y);
							Game::bossDoor = new Door("Boss");
						}
					} else if (elem->getAttribute("doorType") == "secret") {
						if (Game::secretDoor == nullptr) {
							Game::spawnSecretDoor = Vector2(x, y);
							Game::secretDoor = new Door("Secret");
						}
						if (elem->getAttribute("speType") == "return") {
							Game::spawnSecretReturnDoor = Vector2(x, y);
							Game::secretReturnDoor = new Door("SecretReturn");
						}
					}
				}
				elem->display();
				this->_elemOfTheMap.push_back(elem);
			}
		}
	}
	this->_isUsed = 1;
	return *this;
}

void	Map::destroyMap(void) {
	std::list<Elements *>::iterator		it;
	std::list<Enemy *>::iterator		en;

	// Remove physics and remove the element
	for (it = this->_elemOfTheMap.begin(); it != this->_elemOfTheMap.end(); it++) {
		if ((*it)->getAttribute("physic") != "") {
			(*it)->GetBody()->SetActive(false);
		}
		theWorld.Remove(*it);
	}

	// Pause enemies
	for (en = this->_enemies.begin(); en != this->_enemies.end(); en++) {
		if (!(*en)->dead()) {
			Game::stopRunning((*en));
			(*en)->GetBody()->SetActive(false);
		}
	}
}

void	Map::callAllPatterns(void) {
	std::list<Enemy *>::iterator		en;

	for (en = this->_enemies.begin(); en != this->_enemies.end(); en++) {
		if (!(*en)->dead())
			(*en)->getPattern()->tick(*this);
	}
}

void	Map::removeEnemy(Enemy *e) {
	this->_enemies.remove(e);
}
