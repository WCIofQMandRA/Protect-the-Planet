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
	//生成武器收到的效果的列表
	{
		ako_weapon_effect.push_back({false,false,false,true,false,0.7,1,1});
	}

	//生成效果列表
	{
		ako_effect.push_back({1500,10,EFFECT_RECIVER_CURRENT_WEAPON,0,0,false,
							  std::function<void(void*)>()});
	}

}
}
