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
#include <boost/math/constants/constants.hpp>


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

//ako: all kinds of，所有可能出现的陨石的列表
static std::vector<meteorite0_t> ako_meteorite;
static std::vector<box0_t> ako_box;
static std::vector<weapon0_t> ako_weapon;
static std::vector<effect_t> ako_effect;
static std::vector<food_t> ako_food;

//meteorites[k]是第k关(编号从0开始)计划生成的所有陨石，get<0>是可能生成的最早时刻，get<1>是可能生成的最晚时刻，get<2>是计划生成的陨石
static std::vector<std::vector<std::tuple<uint64_t,uint64_t,uint16_t>>> meteorites;
static std::vector<std::vector<std::tuple<uint64_t,uint64_t,uint16_t>>> boxes;

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
	std::ifstream configin;
	//初始化预制关卡
	{
		if(configin.open(trpath("[program]/config/levels"),std::ios_base::in|std::ios_base::binary),!configin)
		{
			std::cerr<<"无法读取配置文件："<<trpath("[program]/config/levels")<<std::endl;
			abort();
		}
		uint64_t n;//预制的关卡数，每个关卡先存陨石，再存补给箱
		configin.read(reinterpret_cast<char*>(&n),8);
		//TODO: 完成levels文件
		meteorites.resize(n);
		boxes.resize(n);
		for(uint64_t i=0;i<n;++i)
		{
			uint64_t m;
			configin.read(reinterpret_cast<char*>(&m),8);
			meteorites[i].resize(m);
			for(uint64_t j=0;j<m;++j)
			{
				uint64_t a,b;uint16_t c;
				configin.read(reinterpret_cast<char*>(&a),8);
				configin.read(reinterpret_cast<char*>(&b),8);
				configin.read(reinterpret_cast<char*>(&c),2);
				meteorites[i][j]=std::make_tuple(a,b,c);
			}
			configin.read(reinterpret_cast<char*>(&m),8);
			boxes[i].resize(m);
			for(uint64_t j=0;j<m;++j)
			{
				uint64_t a,b;uint16_t c;
				configin.read(reinterpret_cast<char*>(&a),8);
				configin.read(reinterpret_cast<char*>(&b),8);
				configin.read(reinterpret_cast<char*>(&c),2);
				boxes[i][j]=std::make_tuple(a,b,c);
			}
		}
		configin.close();
	}
	//生成陨石种类列表
	{
		ako_meteorite.push_back({std::make_pair(160,200),0,5,1e7,
								[](intmp_t &health,const floatmp_t &,bool is_neg,const floatmp_t &hurt_rate_planet,const floatmp_t &hurt_rate_meteorite)
								{
									 health-=static_cast<intmp_t>(100*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(160,200),1,10,1e7,
								[](intmp_t &health,const floatmp_t &complete_rate,bool is_neg,const floatmp_t &hurt_rate_planet,const floatmp_t &hurt_rate_meteorite)
								{
									health-=static_cast<intmp_t>(150*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(80,100),2,8,8e6,
								[](intmp_t &health,const floatmp_t &complete_rate,bool is_neg,const floatmp_t &hurt_rate_planet,const floatmp_t &hurt_rate_meteorite)
								{
									health-=static_cast<intmp_t>(80*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(80,100),3,3,1e7,
								[](intmp_t &health,const floatmp_t &complete_rate,bool is_neg,const floatmp_t &hurt_rate_planet,const floatmp_t &hurt_rate_meteorite)
								{
									health-=static_cast<intmp_t>(150*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(550,600),4,30,1e7,
								[](intmp_t &health,const floatmp_t &complete_rate,bool is_neg,const floatmp_t &hurt_rate_planet,const floatmp_t &hurt_rate_meteorite)
								{
									health-=static_cast<intmp_t>(1000*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(60,65),5,3,1e7,
								[](intmp_t &health,const floatmp_t &,bool is_neg,const floatmp_t &hurt_rate_planet,const floatmp_t &hurt_rate_meteorite)
								{
									health-=static_cast<intmp_t>(30*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(800,850),6,1000,5e6,
								[](intmp_t &health,const floatmp_t &complete_rate,bool is_neg,const floatmp_t &hurt_rate_planet,const floatmp_t &hurt_rate_meteorite)
								{
									health=static_cast<intmp_t>(exp(static_cast<floatmp_t>(health)-log(static_cast<floatmp_t>(1.2))*complete_rate*hurt_rate_planet*hurt_rate_meteorite*(is_neg?-1:1)));
								}});
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
