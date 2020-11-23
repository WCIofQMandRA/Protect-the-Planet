//kernel/effect.h: 只能被kernel.h包含

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

//未触发的效果，对一种效果的抽象描述，不涉及具体的效果的持续时间、等级，但涉及效果的接受对象
//效果的持续时间是uint64_t；等级可能为uint64_t或floatmp_t，用std::variant<uint64_t,floatmp_t>表示
struct effect_t
{
	//效果的接受者
	uint16_t reciever;

};

//玩家收到的（并且正生效的）效果
struct received_effect_player_t
{

};

//行星收到的（并且正生效的）效果
struct received_effect_planet_t
{

};

//陨石收到的（并且正生效的）效果
struct received_effect_meteorite_t
{

};

//未落下的补给箱收到的（并且正生效的）效果
struct received_effect_box_t
{

};