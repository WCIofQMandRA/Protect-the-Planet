//ako.meteorite.cpp

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
std::vector<meteorite0_t> ako_meteorite;

void get_ako_memeorite()
{
	ako_meteorite.push_back({std::make_pair(400,500),0,5,5e6,
							[](intmp_t &health,const double &,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
							{
								 health-=static_cast<intmp_t>(100*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
							}});
	ako_meteorite.push_back({std::make_pair(400,500),1,10,5e6,
							[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
							{
								health-=static_cast<intmp_t>(150*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
							}});
	ako_meteorite.push_back({std::make_pair(200,250),2,8,4e6,
							[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
							{
								health-=static_cast<intmp_t>(80*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
							}});
	ako_meteorite.push_back({std::make_pair(200,250),3,3,5e6,
							[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
							{
								health-=static_cast<intmp_t>(150*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
							}});
	ako_meteorite.push_back({std::make_pair(1380,1500),4,30,6.8e6,
							[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
							{
								health-=static_cast<intmp_t>(1000*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
							}});
	ako_meteorite.push_back({std::make_pair(150,170),5,10,3e6,
							[](intmp_t &health,const double &,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
							{
								health-=static_cast<intmp_t>(30*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
							}});
	ako_meteorite.push_back({std::make_pair(2000,2130),6,1000,9e6,
							[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
							{
								health=static_cast<intmp_t>(exp(log(static_cast<floatmp_t>(health))-log(static_cast<floatmp_t>(1.2))*complete_rate*hurt_rate_planet*hurt_rate_meteorite*(is_neg?-1:1)));
							}});
}
}
