//kernel.init.hpp

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

#ifndef KERNEL_INIT_HPP
#define KERNEL_INIT_HPP
#include "type.hpp"
namespace kernel
{
extern std::vector<meteorite0_t> ako_meteorite;
extern std::vector<box0_t> ako_box;
extern std::vector<weapon0_t> ako_weapon;
extern std::vector<effect_t> ako_effect;
extern std::vector<food_t> ako_food;
extern std::vector<received_effect_box_t> ako_box_effect;
extern std::vector<received_effect_meteorite_t> ako_meteorite_effect;
extern std::vector<received_effect_planet_t> ako_planet_effect;
extern std::vector<received_effect_weapon_t> ako_weapon_effect;
extern std::vector<received_effect_player_t> ako_player_effect;

void get_ako_box();
void get_ako_effect();
void get_ako_food();
void get_ako_memeorite();
void get_ako_weapon();
}
#endif // KERNEL_INIT_HPP
