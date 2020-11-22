//kernel.h: 游戏内核的相关函数和类的声明

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

/*
关于数据类型的使用：
整数：
uint16_t uint64_t
int16_t int64_t intmp_t

浮点数
floatmp_t

关于时间:
50ms为1游戏刻（token，缩写tk）

关于位置：
以“米”为单位

数据类型 单位：
时间：uint64_t tk
速度：floatmp_t m/tk
位矢：floatmp_t m
质量×G：floatmp_t m^3/tk^2
计数器：uint64_t
种类：uint16_t

行星的大小为0.8~1.5×10^7m
地图的大小为4×10^8m×4*10^8m，只渲染2×10^8m×2*10^8m的区域，渲染时的像素数由渲染模块确定
陨石/补给箱的大小为1~10×10^6m

*/
#ifndef CORE_H
#define CORE_H
#include "include.h"

//陨石
struct meteorite_t
{
	std::pair<floatmp_t,floatmp_t> vec_v;//速度
	std::pair<floatmp_t,floatmp_t> vec_r;//位矢
	floatmp_t size;//陨石的大小，决定碰撞箱和渲染
	intmp_t strength;//陨石的强度
	intmp_t strength_left;//陨石的剩余强度
	intmp_t hurt;//陨石的总伤害，落到行星时的伤害为hurt*f(strength_left,strength)，f是一个待确定的函数
	uint16_t type;//陨石的种类，与材质关联
};

//武器
struct weapon_t
{
	uint64_t value;//武器的价值
	uint16_t type;//武器类型
	//use(const intmp_t &x)
	//对某个剩余强度为x的陨石成功使用一次该武器后，该陨石新的剩余强度
	std::function<void(intmp_t&)> use;
	QString name;//武器的名称
	//子弹的速率，速率为inf的子弹可瞬间击中目标，即在下一次跟新场景时，清楚子弹前进方向上的第一个目标
	floatmp_t speed;
};

//食物，获得食物时，玩家立即活动相应的饥饿值
struct food_t
{
	uint64_t add_hunger;//饥饿值的增加量
	QString name;//食物的名称
};

struct effect_t
{
	//效果的接受者
#define EFFECT_RECIVER_PLAYER 1//玩家
#define EFFECT_RECIVER_BOX 2//全体未落下的补给箱
#define EFFECT_RECIVER_CURRENT_WEAPON 3//玩家当前使用的武器
#define EFFECT_RECIVER_ALL_WEAPONS 4//玩家持有的所有武器
#define EFFECT_RECIVER_METE 5//全体陨石
	uint16_t reciever;

};

//补给箱
struct box_t
{
	std::pair<floatmp_t,floatmp_t> vec_v;//速度
	std::pair<floatmp_t,floatmp_t> vec_r;//位矢
	intmp_t strength;//陨石的强度
	intmp_t strength_left;//陨石的剩余强度
	floatmp_t size;//大小，决定碰撞箱和渲染
	//第一层的get<0>表示补给箱的内容物的种类：
#define CONTAIN_TYPE_WEAPON 1//武器
#define CONTAIN_TYPE_FOOD 2//食物
#define CONTAIN_TYPE_PILL 3//子弹
#define CONTAIN_TYPE_EFFECT 4//效果
	//get<1>储存内容物的具体信息
	//get<2>储存内容物的数量
	std::vector<std::tuple<uint16_t,uint16_t,uint64_t>> contains;
};

//行星
struct planet_t
{
	intmp_t health;//行星的完整程度
	floatmp_t size;//行星的大小
	//特殊状态，tuple<剩余时间,等级/强度,剩余强度>
	std::map<uint16_t,std::tuple<uint64_t,std::variant<uint64_t,floatmp_t>,floatmp_t>> effect;
	floatmp_t m_mul_G;//质量×万有引力常数
};

const floatmp_t PLAYER_BASE_SPEED=0.04;
const uint64_t PLAYER_INIT_HUNGER=3000;
const uint64_t PLAYER_INIT_PILLS=10;
//玩家
struct player_t
{
	//饥饿值，初始时为3000，每tk减少1，每次射击减少10，每次触发效果减少15，没有上限
	uint64_t hunger;
	//玩家持有的武器，最多10把，当持有的武器达到10并继续试图捡起武器时，玩家需丢弃旧的武器才能捡起新的
	std::array<uint16_t,10> weapon;
	std::map<uint16_t,uint64_t> effect;//玩家可触发的效果
	uint64_t pills;//玩家持有的子弹的数量
	floatmp_t position;//玩家的位置，以弧度为单位
	floatmp_t speed;//玩家的速率，单位rad/tk，受效果的影响
};

#endif // CORE_H
