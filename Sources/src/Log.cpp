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

//! Base constructor
Log::Log(void) {
	return ;
}

//! Base destructor
Log::~Log(void) {
	return ;
}

//! Print an info log
/**
 * Log INFO level (Blue)
 * @param str The string to display
 */
void		Log::info(std::string str) {
	if (Log::isLog == 1)
		std::cout << "\033[1;34m[INFO]\033[0m " << str << std::endl;
}

//! Print a warning log
/**
 * Log WARNING level (Orange)
 * @param str The string to display
 */
void		Log::warning(std::string str) {
	if (Log::isLog == 1)
		std::cout << "\033[1;33m[WARNING]\033[0m " << str << std::endl;
}

//! Print an error log
/**
 * Log ERROR log (Red), then exit with 1 code.
 * @param str The error to display
 */
void		Log::error(std::string str) {
	if (Log::isLog == 1) {
		std::cout << "\033[1;31m[ERROR]\033[0m " << str << std::endl;
		exit(1);
	}
}

//! Set the debug prompt as false
/**
 * Disable the debug.
 * 1 = Debug, 0 = None
 * @param n 1 / 0
 */
void	Log::setLog(int n) {
	if (n != 0 && n != 1)
		Log::warning("The argument of setLog is invalid (0 / 1)");
	else
		Log::isLog = n;
	std::cout << "Setting Log at " << n << std::endl;
}

int Log::isLog = 1;
