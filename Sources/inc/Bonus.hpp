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
 * File: Bonus.hpp
 * Creation: 2015-05-20 12:48
 * Manon Budin <mbudin@student.42.fr>
 */

 #include <iostream>

 class Bonus
 {
 public:

 	enum BonusType
 	{
 		NONE,
 		HP_BUFF,
 		MANA_BUFF,
 		SPEED_BUFF
 	};

 	Bonus(BonusType type, float amount);
 	~Bonus(void);

 	float getAmount();
 	BonusType getType();
 	static BonusType	parseType(std::string);

private:	
 	BonusType	_type;
 	float		_buffAmount;


 };