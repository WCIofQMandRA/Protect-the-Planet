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

//未生成的武器（weapon_t去掉received_effect和combined_effect）
struct weapon0_t
{
	uint64_t value;//武器的价值
	uint64_t shoot_speed;//射击速度（两次射击至少间隔shoot_speed tk）
	uint16_t hurt_count;//一枚子弹可穿透多少个陨石，为了方便，姑且令穿透陨石后不减速
	uint16_t type;
	bool infinate_speed;//子弹可瞬间命中，即在下一次更新场景时，清除子弹前进方向上的第一个目标
	//use(intmp_t &x,const intmp_t &y,double power_rate_pill,
	//double power_rate_meteorite/*or power_rate_box*/)
	//对某个剩余强度为x的陨石（或补给箱）成功使用一次该武器后，该陨石（或补给箱）新的剩余强度
	//y是陨石/补给箱的总强度
	std::function<void(intmp_t&,const intmp_t&,double,double)> use;
	//子弹的速率，不能为inf
	double pill_speed;
};

//武器
struct weapon_t
{
	uint16_t type;
	uint64_t last_use_time;//上一次使用武器的时间
	//效果编号，持续时间
	std::map<uint16_t,uint64_t> received_effect;
	received_effect_weapon_t combined_effect;
	void from_0(const weapon0_t&);
};
