//types/meteorite.hpp: 只能被type.hpp包含

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
//陨石
struct meteorite_t
{
	orbit_t orbit;//轨道
	floatmp_t theta;//当前极角
	intmp_t strength;//陨石的强度
	intmp_t strength_left;//陨石的剩余强度
	floatmp_t size;//陨石的大小，决定碰撞箱和渲染
	//hurt(intmp &x,const floatmp_t &complete_rate, bool is_neg,const floatmp_t &hurt_rate_planet,const floatmp_t &hurt_rate_meteorite)
	//陨石击中完整程度为x的行星后，行星的剩余完整程度
	//complete_rate=(floatmp_t)strength_left/strength
	std::function<void(intmp_t &,const floatmp_t &,bool,const floatmp_t &,const floatmp_t &)> hurt;
	//intmp_t hurt;//陨石的总伤害，落到行星时的伤害为hurt*f(strength_left,strength)，f是一个待确定的函数
	std::map<uint16_t,received_effect_meteorite_t> received_effect;
	received_effect_meteorite_t combined_effect;
	uint16_t type;//陨石的种类，与材质关联
};

//未生成的陨石
struct meteorite0_t
{
	std::pair<uint64_t,uint64_t> fly_time;//从生成到落到行星表面的时间范围（first~second）
	uint16_t type;
	intmp_t strength;
	floatmp_t size;
	std::function<void(intmp_t &,const floatmp_t &,bool,const floatmp_t &,const floatmp_t &)> hurt;
};
