//types/effect.hpp: 只能被type.hpp包含

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

//玩家收到的（并且正生效的）效果
struct received_effect_player_t
{
	//终止饥饿值下降。合并方式：||
	bool stop_hunger_decrease=false;
	//速度倍率。合并方式：*
	double speed_rate=1;
	received_effect_player_t& operator+=(const received_effect_player_t&);
};

//武器收到的（并且正生效的）效果
struct received_effect_weapon_t
{
	//无限子弹。合并方式：||
	bool infinate_pills=false;
	//无限子弹速度，为true时，忽略pill_speed_rate。合并方式：||
	bool infinate_pill_speed=false;
	//子弹威力无限，即射中陨石即清除陨石。合并方式：||
	bool infinate_power=false;
	//子弹会伤害补给箱。合并方式：&&
	bool hurt_box=true;
	//子弹可穿透无限个陨石。合并方式：||
	bool infinate_hurt_count=false;
	//射击速度倍率（一般小于1）。合并方式：*
	double shoot_speed_rate=1;
	//子弹速度的倍率。合并方式：*
	double pill_speed_rate=1;
	//子弹威力倍率，交给weapon_t::use函数处理。合并方式：*
	double power_rate=1;
	double hurt_count_rate=1;
	received_effect_weapon_t& operator+=(const received_effect_weapon_t&);
};

//行星收到的（并且正生效的）效果
struct received_effect_planet_t
{
	//终止完整程度的下降，即被陨石击中时完整度不变。合并方式：||
	bool stop_heath_decrease=false;
	//行星被陨石击中时，完整度反而会上升。合并方式：||
	bool negtive_hurt=false;
	//受到的伤害的倍率，交给meteorite_t::hurt函数处理。合并方式：*
	double hurt_rate=1;
	//每游戏刻，完整度+health_add。合并方式：+
	intmp_t health_add=0;//通常health_add>=0
	//每游戏刻，完整度*health_mul。合并方式：*
	//规定：先乘后加
	double health_mul=1;//通常health_mul>=1，要求health_mul>0
	received_effect_planet_t& operator+=(const received_effect_planet_t&);
};

//陨石收到的（并且正生效的）效果
struct received_effect_meteorite_t
{
	//被击中后立即毁灭。合并方式：||
	bool kill_after_shooted=false;
	//造成的伤害的倍率，交给meteorite_t::hurt函数处理。合并方式：*
	double hurt_rate=1;
	//受到子弹的破坏的倍率，交给weapon_t::use函数处理。合并方式：*
	double power_rate=1;
	//每游戏刻，强度+strength_add。合并方式：+
	intmp_t strength_add=0;//通常strength_add<=0
	//每游戏刻，强度*strength_mul。合并方式：*
	//规定：先加后乘
	double strength_mul=1;//通常0<strength_mul<=1，要求strength_mul>0
	double speed_rate=1;
	received_effect_meteorite_t& operator+=(const received_effect_meteorite_t&);
};

//未落下的补给箱收到的（并且正生效的）效果
struct received_effect_box_t
{
	//可被武器攻击。合并方式：&&
	bool hurt_by_weapon=true;
	//受到子弹的破坏的倍率，交给weapon_t::use函数处理。合并方式：*
	double power_rate=1;
	double speed_rate=1;
	//通常strength_add>=0,strength_mul>=1
	//规定：先乘后加
	intmp_t strength_add=0;
	double strength_mul=1;
	received_effect_box_t& operator+=(const received_effect_box_t&);
};

//未触发的效果，对一种效果的抽象描述，涉及效果的持续时间、等级和接受对象
//效果的持续时间是uint64_t；等级为为uint16_t
struct effect_t
{
	uint64_t time;//持续时间
	uint64_t value;//效果的价值
	uint16_t reciever;//效果的接受者
	uint16_t type;//效果的类型编号
	uint16_t detail;//效果的详尽描述
	bool instant;//瞬间作用效果
	//关于效果作用的方式：
	//对于非瞬间作用的效果，为接受对象中的received_effect添加一项，并重新计算combined_effect
	//对于瞬间作用的效果，调用use函数，使之处理作用对象
	
	//void use(void *receiver) receiver可能指向player_t、box_t、weapon_t、meteorite_t、planet_t
	std::function<void(void*)> use;
};
