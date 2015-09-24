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
 * File: SpecialMoves.hpp
 * Creation: 2015-06-30 09:57
 * Matthieu Maudet<arkhane84@gmail.com>
 */

 #ifndef __SpecialAttack__
 # define __SpecialAttack__

 # ifdef __APPLE__
 #  include "../../Tools/jsoncpp/include/json/json.h"
 # else
 #  include "json/json.h"
 # endif

 #include "Characters.hpp"

 class Characters;

 class SpecialAttack : public MessageListener {

     public:
         SpecialAttack(void);
         SpecialAttack(Characters*);
         ~SpecialAttack(void);

         Characters* character;

         void			_setCategory(std::string category);
		 void			ReceiveMessage(Message *m);
         void           _whirlwind(void);
         void           _rapidFire(void);


     private:
         std::string		_category;
		int					_previousSpeed;
		int					_previousDmg;
		int					_previousResistance;
		std::string			_currentAttack;

 };

 #endif
