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
#include <random>

namespace kernel
{
std::vector<meteorite0_t> ako_meteorite;
extern std::mt19937_64 rand64;

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
	ako_meteorite.push_back({std::make_pair(1380,1500),4,100,6.8e6,
							[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
							{
								health-=static_cast<intmp_t>(1000*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
							}});
	ako_meteorite.push_back({std::make_pair(150,170),5,10,3e6,
							[](intmp_t &health,const double &,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
							{
								health-=static_cast<intmp_t>(30*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
							}});
	ako_meteorite.push_back({std::make_pair(1800,1900),6,1000,9e6,
							[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
							{
								health=static_cast<intmp_t>(exp(log(static_cast<floatmp_t>(health))-log(static_cast<floatmp_t>(3))*complete_rate*hurt_rate_planet*hurt_rate_meteorite*(is_neg?-1:1)));
							}});
	ako_meteorite.push_back({std::make_pair(1800,1900),7,intmp_t("1208925819614629174706176"),8.5e6,
							 [](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
							 {
								 double tmp=complete_rate*hurt_rate_planet*hurt_rate_meteorite;
								 if(!is_neg)
								 {
									 if(tmp>0.7)
										health=intmp_t(log(floatmp_t(health))/log(2));
									 else if(tmp>0.3)
										health=intmp_t(sqrt(floatmp_t(health)));
									 else
										health/=2;
								 }
							 }});
	ako_meteorite.push_back({std::make_pair(500,600),8,30,4e6,
							[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
							 {
								 health-=static_cast<intmp_t>(500*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
							 }});
	ako_meteorite.push_back({std::make_pair(900,1000),9,70,5e6,
							 [](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
							  {
								  health=intmp_t(exp(log(floatmp_t(health))-log(floatmp_t(10))*complete_rate*hurt_rate_planet*hurt_rate_meteorite*(is_neg?-1:1)));
							  }});
	ako_meteorite.push_back({std::make_pair(1300,1400),10,5000,6e6,
							 [](intmp_t &health,const double &,bool is_neg,const double &,const double &)
							  {
								 if(is_neg)
								 {
									 auto s=health.str();std::shuffle(s.begin(),s.end(),rand64);health=intmp_t(s);
								 }
								 else
								 {
									  auto s=health.str();std::shuffle(s.begin(),s.end(),rand64);health=intmp_t(s.substr(rand64()%2?1:0));
								 }
							  }});
	ako_meteorite.push_back({std::make_pair(1700,1800),11,100,7e6,
							 [](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
							  {
								  health-=static_cast<intmp_t>(10/complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
							  }});
}
}
