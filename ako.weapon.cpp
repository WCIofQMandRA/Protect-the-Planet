//ako.weapon.cpp

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

namespace kernel
{
std::vector<weapon0_t> ako_weapon;

void get_ako_weapon()
{
	ako_weapon.push_back({5,18,1,0,false,[](intmp_t &x,const intmp_t &,double power_rate_pill,double power_rate_meteorite/*or power_rate_box*/)
						  {
							  x-=static_cast<intmp_t>(3*power_rate_pill*power_rate_meteorite);
						  },2e6});
}
}
