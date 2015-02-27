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
 * File: ContactFilter.hpp
 * Creation: 2015-02-23 12:40
 * Vincent Rey <vrey@student.42.fr>
 */



#include "../inc/ContactFilter.hpp"


bool	ContactFilter::ShouldCollide(b2Fixture* fixA, b2Fixture* fixB) {

	std::string attrA = static_cast<Elements*>(fixA->GetBody()->GetUserData())->getAttributes()["type"];
	std::string attrB = static_cast<Elements*>(fixB->GetBody()->GetUserData())->getAttributes()["type"];

	if ((attrA == "Hero" || attrB == "Hero") && ((attrA == "heroWeapon" || attrB == "heroWeapon") || (attrA == "heroProjectile" || attrB == "heroProjectile"))) {
		// std::cout << "hello" << std::endl;
		return false;
	}
	else return true;
}
