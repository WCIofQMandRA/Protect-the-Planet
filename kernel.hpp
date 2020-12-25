//kernel.hpp: 游戏内核的相关函数和类的声明

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
double

字符串
u32string

关于时间:
20ms为1游戏刻（token，缩写tk）

关于位置：
以“米”为单位

数据类型 单位：
时间：uint64_t tk
速度：double m/tk
位矢：double m
质量×G：double m^3/tk^2
计数器：uint64_t
种类：uint16_t

行星的大小为0.8~1.5×10^7m
地图的大小为2×10^8m×2*10^8m
陨石/补给箱的大小为1~10×10^6m

*/
#ifndef KERNEL_H
#define KERNEL_H
#include "type.hpp"
#include <thread>
#include <atomic>

//游戏状态
#define STATE_PLAYING 1
#define STATE_PAUSE 2
#define STATE_STOP 3
#define STATE_CHOOSING 4

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

namespace kernel
{
//与菜单模块通信
namespace comu_menu
{
extern volatile std::atomic<bool> should_pause;//下一游戏刻时是否应暂停游戏
extern volatile std::atomic<bool> game_ended;//游戏已成功或失败，需要菜单模块调用stop_game()
}

//与控制模块通信
namespace comu_control
{
extern volatile std::atomic<int16_t> move;//玩家移动，取1/-1/0
//对武器的操作:
//10：不使用
//11：射击
//12：丢弃当前武器
//1/2：选择武器
extern volatile std::atomic<int16_t> weapon;
extern volatile std::atomic<double> weapon_direct;
//触发的效果
//0：无操作
//1: 左，2: 右，3: 下，4：上
//5：触发当前效果
//6：丢弃当前效果
extern volatile std::atomic<uint16_t> active_effect;
//更换掉在行星上的补给箱内的物品（含义参考游戏时的操作）
//0：不更换
//1/-1：更换
//2：获取
extern volatile std::atomic<int16_t> change_dropped_item;
}

//与渲染模块通信
namespace comu_paint
{
//下一帧已准备好，每一游戏刻的计算结束后，
//如果ready==false则将新的状态复制到comu_paint，同时将ready设置为true
//ready==true时才进行绘图
//每次绘图结束后，ready被设为false
extern volatile std::atomic<bool> ready;
extern std::vector<meteoritep_t> meteorite_list;
extern std::vector<boxp_t> box_list,dropped_box_list;
extern std::vector<pill_t> pill_list;
extern planet_t planet;
extern player_t player;
extern uint64_t game_clock,level,score;
extern std::pair<uint32_t,uint64_t> dropped_item;
extern std::tuple<double,double,double,double> infinate_speed_weapon_path_list;
extern std::u32string hint_line;
}

//内核开始执行时会创建一个线程，该线程储存在process_thread中

//启动程序时调用init()，进入游戏时调用start_game()，游戏暂停时，
//内核的执行器终止，即线程process_thread退出，之后可调用continue_game()
//恢复游戏，或调用stop_game()退出游戏，程序退出时，调用clear()
extern std::thread process_thread;

void init();
void clear();
void start_game(const std::u32string &name,uint16_t difficulty);
void continue_game();
void stop_game();
void process_oneround();
void process_thread_main();
void prepare_data_for_painting();
}//namespace kernel


#endif // KERNEL_H
