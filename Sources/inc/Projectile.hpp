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
 * File: Projectile.hpp
 * Creation: 2015-02-23 15:14
 * Matthieu Maudet <mmaudet@student.42.fr>
 */

#ifndef __Projectile__
# define __Projectile__

# include "Elements.hpp"

class Characters;

class Projectile: public Elements {
public:
	Projectile(float x, float y, int direction, std::string owner);
	Projectile(Weapon *w, Characters *c);
	Projectile(Weapon *w, int dmg);
	Projectile(std::string img, int dmg, Vector2 pos, Vector2 force, Vector2 init, std::string name = "projectile", int rotat = -1, float isGravity = 0);
	~Projectile();

	std::string     getName(void);
	std::string     getFlavor(void);
	std::string     getAttack(void);
    float           getActive(void);
    int             getSize(void);
    int             getDamage(void);
    int             getPushback(void);
    float           getRecovery(void);
    int             attackReady(void);
	int				getCritRate(void);

	void	WaveContact(Elements *m, b2Contact *contact);
	void	BeginContact(Elements *elem, b2Contact *contact);
	void	EndContact(Elements *elem, b2Contact *contact);
	void	ReceiveMessage(Message *m);
private:

	void		_initDirection(Weapon* w, Characters* c);

	std::string _name;
	std::string _flavor;
	std::string _attack;
	float		_active;
	int			_size;
	int			_damage;
	int			_pushback;
	float		_recovery;
	int			_critRate;
	bool		_toDestroy;

};
#endif
