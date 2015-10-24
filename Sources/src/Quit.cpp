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

/****
 *** * File: Quit.cpp
 * *** Creation: 2015-06-23 16:35
 *** * Ne02ptzero <louis@ne02ptzero.me>
 * *** lsolofri@e2r12p22.42.fr
 ****/

#include "Quit.hpp"

/**
 * Default constructor
 */
Quit::Quit(void) {
	return ;
}

/**
 * Destructor
 */
Quit::~Quit(void) {
	return ;
}

void	Quit::quitGame(void) {
	Menu		*menu = Game::currentGame->menu;
	Quit::writeBindings(menu->getBindings());
	Quit::doSave((Hero *)Game::currentGame->getHero());
	exit(0);
}

void	Quit::writeBindings(std::map<std::string, std::list<t_bind *> > binds) {
	std::map<std::string, std::list<t_bind *> >::iterator	it;
	std::list<t_bind *>::iterator							it2;
	Json::Value												root;
	std::ofstream jsonFile, luaFile;

	if (binds.size() != 0) {
		jsonFile.open("Config/Bindings.json", std::ofstream::out | std::ofstream::trunc);
		luaFile.open("Config/input_bindings.ini", std::ofstream::out | std::ofstream::trunc);
		if (!jsonFile.is_open() || !luaFile.is_open()) {
			Log::error("A binding file is missing (Bindings.json / input_bindings.ini)");
		}

		jsonFile << "/* This file is generated by Grog-Like. DO NOT modify it. */" << std::endl;
		luaFile << "; This file is generated by Grog-Like. DO NOT modify it." << std::endl;
		for (it = binds.begin(); it != binds.end(); it++) {
			luaFile << ";; " << it->first << std::endl;
			for (it2 = it->second.begin(); it2 != it->second.end(); it2++) {
				// Json file
				root[it->first][(*it2)->name]["broadcast"] = (*it2)->broadcast;
				root[it->first][(*it2)->name]["key"] = (*it2)->realKey;
				root[it->first][(*it2)->name]["controller"] = (*it2)->controller;

				// Lua file
				if (!Quit::isUpper((*it2)->broadcast)) {
					luaFile << "\t" << (*it2)->realKey << " = +" << (*it2)->broadcast << "Pressed" << std::endl;
					luaFile << "\t" << (*it2)->realKey << " = -" << (*it2)->broadcast << "Released" << std::endl;
				} else {
					luaFile << "\t" << (*it2)->realKey << " = " << (*it2)->broadcast << std::endl;
				}
			}
		}
		// Yeah yeah, nasty AF.
		luaFile << "\n;; Controller\n\n\tP1BUTTON_A = +buttonAPressed\n\tP1BUTTON_A = -buttonAReleased\n\tP1BUTTON_B = +buttonBPressed\n\tP1BUTTON_B = -buttonBReleased\n\tP1BUTTON_X = +buttonXPressed\n\tP1BUTTON_X = -buttonXReleased\n\tP1BUTTON_Y = +buttonYPressed\n\tP1BUTTON_Y = -buttonYReleased\n\tP1BUTTON_START = buttonSTARTPressed\n\tP1BUTTON_BACK = buttonBACKPressed\n\tP1BUTTON_RIGHTBUMPER = buttonRBPressed\n\tP1BUTTON_LEFTBUMPER = buttonLBPressed";
		jsonFile << root << std::endl;
	}
}

int		Quit::isUpper(std::string s) {
	int		i;

	for (i = 0; i < s.size(); i++) {
		if (s[i] >= 'A' && s[i] <= 'Z')
			return 1;
	}
	return 0;
}

void		Quit::doSave(Hero *h) {
	Json::Value		root, chest;
	std::ofstream	jsonFile;
	std::stringstream	string;
	std::map<int, std::string>		items;
	std::map<int, std::string>::iterator		it;

	if (Game::currentGame->getSave().size() == 0)
		return ;

	if (Game::chest != nullptr)
		items = Game::chest->getItems();
	else {
		root["chest"] = Game::currentGame->getSave()["chest"];
	}

	jsonFile.open(".save", std::ofstream::trunc);
	root["level"] = h->getLevel();
	//root["level"] = 15;
	root["key"] = KEY;

	if (Game::chest != nullptr) {
		for (it = items.begin(); it != items.end(); it++) {
			if (it->second != "")
				root["chest"][std::to_string(it->first)] = it->second;
		}
		root["chest"]["gold"] = Game::chest->getGold();
	}

	string << root << std::endl;
	jsonFile << base64_encode((unsigned char *)string.str().c_str(), string.str().length()) << std::endl;
}

std::map<std::string, Json::Value>		Quit::getSave(void) {
	std::ifstream		file(".save");
	std::string			fileContent, tmp;
	Json::Reader		reader;
	Json::Value			root;
	std::map<std::string, Json::Value>		result;

	if (file.is_open()) {
		for (; std::getline(file, tmp); )
			fileContent += tmp;
	} else {
		result["gold"] = 0;
		result["level"] = 1;
		return result;
	}
	reader.parse(base64_decode(fileContent), root, false);
	if (root["key"].asString() != KEY)
		Quit::cheater();
	result["chest"] = root["chest"];
	result["level"] = root["level"];
	Quit::level = root["level"].asInt();
	Quit::gold = root["chest"]["gold"].asInt();
	return result;
}

void		Quit::cheater(void) {
	std::cout << "\033[0;31m";
	std::cout << "   _____ _    _ ______       _______ ______ _____  \n\
  / ____| |  | |  ____|   /\\|__   __|  ____|  __ \\  \n\
 | |    | |__| | |__     /  \\  | |  | |__  | |__) | \n\
 | |    |  __  |  __|   / /\\ \\ | |  |  __| |  _  /  \n\
 | |____| |  | | |____ / ____ \\| |  | |____| | \\ \\  \n\
  \\_____|_|  |_|______/_/    \\_\\_|  |______|_|  \\_\\ \n\
 \n\
" << std::endl;
	std::cout << "\033[0m";
	_exit(0);
}

/* /!\ The following code is not ours.*/
/* Found here: http://www.adp-gmbh.ch/cpp/common/base64.html */
/**
 * Copyright (C) 2004-2008 René Nyffenegger
 * This source code is provided 'as-is', without any express or implied
 * warranty. In no event will the author be held liable for any damages
 * arising from the use of this software.
 *  René Nyffenegger rene.nyffenegger@adp-gmbh.ch
 */

static const std::string base64_chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
			ret += '=';

	}

	return ret;

}

std::string base64_decode(std::string const& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}

int		Quit::level = 0;
int		Quit::gold = 0;
