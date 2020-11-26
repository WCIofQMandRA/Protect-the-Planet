//kernel/box.h: 只能被kernel.h包含

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

//补给箱
struct box_t
{
	std::pair<floatmp_t,floatmp_t> vec_v;//速度
	std::pair<floatmp_t,floatmp_t> vec_r;//位矢
	intmp_t strength;//陨石的强度
	intmp_t strength_left;//陨石的剩余强度
	floatmp_t size;//大小，决定碰撞箱和渲染
	//get<0>表示补给箱的内容物的种类：
	//get<1>储存内容物的具体信息（即一个物品在某类物品的编号）
	//get<2>储存内容物的数量
	std::vector<std::tuple<uint16_t,uint16_t,uint64_t>> contains;
	std::map<uint16_t,received_effect_box_t> received_effect;
	received_effect_box_t combined_effect;
};