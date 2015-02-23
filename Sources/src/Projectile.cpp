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
 * File: Projectile.cpp
 * Creation: 2015-02-23 15:140
 * Matthieu Maudet <mmaudet@student.42.fr>
 */

# include "../inc/Projectile.hpp"


/*
** Default constructor, using the element that called the attack
** @param: Elements *
*/
Projectile::Projectile(int x, int y) {
	this->SetPosition(x, y);
	this->SetSize(0.5f);
	this->SetName("Projectile");
	this->SetShapeType(PhysicsActor::SHAPETYPE_BOX);
	this->SetDensity(1);
	this->SetFriction(0);
	this->SetRestitution(0.0f);
	this->SetFixedRotation(true);
	this->Tag("projectile");
	this->InitPhysics();
	this->GetBody()->SetGravityScale(0.0f);
	this->ApplyLinearImpulse(Vector2(5, 0), Vector2(0, 0));
	theWorld.Add(this);
	theSwitchboard.DeferredBroadcast(new Message("DeleteProjectile"), 0.1f);
}

Projectile::~Projectile(void) {
	return;
}

void	Projectile::BeginContact(Elements *elem, b2Contact *contact) {
	std::cout << "TODO" << std::endl;
	// theWorld.GetPhysicsWorld().DestroyBody(this->GetBody());
	// theWorld.Remove(this);
}

void	Projectile::EndContact(Elements *elem, b2Contact *contact) {
	std::cout << "TODO" << std::endl;
}

