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
* File: HUDTargeting.cpp
* Creation: 2015-06-30 14:20
* Matthieu Maudet <arkhane84@gmail.com>
*/

# include "HUDTargeting.hpp"

//! Constructor
/**
 * Basic constructor
 */

 HUDTargeting::HUDTargeting(void) {
    _enemies = Game::currentGame->maps->_XYMap[Game::currentY][Game::currentX].getEnemies();
    std::cout << _enemies.size() << std::endl;
    if (_enemies.size() > 0) {
        this->SetSize(0.5);
        this->SetName("TargetGizmo");
        this->SetDrawShape(ADS_Square);
        this->SetColor(1, 0, 0, 1);
        this->SetSprite("Resources/Images/HUD/cible.png");
        this->SetLayer(101);
        this->addAttribute("physic", "1");
        this->SetDensity(0.0001);
        this->SetFixedRotation(true);
        this->SetIsSensor(true);
        this->SetPosition(_enemies.front()->GetBody()->GetWorldCenter().x, _enemies.front()->GetBody()->GetWorldCenter().y);
        this->InitPhysics();

        b2DistanceJointDef jointDef;
    	jointDef.Initialize(_enemies.front()->GetBody(), this->GetBody(), b2Vec2(_enemies.front()->GetBody()->GetWorldCenter().x, _enemies.front()->GetBody()->GetWorldCenter().y),
    						 this->GetBody()->GetWorldCenter());
    	jointDef.collideConnected = false;
        b2DistanceJoint *joint = (b2DistanceJoint*)theWorld.GetPhysicsWorld().CreateJoint(&jointDef);

        theWorld.Add(this);
    }
 }

 //! Destructor
 /**
  * Basic destructor
  */
 HUDTargeting::~HUDTargeting(void) {
 	return;
 }
