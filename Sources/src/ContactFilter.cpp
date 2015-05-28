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

	Elements* attrA;
	Elements* attrB;

	attrA = static_cast<Elements*>(fixB->GetBody()->GetUserData());
	attrB = static_cast<Elements*>(fixA->GetBody()->GetUserData());
	if (static_cast<Elements*>(fixA->GetBody()->GetUserData())->getAttribute("type") == "Hero") {
		Elements* attrA = static_cast<Elements*>(fixA->GetBody()->GetUserData());
		Elements* attrB = static_cast<Elements*>(fixB->GetBody()->GetUserData());
	}
	if (attrA->getAttribute("type") == "Object" && attrB->getAttribute("type") != "ground"
		|| attrB->getAttribute("type") == "Object" && attrA->getAttribute("type") != "ground") {
		if (attrA->getAttribute("type") == "Hero")
			attrA->BeginContact(attrB, nullptr);
		return false;
	}
	if (attrA->getAttribute("type") == "Hero" &&
		attrB->getAttribute("type") == "ground" &&
		attrB->getAttribute("speType") == "canCross") {
		std::cout << "fewfew" << std::endl;
		if (attrA->GetBody()->GetWorldCenter().y - 1 <
			attrB->GetBody()->GetWorldCenter().y) {
			return false;
		}
		else if (static_cast<Characters*>(attrA)->getOrientation() == Characters::DOWN) {
			return false;
		}
	}
	return true;
}
