
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
 * File: HUDWindow.hpp
 * Creation: 2015-03-14 05:35
 * Louis Solofrizzo <louis@ne02ptzero.me>
 */

#ifndef __HUDWindow__
# define __HUDWindow__

# include "../../Angel/Angel.h"
# include "Game.hpp"

class	HUDWindow : public HUDActor {
	public:
		HUDWindow(void);
		~HUDWindow(void);

		void	setText(std::string str, int x, int y);
		void	displayText(void);
		void	addImage(std::string p, int x, int y);

	private:
		std::string	_text;
};

#endif
