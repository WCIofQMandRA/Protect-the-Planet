//types/weapon.hpp: 只能被type.hpp包含

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

//武器
struct weapon_t
{
	uint64_t value;//武器的价值
	uint64_t shoot_speed;//射击速度（两次射击至少间隔shoot_speed tk）
	uint16_t type;//武器类型
	bool infinate_speed;//子弹可瞬间命中，即在下一次更新场景时，清除子弹前进方向上的第一个目标
	bool hurt_count;//一枚子弹可穿透多少个陨石，为了方便，姑且令穿透陨石后不减速
	//use(intmp_t &x,const double &power_rate_pill,
	//const double &power_rate_meteorite/*or power_rate_box*/)
	//对某个剩余强度为x的陨石（或补给箱）成功使用一次该武器后，该陨石（或补给箱）新的剩余强度
	std::function<void(intmp_t&,const double&,const double&)> use;
	std::map<uint16_t,received_effect_weapon_t> received_effect;
	received_effect_weapon_t combined_effect;
	std::u32string name;//武器的名称
	//子弹的速率，不能为inf
	double pill_speed;
};

//未生成的武器（weapon_t去掉received_effect和combined_effect）
struct weapon0_t
{
	uint64_t value;
	uint64_t shoot_speed;
	uint16_t type;
	bool infinate_speed;
	bool hurt_count;
	std::function<void(intmp_t&,const double&,const double&)> use;
	std::u32string name;
	double pill_speed;
};
