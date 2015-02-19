
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
 * File: Elements.hpp
 * Creation: 2015-02-13 07:39
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

#ifndef __Elements__
# define __Elements__
# include <map>
# include <string>
# ifndef __Maps__
#  include "../inc/Game.hpp"
# endif
# include "Maps.hpp"
# include "../../Angel/Angel.h"

class Weapon;

class Elements : public PhysicsActor {
public:
	Elements();
	Elements(int id);
	Elements(Elements & obj);
	~Elements();

	void 		addAttribute(std::string name, std::string value);
	std::string	getAttribute(std::string name);
	void		setXStart(float X);
	void		setYStart(float Y);
	void		display(void);
	std::map<std::string, std::string>		getAttributes(void);

	/* Virtual function, overridden in Childs */
	virtual void	callback(Elements * elem) { };
	virtual void	BeginContact(Elements * elem, b2Contact *contact) { };
	virtual void	EndContact(Elements * elem, b2Contact *contact) { };

	int				getOrientationX(void);
	int				getOrientationY(void);

private:
	float								_XStartPos;
	float								_YStartPos;
	std::map<std::string, std::string>	_attributes;

protected:
	int									_jumping;
	int									_canMove;
	int									_invincibility;
	int									_meleeAttack;
	int									_rangedAttack;
	int									_smashing;
	int									_canSmash;
	int									_orientation;
	int									_up;
	int									_isJump;
	Weapon*								_weapon;
	std::list<Elements*>				_grounds;
	std::list<Elements*> 				_walls;
};
#endif
