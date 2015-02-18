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
 * File: Hero.hpp
 * Creation: 2015-02-14 10:49
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

#ifndef __Hero__
# define __Hero__

# include "Elements.hpp"

# define RUN_SPEED 4
# define MAX_RUN_SPEED 10
# define MAX_JUMP 2
# define HERO_SIZE 1

class Hero : public Elements {
public:
	Hero();
	~Hero();

	virtual void	callback(Elements * elem);
	virtual void	EndContact(Elements * elem, b2Contact *contact);
	virtual void	BeginContact(Elements * elem, b2Contact *contact);
	virtual void	AnimCallback(String name);
	void			ReceiveMessage(Message *m);
	void			init();

private:
	int						_jumping;
	bool					_canMove;
	bool					_invincibility;
	bool					_meleeAttack;
	bool					_rangedAttack;
	bool					_smashing;
	bool					_canSmash;
	int						_orientation;
	int						_up;
	std::list<Elements*>	_grounds;
	std::list<Elements*>	_walls;
	int						_isJump;
};

#endif
