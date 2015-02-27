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
 * File: Log.cpp
 * Creation: 2015-02-27 04:51
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

# include "Log.hpp"

/**
 * Base constructor
 */
Log::Log(void) {
	return ;
}

/**
 * Base destructor
 */
Log::~Log(void) {
	return ;
}

/**
 * Log INFO level
 * @param: str (std::string)
 */
void		Log::info(std::string str) {
	std::cout << "\033[1;34m[INFO]\033[0m " << str << std::endl;
}

/**
 * Log WARNING level
 * @param: str (std::string)
 */
void		Log::warning(std::string str) {
	std::cout << "\033[1;33m[WARNING]\033[0m " << str << std::endl;
}

/**
 * Log ERROR log
 * @param: str (std::string)
 */
void		Log::error(std::string str) {
	std::cout << "\033[1;31m[ERROR]\033[0m " << str << std::endl;
	exit(1);
}
