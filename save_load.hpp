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

class save_load_class
{
private:
	//用户名，编号，上一次游戏时的难度
	std::map<std::u32string,std::pair<uint64_t,uint16_t>> user_list;
	uint64_t last_play_user;
	void save_user_list();
public:
	save_load_class();
	~save_load_class();
	save_load_class(const save_load_class&)=delete;
	save_load_class& operator=(const save_load_class&)=delete;
	//玩家名和玩家上次退出游戏时选择的难度
	std::vector<std::pair<std::u32string,uint16_t>> get_userlist();
	bool add_user(const std::u32string &name);
	bool delete_user(const std::u32string &name);
	bool load(const std::u32string &name,uint16_t difficulty,uint64_t &level,uint64_t &counter,uint64_t &score,player_t&,planet_t&,std::map<uint64_t,std::pair<boxd_t,uint64_t>>&);
	bool save(const std::u32string &name,uint16_t difficulty,uint64_t level,uint64_t counter,uint64_t score,const player_t&,const planet_t&,const std::map<uint64_t,std::pair<boxd_t,uint64_t>>&);
};

extern save_load_class save_load;
#endif //SAVE_LOAD_H
