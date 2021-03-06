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
 * File: Boss.hpp
 * Creation: 2015-09-29 18:09
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

#ifndef __BOSS__
# define __BOSS__
class	Characters;
class	HUDWindow;
# include "Characters.hpp"
# include "Projectile.hpp"
# define ORIENT 0.1f

class	Boss : public Characters {
	public:
		Boss(std::string name, int x, int y);
		~Boss(void);

		virtual void	ReceiveMessage(Message *m);
		virtual void	AnimCallback(String s);
		virtual void	BeginContact(Elements *elem, b2Contact *contact);
		virtual void	EndContact(Elements *elem, b2Contact *contact);
		void			lifeBar(void);
		void			createProjectile(Vector2 force, Vector2 init);

	private:
		HUDWindow		*_h;
		std::list<HUDActor *>	_lifeList;
		int						_lastHitID;
		int						_inactive;
		float						_x;
		float						_y;
		int							_stade;
};

# include "HUDWindow.hpp"

#endif
