//ako.box.cpp

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
#include "kernel.init.hpp"

//补给箱内容物的种类
#define CONTAIN_TYPE_WEAPON 1//武器
#define CONTAIN_TYPE_FOOD 2//食物
#define CONTAIN_TYPE_PILL 3//子弹
#define CONTAIN_TYPE_EFFECT 4//效果

namespace kernel
{
std::vector<box0_t> ako_box;

void get_ako_box()
{
	ako_box.push_back({std::make_pair(200,300),std::make_pair(0,0),
					   {},
					   {{compress16(CONTAIN_TYPE_PILL,0),30}},
					  0,1,3.7e6});
	ako_box.push_back({std::make_pair(500,550),std::make_pair(17,19),
					   {compress16(CONTAIN_TYPE_FOOD,0),compress16(CONTAIN_TYPE_PILL,0)},
					   {{compress16(CONTAIN_TYPE_EFFECT,0),1}},
					   1,15,2.3e6});
	ako_box.push_back({std::make_pair(50,100),std::make_pair(0,0),
					   {},
					   {{compress16(CONTAIN_TYPE_EFFECT,15),1}},
					  2,5,5.0e6});
	ako_box.push_back({std::make_pair(100,150),std::make_pair(15,20),
					   {compress16(CONTAIN_TYPE_EFFECT,8),compress16(CONTAIN_TYPE_EFFECT,9)},
					   {},
					  3,20,4.0e6});
	ako_box.push_back({std::make_pair(100,150),std::make_pair(40,50),
					   {compress16(CONTAIN_TYPE_FOOD,1),compress16(CONTAIN_TYPE_PILL,0)},
					   {{compress16(CONTAIN_TYPE_FOOD,0),5}},
					  4,20,5.0e6});
	ako_box.push_back({std::make_pair(200,200),std::make_pair(15,36),
					   {compress16(CONTAIN_TYPE_EFFECT,9),compress16(CONTAIN_TYPE_EFFECT,24),compress16(CONTAIN_TYPE_EFFECT,23)},
					   {},
					  5,10,5.0e6});
	ako_box.push_back({std::make_pair(100,200),std::make_pair(0,0),
					   {},
					   {{compress16(CONTAIN_TYPE_WEAPON,12),1}},
					  6,10,5.0e6});
	ako_box.push_back({std::make_pair(150,250),std::make_pair(40,50),
					   {compress16(CONTAIN_TYPE_EFFECT,19),compress16(CONTAIN_TYPE_EFFECT,2),compress16(CONTAIN_TYPE_EFFECT,28)},
					   {},
					  6,10,5.0e6});
}
}
