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
 * File: Shooter.hpp
 * Creation: 2015-02-23 16:14
 * Matthieu Maudet <mmaudet@student.42.fr>
 */

#ifndef __Shooter__
# define __Shooter__

# include <iostream>
# include "Elements.hpp"

class Shooter : public Elements {
public:
	Shooter();
	~Shooter();

	void fire(float x, float y, int direction, std::string owner);

private:
	int _damage;
	float _rate;
	int _orientation;
	bool _straight;
	std::string _spriteName;	

};

#endif
