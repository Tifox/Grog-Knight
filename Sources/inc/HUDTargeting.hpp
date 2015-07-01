
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
* File: HUDTargeting.hpp
* Creation: 2015-06-30 14:20
* Matthieu Maudet <arkhane84@gmail.com>
*/

#ifndef __HUDTargeting__
# define __HUDTargeting__

# include "../../Angel/Angel.h"
# include <list>
# include "Elements.hpp"

class Enemy;

class HUDTargeting : public Elements{

    public:

        HUDTargeting(void);
        ~HUDTargeting(void);

        void changeTarget(void);

    private:

        std::list<Enemy *>  _enemies;
        int                 _enemyId;
        Enemy*              _current;
        b2DistanceJoint*    _joint;

};

#endif
