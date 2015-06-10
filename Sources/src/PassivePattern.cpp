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
 * File: PassivePattern.cpp
 * Creation: 2015-05-03 10:25
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "PassivePattern.hpp"

PassivePattern::PassivePattern(void) : CPattern("passive"), _tickNumber(0) {

}

PassivePattern::~PassivePattern(void) {
}

void	PassivePattern::tick(Map m) {
	CPattern::tick(m);

	if (this->_orientation == 1) {
		if (!m.getPhysicMap()[this->_y + 1][this->_x + 1]) {
			this->_orientation = 2;
			this->_enemy->_backward(1);
		} else if (!m.getPhysicMap()[this->_y][this->_x + 1])
			this->_enemy->_forward(1);
		else if (m.getPhysicMap()[this->_y][this->_x + 1]) {
			this->_orientation = 2;
			this->_enemy->_backward(1);
		}
	} else if (this->_orientation == 2) {
		if (!m.getPhysicMap()[this->_y +1][this->_x - 1]) {
			this->_orientation = 1;
			this->_enemy->_forward(1);
		} else if (!m.getPhysicMap()[this->_y][this->_x - 1])
			this->_enemy->_backward(1);
		else if (m.getPhysicMap()[this->_y][this->_x - 1]) {
			this->_orientation = 1;
			this->_enemy->_forward(1);
		}
	}
}
