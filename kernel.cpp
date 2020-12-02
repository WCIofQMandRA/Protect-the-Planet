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
#include "save_load.hpp"
#include "file.hpp"
#include <fstream>

//floatmp_t PLAYER_BASE_SPEED=0.04;
//uint64_t PLAYER_INIT_HUNGER=3000;
//uint64_t PLAYER_INIT_PILLS=10;

//从kernel.hpp复制，但去掉了extern
namespace kernel
{
//uint64_t是绝对编号，从游戏开始运行时记
std::unordered_map<uint64_t,meteorite_t> meteorite_list;
std::unordered_map<uint64_t,box_t> box_list;
planet_t planet;
player_t player;
uint64_t level;//玩家通过的关卡数
uint64_t counter;//绝对编号
uint16_t difficulty;//游戏难度
namespace attribute
{
//玩家的初始属性
floatmp_t PLAYER_BASE_SPEED;
uint64_t PLAYER_INIT_HUNGER;
uint64_t PLAYER_INIT_PILLS;
}//namespace attribute

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
//10：不使用
//11：射击
//12：丢弃当前武器
//0~9：选择武器
volatile std::atomic<int16_t> weapon;
volatile std::atomic_flag weapon_direct_lock;
volatile floatmp_t weapon_direct;
//触发的效果
//10,0：不触发
//11,0：触发当前效果
//12,0：丢弃当前效果
//0~9,0~9：选择效果
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
}

std::thread process_thread;

//meteorites[k]是第k关计划生成的所有陨石，get<0>是可能生成的最早时刻，get<1>是可能生成的最晚时刻，get<2>是计划生成的陨石
static std::vector<std::vector<std::tuple<uint64_t,uint64_t,meteorite_t>>> meteorites;
static std::vector<std::vector<std::tuple<uint64_t,uint64_t,box_t>>> boxes;

void process_thread_main()
{
	using namespace std::chrono_literals;
	std::cout<<"create thread"<<std::endl;
	auto process_time=std::chrono::system_clock::now()+1ms;
	while(!comu_menu::should_pause)
	{
		std::this_thread::sleep_until(process_time);
		process_oneround();
		process_time+=50ms;
	}
	std::cout<<"stop thread"<<std::endl;
}

void init()
{
	//初始化预制关卡
	{
		std::ifstream configin;
		if(configin.open(trpath("[program]/levels/meteorities"),std::ios_base::in|std::ios_base::binary),!configin)
		{
			std::cerr<<"无法读取配置文件："<<trpath("[program]/levels/meteorities")<<std::endl;
			abort();
		}
	}
}

void start_game(const std::u32string &name,uint16_t difficulty)
{
	std::cout<<"kernel:开始游戏"<<std::endl;
	player.name=name;
	kernel::difficulty=difficulty;
	comu_menu::should_pause=false;
	//TODO：加载存档
	if(process_thread.joinable())
		process_thread.join();
	process_thread=std::thread(process_thread_main);
}

void continue_game()
{
	std::cout<<"kernel:继续游戏"<<std::endl;
	comu_menu::should_pause=false;
	if(process_thread.joinable())
		process_thread.join();
	process_thread=std::thread(process_thread_main);
}

void stop_game()
{
	std::cout<<"kernel:终止游戏"<<std::endl;
}

void clear()
{

}

//生成陨石或补给箱
void gernerate_mete_or_box()
{

}

void process_oneround()
{
	std::cout<<"process"<<std::endl;
}
}//namespace kernel
