//types/box.hpp: 只能被type.hpp包含

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

#include "./orbit.hpp"
//补给箱
struct box_t
{
	orbit_t orbit;//轨道
	floatmp_t theta;//当前极角
	intmp_t strength;//补给箱的强度
	intmp_t strength_left;//补给箱的剩余强度
	floatmp_t size;//大小，决定碰撞箱和渲染
	//get<0>表示补给箱的内容物的种类：
	//get<1>储存内容物的具体信息（即一个物品在某类物品的编号）
	//get<2>储存内容物的数量
	std::vector<std::tuple<uint16_t,uint16_t,uint64_t>> contains;
	std::map<uint16_t,received_effect_box_t> received_effect;
	received_effect_box_t combined_effect;
	uint16_t type;
};

//未生成的补给箱
struct box0_t
{
	std::pair<uint64_t,uint64_t> fly_time;//从生成到落到行星表面的时间范围（first~second）
	std::pair<uint64_t,uint64_t> total_value;//内容物的总价值的范围
	std::vector<std::pair<uint16_t,uint16_t>> probal_contain;//可能含有的内容物（first:种类，second:物品编号）
	uint64_t type;
	intmp_t strength;
	floatmp_t size;
};
