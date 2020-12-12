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
#include <random>
#include <ctime>
#include <cmath>


//double PLAYER_BASE_SPEED=0.04;
//uint64_t PLAYER_INIT_HUNGER=3000;
//uint64_t PLAYER_INIT_PILLS=10;

namespace kernel
{
namespace attribute
{
//玩家的初始属性
double PLAYER_BASE_SPEED[4];
uint64_t PLAYER_INIT_HUNGER[4];
uint64_t PLAYER_INIT_PILLS[4];
//行星的初始属性
intmp_t PLANET_INIT_HEALTH[4];
std::pair<double,double> PLANET_SIZE;
std::pair<double,double> PLANET_GM;
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
volatile std::atomic<double> weapon_direct;
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

/////////////////////////////////////////////////////////////////////

//随机数生成引擎
std::mt19937_64 random_num;

//ako: all kinds of，所有可能出现的陨石的列表
std::vector<meteorite0_t> ako_meteorite;
std::vector<box0_t> ako_box;
std::vector<weapon0_t> ako_weapon;
std::vector<effect_t> ako_effect;
std::vector<food_t> ako_food;

//uint64_t是绝对编号，从游戏开始运行时记
std::unordered_map<uint64_t,meteorite_t> meteorite_list;
std::unordered_map<uint64_t,box_t> box_list;
planet_t planet;
player_t player;
uint64_t counter;//绝对编号
uint64_t level;//玩家通过的关卡数
uint16_t difficulty;//游戏难度
//游戏时钟
uint64_t game_clock;

//meteorites[k]是第k关(编号从0开始)计划生成的所有陨石，get<0>是可能生成的最早时刻，get<1>是可能生成的最晚时刻，get<2>是计划生成的陨石
std::vector<std::vector<std::tuple<uint64_t,uint64_t,uint16_t>>> meteorites;
std::vector<std::vector<std::tuple<uint64_t,uint64_t,uint16_t>>> boxes;

//根据难度换算关卡
//为了方便，游戏只预制了一套关卡，但难度较高时，玩家每通过一关，预制关卡号会增加大于1
//比如，对于简单难度，第一关为内部的关卡0，第二关为内部的关卡1
//对于困难难度，第一关为内部的关卡0，第二关为内部的关卡3
//当然，游戏的难度调节也体现在其他方面，比如玩家的基础移动速度会随难度的增加而减少
std::vector<uint16_t> trans_level[4];

//当前关卡计划生成的陨石和补给箱（在start_game函数中确定）
std::map<uint64_t,std::vector<uint16_t>> meteorites_thisround;
std::map<uint64_t,std::vector<uint16_t>> boxes_thisround;

inline uint64_t urand_between(uint64_t s,uint64_t t)
{
	return random_num()%(t-s+1)+s;
}

inline uint64_t irand_between(int64_t s,int64_t t)
{
	return static_cast<int64_t>(random_num()%static_cast<uint64_t>(t-s+1))+s;
}

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
        ++game_clock;
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
		for(size_t i=0;i<3;++i)
		{
			configin.read(reinterpret_cast<char*>(&n),8);
			trans_level[i].resize(n);
			configin.read(reinterpret_cast<char*>(trans_level[i].data()),2*n);
		}
		configin.close();
	}
	//生成陨石种类列表
	{
		ako_meteorite.push_back({std::make_pair(160,200),0,5,1e7,
								[](intmp_t &health,const double &,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
								{
									 health-=static_cast<intmp_t>(100*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(160,200),1,10,1e7,
								[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
								{
									health-=static_cast<intmp_t>(150*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(80,100),2,8,8e6,
								[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
								{
									health-=static_cast<intmp_t>(80*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(80,100),3,3,1e7,
								[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
								{
									health-=static_cast<intmp_t>(150*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(550,600),4,30,1e7,
								[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
								{
									health-=static_cast<intmp_t>(1000*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(60,65),5,3,1e7,
								[](intmp_t &health,const double &,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
								{
									health-=static_cast<intmp_t>(30*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(800,850),6,1000,5e6,
								[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
								{
									health=static_cast<intmp_t>(exp(static_cast<double>(health)-log(static_cast<double>(1.2))*complete_rate*hurt_rate_planet*hurt_rate_meteorite*(is_neg?-1:1)));
								}});
	}
	//生成武器列表
	{
		ako_weapon.push_back({5,18,0,false,1,[](intmp_t &x,const double &power_rate_pill,const double &power_rate_meteorite/*or power_rate_box*/)
							  {
								  x-=static_cast<intmp_t>(3*power_rate_pill*power_rate_meteorite);
							  },U"手枪",2e6});
	}
	//生成食物列表
	{
		ako_food.push_back({5,1,0,U"糖果"});
	}
	//生成效果列表
	{
		ako_effect.push_back({600,10,EFFECT_RECIVER_CURRENT_WEAPON,0,false,U"快速射击I",
							  received_effect_weapon_t{false,false,false,true,false,1.5,1,1},[](void*){}});
	}
	//生成补给箱列表
	{
		ako_box.push_back({std::make_pair(200,210),std::make_pair(10,13),{std::make_pair(CONTAIN_TYPE_FOOD,0),std::make_pair(CONTAIN_TYPE_EFFECT,0)},0,1000,1.3e6});
	}

	//初始化随机数引擎
	random_num.seed(time(nullptr));
}

void start_game(const std::u32string &name,uint16_t difficulty)
{
	std::cout<<"kernel:开始游戏"<<std::endl;
    kernel::difficulty=difficulty;
    game_clock=0;
	comu_menu::should_pause=false;
    //新建关卡
    if(!save_load::load(name,difficulty,level,counter,player,planet))
    {
        level=0;
        counter=0;
        player.pills=attribute::PLAYER_INIT_PILLS[difficulty];
        player.hunger=attribute::PLAYER_INIT_HUNGER[difficulty];
        player.speed=attribute::PLAYER_BASE_SPEED[difficulty];
        player.chosen_effect=0;
        player.chosen_weapon=0;
        player.weapon.fill(65535);
        player.weapon[0]=0;
        player.effect.clear();
        player.received_effect.clear();
        player.combined_effect=received_effect_player_t();
        player.position=0;
        player.weapon_direct=M_PI/2;
        player.score=0;
        player.name=name;
    }
	//生成这一关的陨石和补给箱的出现时刻
	for(const auto &i:meteorites[trans_level[difficulty][level]])
	{
		meteorites_thisround[urand_between(std::get<0>(i),std::get<1>(i))].push_back(std::get<2>(i));
	}
	for(const auto &i:boxes[trans_level[difficulty][level]])
	{
		boxes_thisround[urand_between(std::get<0>(i),std::get<1>(i))].push_back(std::get<2>(i));
	}
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

//根据meteorites_thisround和boxes_thisround的信息生成陨石或补给箱
void gernerate_mete_or_box()
{
	if(auto it=meteorites_thisround.find(game_clock);it!=meteorites_thisround.end())
	{
		for(const auto &i:it->second)
		{
            ;
		}
	}
	if(auto it=boxes_thisround.find(game_clock);it!=boxes_thisround.end())
	{
		for(const auto &i:it->second)
		{
            ;
		}
	}
}

void process_oneround()
{
	std::cout<<"process"<<std::endl;
}
}//namespace kernel
