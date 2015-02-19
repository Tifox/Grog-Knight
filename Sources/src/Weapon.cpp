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
 * File: Weapon.cpp
 * Creation: 2015-02-18 14:00
 * Vincent Rey <vrey@student.42.fr>
 */

# include "../inc/Weapon.hpp"


/*
** Default constructor, using the element that called the attack
** @param: Elements *
*/
Weapon::Weapon(Elements *m) {
	this->SetPosition(m->GetBody()->GetWorldCenter().x + m->getOrientationX(), m->GetBody()->GetWorldCenter().y + m->getOrientationY());
	this->SetSize(1);
	this->SetName("Weapon");
	this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	this->SetDensity(1.0f);
	this->SetFriction(1.0f);
	this->SetRestitution(0.0f);
	this->SetDrawShape(ADS_Square);
	this->SetFixedRotation(true);
	this->Tag("weapon");
	this->InitPhysics();
	this->GetBody()->SetLinearVelocity(m->GetBody()->GetLinearVelocity());
	theSwitchboard.DeferredBroadcast(new Message("DeleteWeapon"), 0.2f);
	theWorld.Add(this);
}

Weapon::~Weapon(void) {
	return;
}

void	Weapon::BeginContact(Elements *elem, b2Contact *contact) {
	if (elem->getAttributes()["type"] == "enemy") {
		std::cout << "Enemy hit" << std::endl;
	}
	else if (elem->getAttributes()["type"] == "wall" ||
			 elem->getAttributes()["type"] == "ground" ||
			 elem->getAttributes()["type"] == "corner") {
		std::cout << "Ground hit" << std::endl;
	}
	else if (elem->getAttributes()["type"] == "hero") {
		std::cout << "Hero hit" << std::endl;
		contact->SetEnabled(false);
	}
}

void	Weapon::EndContact(Elements *elem, b2Contact *contact) {
	std::cout << "EndContact with: " << elem->getAttributes()["type"] << std::endl;
}

