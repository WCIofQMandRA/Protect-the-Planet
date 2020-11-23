//kernel/default_attribute.cpp
//游戏内各事物的默认属性，注意，它们不是常量，可以从硬盘读取

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
#include "../include.h"
#include "./default_attribute.h"

//玩家的初始属性
floatmp_t PLAYER_BASE_SPEED=0.04;
uint64_t PLAYER_INIT_HUNGER=3000;
uint64_t PLAYER_INIT_PILLS=10;