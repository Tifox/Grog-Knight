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
 * File: Hitbox.hpp
 * Creation: 2015-02-27 04:45
 * Vincent Rey <vrey@student.42.fr>
 */

#ifndef __Hitbox__
# define __Hitbox__

# include "main.hpp"

class Hitbox {
public:
	Hitbox();
	~Hitbox();
	b2PolygonShape		getHitbox(std::string);
	int					checkExists(std::string);

private:
	b2PolygonShape	_getPolygon(std::string res);
	b2PolygonShape	_parseJson(std::string res);
	b2PolygonShape	_parseVertices(int v, std::vector<std::vector<int> >map);

	std::map<std::string, b2PolygonShape>		_hitboxes;
};

#endif
