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

//用于绘图的陨石
struct meteoritep_t
{
	double x,y,size;
	uint16_t complete_rate;//完整度，取值范围1~3
	uint16_t type;
	std::vector<uint16_t> effects;
};

//陨石
struct meteorite_t
{
	orbit_t orbit;//轨道
	double theta;//当前极角
	intmp_t strength;//陨石的强度
	intmp_t strength_left;//陨石的剩余强度
	double size;//陨石的大小，决定碰撞箱和渲染
	//hurt(intmp &x,const double &complete_rate, bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
	//陨石击中完整程度为x的行星后，行星的剩余完整程度
	//complete_rate=(double)strength_left/strength
	std::function<void(intmp_t &,const double &,bool,const double &,const double &)> hurt;
	//效果编号，持续时间
	std::set<std::pair<uint16_t,uint64_t>> received_effect;
	received_effect_meteorite_t combined_effect;
	uint16_t type;//陨石的种类，与材质关联
	void to_p(meteoritep_t &)const;
};

//未生成的陨石
struct meteorite0_t
{
	std::pair<uint64_t,uint64_t> fly_time;//从生成到落到行星表面的时间范围（first~second）
	uint16_t type;
	intmp_t strength;
	double size;
	std::function<void(intmp_t &,const double &,bool,const double &,const double &)> hurt;
};
