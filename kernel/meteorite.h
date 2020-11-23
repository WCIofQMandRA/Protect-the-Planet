//kernel/meteorite.h: 只能被kernel.h包含

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
//	along with 保卫行星 .
//	If not, see https://www.gnu.org/licenses/.

//陨石
struct meteorite_t
{
	std::pair<floatmp_t,floatmp_t> vec_v;//速度
	std::pair<floatmp_t,floatmp_t> vec_r;//位矢
	floatmp_t size;//陨石的大小，决定碰撞箱和渲染
	intmp_t strength;//陨石的强度
	intmp_t strength_left;//陨石的剩余强度
	intmp_t hurt;//陨石的总伤害，落到行星时的伤害为hurt*f(strength_left,strength)，f是一个待确定的函数
	uint16_t type;//陨石的种类，与材质关联
};