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
 * File: GameContactListener.cpp
 * Creation: 2015-02-18 16:00
 * Vincent Rey <vrey@student.42.fr>
 */

#include "../inc/GameContactListener.hpp"

void	GameContactListener::BeginContact(b2Contact * contact) {
	PhysicsActor *p1;
	PhysicsActor *p2;

	p1 = static_cast<PhysicsActor*>(contact->GetFixtureA()->GetBody()->GetUserData());
	p2 = static_cast<PhysicsActor*>(contact->GetFixtureB()->GetBody()->GetUserData());
	Game::elementMap[p1->getId()]->BeginContact(Game::elementMap[p2->getId()], contact);
	Game::elementMap[p2->getId()]->BeginContact(Game::elementMap[p1->getId()], contact);
}

void	GameContactListener::EndContact(b2Contact * contact) {
	PhysicsActor *p1;
	PhysicsActor *p2;

	p1 = static_cast<PhysicsActor*>(contact->GetFixtureA()->GetBody()->GetUserData());
	p2 = static_cast<PhysicsActor*>(contact->GetFixtureB()->GetBody()->GetUserData());
	Game::elementMap[p1->getId()]->EndContact(Game::elementMap[p2->getId()], contact);
	Game::elementMap[p2->getId()]->EndContact(Game::elementMap[p1->getId()], contact);
}
