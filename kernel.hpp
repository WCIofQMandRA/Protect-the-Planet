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
//	and the GNU Gerneral Public License along with 保卫行星 .
//	If not, see https://www.gnu.org/licenses/.

/*
关于数据类型的使用：
整数：
uint16_t uint64_t
int16_t int64_t intmp_t

浮点数
float64_t float128_t floatmp_t

字符串
u32string

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
#ifndef KERNEL_H
#define KERNEL_H
#include "include.hpp"

//未触发的效果在触发时的接受者
#define EFFECT_RECIVER_PLAYER 1//玩家
#define EFFECT_RECIVER_BOX 2//全体未落下的补给箱
#define EFFECT_RECIVER_CURRENT_WEAPON 3//玩家当前使用的武器
#define EFFECT_RECIVER_ALL_WEAPONS 4//玩家持有的所有武器
#define EFFECT_RECIVER_METE 5//全体陨石
#define EFFECT_RECIVER_PLANET 6//行星

//补给箱内容物的种类
#define CONTAIN_TYPE_WEAPON 1//武器
#define CONTAIN_TYPE_FOOD 2//食物
#define CONTAIN_TYPE_PILL 3//子弹
#define CONTAIN_TYPE_EFFECT 4//效果

#include "kernel/food.hpp"
#include "kernel/effect.hpp"
#include "kernel/weapon.hpp"
#include "kernel/meteorite.hpp"
#include "kernel/planet.hpp"
#include "kernel/box.hpp"
#include "kernel/player.hpp"

namespace kernel
{
namespace attribute
{
//玩家的初始属性
extern floatmp_t PLAYER_BASE_SPEED;
extern uint64_t PLAYER_INIT_HUNGER;
extern uint64_t PLAYER_INIT_PILLS;
}//namespace attribute

//uint64_t是绝对编号，从游戏开始运行时记
extern std::unordered_map<uint64_t,meteorite_t> meteorite_list;
extern std::unordered_map<uint64_t,box_t> box_list;
extern planet_t planet;
extern player_t player;
extern intmp_t score;//玩家得分
extern uint64_t level;//玩家通过的关卡数
extern uint64_t counter;//绝对编号
extern uint16_t difficulty;//游戏难度

//与菜单模块通信
namespace comu_menu
{
extern volatile std::atomic<bool> should_pause;//下一游戏刻时是否应暂停游戏
}

//与控制模块通信
namespace comu_control
{
extern volatile std::atomic<int16_t> move;//玩家移动，取1/-1/0
//对武器的操作:
//-1：不使用
//-2：射击
//-3：丢弃当前武器
//0~9：选择武器
extern volatile std::atomic<int16_t> weapon;
extern volatile std::atomic_flag weapon_direct_lock;
extern volatile floatmp_t weapon_direct;
//触发的效果
//两个uint16_t以小端序压位
//-1,0：不触发
//-2,0：触发当前效果
//-3,0：丢弃当前效果
//0~9,0~65535：选择效果
extern volatile std::atomic<uint32_t> active_effect;
}

//与渲染模块通信
namespace comu_paint
{
//下一帧已准备好，每一游戏刻的计算结束后，
//如果ready==false则将新的状态复制到comu_paint，同时将ready设置为true
//ready==true时才进行绘图
//每次绘图结束后，ready被设为false
extern volatile std::atomic<bool> ready;
extern std::vector<meteorite_t> meteorite_list;
extern std::vector<box_t> box_list;
extern planet_t planet;
extern player_t player;
extern intmp_t score;//玩家得分
}
//内核开始执行时会创建一个线程，该线程储存在process_thread中

//进入游戏后，应调用init()，然后调用start_process()，游戏暂停时，
//内核的执行器终止，即线程process_thread退出，之后可调用start_process()
//恢复游戏，或调用clear()退出游戏
extern std::thread process_thread;
void init();
void start_process();
void clear();
}//namespace kernel

#endif // KERNEL_H
