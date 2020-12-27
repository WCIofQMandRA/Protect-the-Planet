//ako.effect.cpp

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

//未触发的效果在触发时的接受者
#define EFFECT_RECIVER_PLAYER 1//玩家
#define EFFECT_RECIVER_BOX 2//全体未落下的补给箱
#define EFFECT_RECIVER_CURRENT_WEAPON 3//玩家当前使用的武器
#define EFFECT_RECIVER_ALL_WEAPONS 4//玩家持有的所有武器
#define EFFECT_RECIVER_METE 5//全体陨石
#define EFFECT_RECIVER_PLANET 6//行星

namespace kernel
{
std::vector<effect_t> ako_effect;
std::vector<received_effect_box_t> ako_box_effect;
std::vector<received_effect_meteorite_t> ako_meteorite_effect;
std::vector<received_effect_planet_t> ako_planet_effect;
std::vector<received_effect_weapon_t> ako_weapon_effect;
std::vector<received_effect_player_t> ako_player_effect;

void get_ako_effect()
{
	////////////////////////////////////////////////////////////////////////
	ako_weapon_effect.push_back({false,false,false,true,false,0.7,1,1});//0
	ako_weapon_effect.push_back({false,false,false,true,false,0.5,1,1});//1
	ako_weapon_effect.push_back({false,false,false,true,false,0.2,1,1});//2
	ako_weapon_effect.push_back({true,false,false,true,false,1,1,1});//3
	////////////////////////////////////////////////////////////////////////////
	ako_planet_effect.push_back({true,false,1,0,1});//0
	ako_planet_effect.push_back({false,false,1,100,1});//1
	ako_planet_effect.push_back({false,false,1,500,1});//2
	ako_planet_effect.push_back({false,false,1,1000,1});//3
	ako_planet_effect.push_back({false,false,1,0,2});//4
	ako_planet_effect.push_back({false,false,1,0,5});//5
	ako_planet_effect.push_back({false,false,1,1,1});//6
	ako_planet_effect.push_back({false,false,1,3,1});//7
	ako_planet_effect.push_back({false,false,1,6,1});//8
	ako_planet_effect.push_back({false,false,1,0,1.004});//9
	ako_planet_effect.push_back({false,false,1,0,1.009});//10
	////////////////////////////////////////////////////////////////////////
	ako_meteorite_effect.push_back({false,1,1,0,1,0.5});//0
	ako_meteorite_effect.push_back({false,1,1,0,1,0.1});//1
	ako_meteorite_effect.push_back({false,1,1,0,1,0});//2
	ako_meteorite_effect.push_back({false,1,5,0,1,0.8});//3
	ako_meteorite_effect.push_back({true,1,1,0,1,0.8});//4
	ako_meteorite_effect.push_back({false,1,1,-1,0.9,1});//5
	///////////////////////////////////////////////////////////////////
	ako_box_effect.push_back({true,0.5,1,0,1});//0
	ako_box_effect.push_back({true,0.1,1,0,1});//1
	ako_box_effect.push_back({false,1,1,0,1});//2
	//////////////////////////////////////////////////////////////
	ako_player_effect.push_back({false,1.5});//0
	ako_player_effect.push_back({false,2});//1
	ako_player_effect.push_back({true,1});//2

	ako_effect.push_back({1500,10,EFFECT_RECIVER_CURRENT_WEAPON,0,0,false,
						  std::function<void(void*)>()});
	ako_effect.push_back({1500,15,EFFECT_RECIVER_CURRENT_WEAPON,1,1,false,
						  std::function<void(void*)>()});
	ako_effect.push_back({1500,20,EFFECT_RECIVER_CURRENT_WEAPON,2,2,false,
						  std::function<void(void*)>()});
	ako_effect.push_back({1500,20,EFFECT_RECIVER_ALL_WEAPONS,3,1,false,
						  std::function<void(void*)>()});
	ako_effect.push_back({1500,40,EFFECT_RECIVER_ALL_WEAPONS,4,2,false,
						  std::function<void(void*)>()});
	ako_effect.push_back({500,25,EFFECT_RECIVER_PLANET,5,0,false,
						  std::function<void(void*)>()});
	ako_effect.push_back({1500,60,EFFECT_RECIVER_PLANET,6,0,false,
						  std::function<void(void*)>()});
	ako_effect.push_back({3000,100,EFFECT_RECIVER_PLANET,7,0,false,
						  std::function<void(void*)>()});
	ako_effect.push_back({1,15,EFFECT_RECIVER_PLANET,8,1,false,
						  std::function<void(void*)>()});
	ako_effect.push_back({1,20,EFFECT_RECIVER_PLANET,9,1,false,
						  std::function<void(void*)>()});
	ako_effect.push_back({1,30,EFFECT_RECIVER_PLANET,10,1,false,
						  std::function<void(void*)>()});
	ako_effect.push_back({1,50,EFFECT_RECIVER_PLANET,11,1,false,
						  std::function<void(void*)>()});
	ako_effect.push_back({1,80,EFFECT_RECIVER_PLANET,12,1,false,
						  std::function<void(void*)>()});
	ako_effect.push_back({250,17,EFFECT_RECIVER_METE,13,0,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({250,31,EFFECT_RECIVER_METE,14,1,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({250,50,EFFECT_RECIVER_METE,15,2,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({500,60,EFFECT_RECIVER_METE,16,2,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({1500,10,EFFECT_RECIVER_PLAYER,17,0,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({1500,15,EFFECT_RECIVER_PLAYER,18,1,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({1500,20,EFFECT_RECIVER_PLAYER,19,2,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({1500,19,EFFECT_RECIVER_BOX,20,0,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({1500,26,EFFECT_RECIVER_BOX,21,1,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({1500,38,EFFECT_RECIVER_BOX,22,2,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({250,18,EFFECT_RECIVER_PLANET,23,6,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({250,24,EFFECT_RECIVER_PLANET,24,7,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({250,36,EFFECT_RECIVER_PLANET,25,8,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({250,68,EFFECT_RECIVER_PLANET,26,9,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({250,89,EFFECT_RECIVER_PLANET,27,10,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({500,40,EFFECT_RECIVER_METE,28,3,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({500,47,EFFECT_RECIVER_METE,29,4,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({50,64,EFFECT_RECIVER_METE,30,5,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({1500,20,EFFECT_RECIVER_CURRENT_WEAPON,31,3,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({1500,60,EFFECT_RECIVER_ALL_WEAPONS,32,3,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({1500,13,EFFECT_RECIVER_CURRENT_WEAPON,33,4,false,
						 std::function<void(void*)>()});
	ako_effect.push_back({1500,32,EFFECT_RECIVER_ALL_WEAPONS,34,4,false,
						 std::function<void(void*)>()});
	//TODO: 更改清除healt<=的陨石、补给箱的位置
	ako_effect.push_back({0,72,EFFECT_RECIVER_METE,35,65535,true,
						  [](void *m0)
						  {
							  meteorite_t *m=reinterpret_cast<meteorite_t*>(m0);
							  m->strength_left=0;
						  }});
}
}
