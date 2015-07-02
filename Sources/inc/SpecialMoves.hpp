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

 #ifndef __SpecialMoves__
 # define __SpecialMoves__

 # ifdef __APPLE__
 #  include "../../Tools/jsoncpp/include/json/json.h"
 # else
 #  include "json/json.h"
 # endif

 #include "Characters.hpp"

 class Characters;

 class SpecialMoves {

     public:
         SpecialMoves(void);
         SpecialMoves(Characters*);
         ~SpecialMoves();

         Characters* character;

         void			_setCategory(std::string category);

         virtual void	_dash(void);
         virtual void	_charge(void);
         virtual void	_stomp(void);
         virtual void	_blink(void);
         virtual void	_fly(void);
         virtual void	_shunpo(void);

     private:
         std::string		_category;

 };

 #endif
