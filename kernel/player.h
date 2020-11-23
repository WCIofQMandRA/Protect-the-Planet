//kernel/player.h: 只能被kernel.h包含

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

//玩家
struct player_t
{
	//饥饿值，初始时为3000，每tk减少1，每次射击减少10，每次触发效果减少15，没有上限
	uint64_t hunger;
	//玩家持有的武器，最多10把，当持有的武器达到10并继续试图捡起武器时，玩家需丢弃旧的武器才能捡起新的
	std::array<uint16_t,10> weapon;
	std::map<uint16_t,std::pair<uint64_t,std::variant<uint64_t,floatmp_t>>> effect;//玩家可触发的效果（编号，（持续时间，等级））
	uint64_t pills;//玩家持有的子弹的数量
	floatmp_t position;//玩家的位置，以弧度为单位
	floatmp_t speed;//玩家的速率，单位rad/tk，受效果的影响
};