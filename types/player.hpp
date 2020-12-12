//types/player.hpp: 只能被type.hpp包含

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

//玩家
struct player_t
{
	//饥饿值，初始时为3000，每tk减少1，每次射击减少10，每次触发效果减少15，没有上限
	uint64_t hunger;
	uint64_t pills;//玩家持有的子弹的数量
	uint32_t chosen_effect;//玩家当前选择的效果
	uint16_t chosen_weapon;//玩家当前选择的武器
	//玩家持有的武器，最多10把，当持有的武器达到10并继续试图捡起武器时，玩家需丢弃旧的武器才能捡起新的
	std::array<uint16_t,10> weapon;
	//玩家可触发的效果（在物品栏的位置，效果编号），数量，瞬间作用的效果的持续时间为0，
	//玩家最多拥有max_effects个不同种类的效果，但每种效果没有叠加次数限制
	std::map<uint32_t,uint16_t> effect;
	//玩家收到的效果，即正在玩家身上生效的效果
	std::map<uint16_t,received_effect_player_t> received_effect;
	received_effect_player_t combined_effect;
	double position;//玩家的位置，以弧度为单位
	double speed;//玩家的速率，单位rad/tk，受效果的影响
	double weapon_direct;//武器枪口的指向，单位rad。
	intmp_t score;//玩家得分
	std::u32string name;
};
