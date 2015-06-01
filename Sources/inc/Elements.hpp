
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
# include <list>
# include "../../Angel/Angel.h"

 
class Weapon;

class Elements : public PhysicsActor {
public:
	friend class Game;
	Elements();
	Elements(int id);
	Elements(Elements & obj);
	~Elements();

	void		setXStart(float X);
	void		setYStart(float Y);
	void		setWidth(int w);
	void		setHeight(int h);
	void		setCutWidth(int w);
	void		setCutHeight(int h);
	void		setFrame(int n);
	void 		addAttribute(std::string name, std::string value);
	std::string	getAttribute(std::string name);
	void		removeAttr(std::string name);
	void		display(void);
	std::map<std::string, std::string>		getAttributes(void);
	void		setFrameSprite(int frame);
	void		setHitbox(std::string);
	void		addAnimation(int frame, float time);

	/* Virtual function, overridden in Childs */
	virtual void	callback(Elements * elem) { };
	virtual void	BeginContact(Elements * elem, b2Contact *contact) { };
	virtual void	EndContact(Elements * elem, b2Contact *contact) { };
	virtual void	AnimCallback(String s);

	int				getOrientationX(void);
	int				getOrientationY(void);
	int				getLateralOrientation(void);
	bool			isDead(void);
	int				isAdded(void);


protected:
	virtual void						_run() {};
	std::string							_hitboxType;
	std::string							_hitbox;
	bool								_isDead;

private:
	class		Animation {
		public:
			int		frame;
			float	time;
	};

	float								_XStartPos;
	float								_YStartPos;
	std::map<std::string, std::string>				_attributes;
	int								_height;
	int								_width;
	int								_cutHeight;
	int								_cutWidth;
	int								_isAdded;
	int								_frame;
	std::list<Animation *>			_animationList;
	std::list<Animation *>::iterator _animIt;

};

# include "Characters.hpp"
# include "Maps.hpp"
# include "Game.hpp"

#endif
