//kernel.cpp: 游戏内核的相关函数和类的定义

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

#include "kernel.hpp"

//floatmp_t PLAYER_BASE_SPEED=0.04;
//uint64_t PLAYER_INIT_HUNGER=3000;
//uint64_t PLAYER_INIT_PILLS=10;

//从kernel.hpp复制，但去掉了extern
namespace kernel
{
namespace attribute
{
//玩家的初始属性
floatmp_t PLAYER_BASE_SPEED;
uint64_t PLAYER_INIT_HUNGER;
uint64_t PLAYER_INIT_PILLS;
}//namespace attribute

//uint64_t是绝对编号，从游戏开始运行时记
std::unordered_map<uint64_t,meteorite_t> meteorite_list;
std::unordered_map<uint64_t,box_t> box_list;
planet_t planet;
player_t player;
intmp_t score;//玩家得分
uint64_t level;//玩家通过的关卡数
uint64_t counter;//绝对编号
uint16_t difficulty;//游戏难度

//与菜单模块通信
namespace comu_menu
{
volatile std::atomic<bool> should_pause;//下一游戏刻时是否应暂停游戏
}

//与控制模块通信
namespace comu_control
{
volatile std::atomic<int16_t> move;//玩家移动，取1/-1/0
//对武器的操作:
//-1：不使用
//-2：射击
//-3：丢弃当前武器
//0~9：选择武器
volatile std::atomic<int16_t> weapon;
volatile std::atomic_flag weapon_direct_lock;
volatile floatmp_t weapon_direct;
//触发的效果
//-1,0：不触发
//-2,0：触发当前效果
//-3,0：丢弃当前效果
//0~9,0~65535：选择效果
volatile std::atomic<uint32_t> active_effect;
}

//与渲染模块通信
namespace comu_paint
{
//下一帧已准备好，每一游戏刻的计算结束后，
//如果ready==false则将新的状态复制到comu_paint，同时将ready设置为true
//ready==true时才进行绘图
//每次绘图结束后，ready被设为false
volatile std::atomic<bool> ready;
std::vector<meteorite_t> meteorite_list;
std::vector<box_t> box_list;
planet_t planet;
player_t player;
intmp_t score;//玩家得分
}

std::thread process_thread;

void init()
{
	
}

void start_process()
{

}

void clear()
{

}
}//namespace kernel
