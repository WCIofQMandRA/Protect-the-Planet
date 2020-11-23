//kernel/default_attribute.h: 只能被kernel.h包含
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

//玩家的初始属性
extern floatmp_t PLAYER_BASE_SPEED;
extern uint64_t PLAYER_INIT_HUNGER;
extern uint64_t PLAYER_INIT_PILLS;