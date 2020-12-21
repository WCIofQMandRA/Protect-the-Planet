//save_load.hpp: 配置和存档的读取和写入

//	-* mode: C++		encoding:UTF-8 *-
//	Copyright 2020 张子辰 & 吕航 (GitHub: WCIofQMandRA & LesterLv)
//
//	This file is part of the game 保卫行星
//
//	This game is free software; you can redistribute it and/or modify it
//	under the terms of the GNU Lesser General Public License as published by
//	the Free Software Foundation; either version 3, or (at your option) any
//	later version.
//
//	This game is distributed in the hope that it will be useful, but
//	WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
//	or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
//	License for more details.
//
//	You should have received copies of the GNU Lesser General Public License
//	and the GNU Gerneral Public License along with 保卫行星 .
//	If not, see https://www.gnu.org/licenses/.

#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H
#include "type.hpp"

class save_load
{
public:
	static std::vector<std::u32string> get_userlist();
	static void add_user();
	static void delete_user();
	static bool load(const std::u32string &name,uint16_t difficulty,uint64_t &level,uint64_t &counter,player_t&,planet_t&);
	static bool save(const std::u32string &name,uint16_t difficulty,uint64_t level,uint64_t counter,const player_t&,const planet_t&);
};
#endif //SAVE_LOAD_H
