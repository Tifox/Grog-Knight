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
	this->_text = str;
	Game::addHUDWindow(this);
}

void	HUDWindow::displayText(void) {
	glColor4f(1, 1, 1, 1);
	DrawGameText(this->_text, "Console", 150.0f, 50.0f, theCamera.GetRotation());
}

void	HUDWindow::addImage(std::string path, int x, int y) {
	HUDActor *tmp = new HUDActor();
	tmp->SetSprite(path);
	tmp->SetPosition(x, y);
	tmp->SetSize(50.0f);
	tmp->SetDrawShape(ADS_Square);
	tmp->SetLayer(100);
	theWorld.Add(tmp);
	std::cout << "Adding an element" << std::endl;
}
