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
 * Creation: 2015-09-25 14:25
 * Vincent Rey <vrey@student.42.fr>
 */


#ifndef __Boss__
# define __Boss__

# include "Characters.hpp"

class Boss : public Characters {
public:
	Boss();
	Boss(std::string);
	~Boss();

	void			BeginContact(Elements* m, b2Contact* contact);
	void			EndContact(Elements *m, b2Contact *contact);

protected:
};

#endif
