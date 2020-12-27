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
#include <random>

namespace kernel
{
std::vector<weapon0_t> ako_weapon;
extern std::mt19937_64 rand64;

void get_ako_weapon()
{
	ako_weapon.push_back({5,18,1,0,false,[](intmp_t &x,const intmp_t &,double power_rate_pill,double power_rate_meteorite/*or power_rate_box*/)
						  {
							  x-=static_cast<intmp_t>(3*power_rate_pill*power_rate_meteorite);
						  },2e6});
	ako_weapon.push_back({10,6,1,1,false,[](intmp_t &x,const intmp_t &,double power_rate_pill,double power_rate_meteorite/*or power_rate_box*/)
						  {
							  x-=static_cast<intmp_t>(3*power_rate_pill*power_rate_meteorite);
						  },4e6});
	ako_weapon.push_back({15,60,1,2,false,[](intmp_t &x,const intmp_t &,double power_rate_pill,double power_rate_meteorite/*or power_rate_box*/)
						  {
							  x-=static_cast<intmp_t>(50*power_rate_pill*power_rate_meteorite);
						  },1.7e6});
	ako_weapon.push_back({30,5,1,3,true,[](intmp_t &x,const intmp_t &,double power_rate_pill,double power_rate_meteorite/*or power_rate_box*/)
						  {
							  x-=static_cast<intmp_t>(1*power_rate_pill*power_rate_meteorite);
						  },nan("")});
	ako_weapon.push_back({50,5,3,4,true,[](intmp_t &x,const intmp_t &,double power_rate_pill,double power_rate_meteorite/*or power_rate_box*/)
						  {
							  x-=static_cast<intmp_t>(2*power_rate_pill*power_rate_meteorite);
						  },nan("")});
	ako_weapon.push_back({80,50,20,5,true,[](intmp_t &x,const intmp_t &,double power_rate_pill,double power_rate_meteorite/*or power_rate_box*/)
						  {
							  x-=static_cast<intmp_t>(100*power_rate_pill*power_rate_meteorite);
						  },nan("")});
	ako_weapon.push_back({80,0,20,6,true,[](intmp_t &x,const intmp_t &,double power_rate_pill,double power_rate_meteorite/*or power_rate_box*/)
						  {
							  x-=static_cast<intmp_t>(3*power_rate_pill*power_rate_meteorite);
						  },nan("")});
	ako_weapon.push_back({75,20,1,7,false,[](intmp_t &x,const intmp_t &,double power_rate_pill,double power_rate_meteorite/*or power_rate_box*/)
						  {
							  x=static_cast<intmp_t>(exp(log(floatmp_t(x))-log(floatmp_t(1.2))*power_rate_pill*power_rate_meteorite));
						  },2e6});
	ako_weapon.push_back({95,20,1,8,false,[](intmp_t &x,const intmp_t &,double power_rate_pill,double power_rate_meteorite/*or power_rate_box*/)
						  {
							  x=static_cast<intmp_t>(exp(log(floatmp_t(x))-log(floatmp_t(1.7))*power_rate_pill*power_rate_meteorite));
						  },2e6});
	ako_weapon.push_back({120,20,1,9,false,[](intmp_t &x,const intmp_t &,double power_rate_pill,double power_rate_meteorite/*or power_rate_box*/)
						  {
							  x=static_cast<intmp_t>(exp(log(floatmp_t(x))-log(floatmp_t(2))*power_rate_pill*power_rate_meteorite));
						  },2e6});
	ako_weapon.push_back({200,100,2,10,false,[](intmp_t &x,const intmp_t &,double power_rate_pill,double power_rate_meteorite/*or power_rate_box*/)
						  {
							  x=static_cast<intmp_t>(pow(floatmp_t(x),1/(2*power_rate_pill*power_rate_meteorite)));
						  },3e6});
	ako_weapon.push_back({400,300,10,11,false,[](intmp_t &x,const intmp_t &,double power_rate_pill,double power_rate_meteorite/*or power_rate_box*/)
						  {
							  x=static_cast<intmp_t>(log(floatmp_t(x))/(log(floatmp_t(2))*power_rate_pill*power_rate_meteorite));
						  },5e5});
	ako_weapon.push_back({100,10,1,12,false,[](intmp_t &x,const intmp_t &y,double ,double)
						  {
							  x*=x;
							  x/=y;
						  },2e6});
	ako_weapon.push_back({20,5,1,13,false,[](intmp_t &x,const intmp_t &,double,double)
						  {
							  auto tmp=x.str();
							  std::shuffle(tmp.begin(),tmp.end(),rand64);
							  x=intmp_t(tmp.substr(rand64()%10?0:1));
						  },1e7});
}
}
