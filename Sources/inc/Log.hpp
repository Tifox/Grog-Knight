
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
 * File: Log.hpp
 * Creation: 2015-02-27 04:51
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

#ifndef __Log__
# define __Log__

# include <iostream>
# include <cstdlib>

class	Log {

	public:
		Log();
		~Log();

		static void		info(std::string name);
		static void		warning(std::string name);
		static void		error(std::string name);

};

#endif
