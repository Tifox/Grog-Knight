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
 * File: Dealer.hpp
 * Creation: 2015-08-07 15:14
 * Manon Budin <mbudin@student.42.fr> 
 */

#ifndef __Dealer__
# define __Dealer__

# include "Characters.hpp"

class Dealer : public Characters {

	public:
		Dealer(std::string);
		~Dealer();

		void	init();
		virtual void	BeginContact(Elements* m, b2Contact* contact);
		virtual void	EndContact(Elements* m, b2Contact* contact);
		void			ReceiveMessage(Message *m);

	private:
		std::string		_name;
		std::string		_talk;
		std::string		_drug;
		bool			_give;

};
#endif
