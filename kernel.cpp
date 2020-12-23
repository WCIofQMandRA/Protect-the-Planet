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
#include "mainwindow.hpp"

//HD=hunger decrease
//不同事件造成的饥饿值下降量
const int64_t HD_SHOOT=500;
const int64_t HD_MOVE=1;
const int64_t HD_EFFECT=2000;

namespace attribute
{
//玩家的初始属性
double player_base_speed[4]={0.04,0.04,0.03};
double player_max_weapon_direct=M_PI/8;
uint64_t player_init_hunger[4]={15000,13000,10000};
uint64_t player_init_pills[4]={20,15,12};
//行星的初始属性
intmp_t planet_init_health[4]={400,300,250};
std::pair<double,double> planet_size={8e6,1.5e7};
std::pair<double,double> planet_GM={0.99e17,1.01e17};
//地图属性
double map_size=2e8;
double player_height=8e6;

uint64_t minimum_select_dropped_item_skip=20;
uint64_t maximum_dropped_box_stay_time=750;
}//namespace attribute

namespace kernel
{
//与菜单模块通信
namespace comu_menu
{
volatile std::atomic<bool> should_pause;//下一游戏刻时是否应暂停游戏
volatile std::atomic<bool> game_ended;//游戏已成功或失败，需要菜单模块调用stop_game()
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
//更换掉在行星上的补给箱内的物品（含义见操作）
//0：不更换
//1/-1：更换
volatile std::atomic<int16_t> change_dropped_item;
}

//与渲染模块通信
namespace comu_paint
{
//下一帧已准备好，每一游戏刻的计算结束后，
//如果ready==false则将新的状态复制到comu_paint，同时将ready设置为true
//ready==true时才进行绘图
//每次绘图结束后，ready被设为false
volatile std::atomic<bool> ready;
std::vector<meteoritep_t> meteorite_list;
std::vector<boxp_t> box_list,dropped_box_list;
std::vector<pill_t> pill_list;
planet_t planet;
player_t player;
uint64_t game_clock,level,score;
std::pair<uint32_t,uint64_t> dropped_item;
}

std::thread process_thread;

/////////////////////////////////////////////////////////////////////

//随机数生成引擎
std::mt19937_64 rand64;

//ako: all kinds of，所有可能出现的陨石的列表
std::vector<meteorite0_t> ako_meteorite;
std::vector<box0_t> ako_box;
std::vector<weapon0_t> ako_weapon;
std::vector<effect_t> ako_effect;
std::vector<food_t> ako_food;
std::vector<received_effect_box_t> ako_box_effect;
std::vector<received_effect_meteorite_t> ako_meteorite_effect;
std::vector<received_effect_planet_t> ako_planet_effect;
std::vector<received_effect_weapon_t> ako_weapon_effect;

//uint64_t是绝对编号，从游戏开始运行时记
std::map<uint64_t,meteorite_t> meteorite_list;
std::map<uint64_t,box_t> box_list;
std::map<uint64_t,std::pair<boxd_t,uint64_t>> dropped_box_list;
std::map<uint64_t,pill_t> pill_list;
planet_t planet;
player_t player;
uint64_t counter;//绝对编号
uint64_t level;//玩家通过的关卡数
uint64_t score;
uint16_t difficulty;//游戏难度
uint64_t opened_dropped_box;
int64_t selected_item_in_dropped_items_list;
int32_t succeeded;
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
//本关卡中，还未被毁灭的陨石和补给箱的数量
uint64_t boxes_and_meteorites_left;
//最后一个被毁灭的陨石或补给箱被毁灭的时刻
uint64_t last_destroy_clock;


inline uint64_t urand_between(uint64_t s,uint64_t t)
{
	return rand64()%(t-s+1)+s;
}

inline uint64_t irand_between(int64_t s,int64_t t)
{
	return static_cast<int64_t>(rand64()%static_cast<uint64_t>(t-s+1))+s;
}

inline double frand_between(double s,double t)
{
	std::uniform_real_distribution<double> un(s,t);
	return un(rand64);
}

template<typename tp>
inline uint64_t urand_between(const tp &x)
{
	return urand_between(std::get<0>(x),std::get<1>(x));
}

template<typename tp>
inline int64_t irand_between(const tp &x)
{
	return irand_between(std::get<0>(x),std::get<1>(x));
}

template<typename tp>
inline double frand_between(const tp &x)
{
	return frand_between(std::get<0>(x),std::get<1>(x));
}

void process_thread_main()
{
	//由于GM是thread_local，必须在这里set_GM
	//std::cout<<"thread start"<<std::endl;
	orbit_t::set_GM(planet.GM);
	using namespace std::chrono_literals;
	auto process_time=std::chrono::system_clock::now()+1ms;
	while(!comu_menu::should_pause)
	{
		std::this_thread::sleep_until(process_time);
		process_oneround();
		process_time+=20ms;
		++game_clock;
	}
	//std::cout<<"thread end"<<std::endl;
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
		ako_meteorite.push_back({std::make_pair(400,500),0,5,5e6,
								[](intmp_t &health,const double &,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
								{
									 health-=static_cast<intmp_t>(100*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(400,500),1,10,5e6,
								[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
								{
									health-=static_cast<intmp_t>(150*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(200,250),2,8,4e6,
								[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
								{
									health-=static_cast<intmp_t>(80*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(200,250),3,3,5e6,
								[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
								{
									health-=static_cast<intmp_t>(150*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(1380,1500),4,30,5e6,
								[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
								{
									health-=static_cast<intmp_t>(1000*complete_rate*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(150,170),5,3,5e6,
								[](intmp_t &health,const double &,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
								{
									health-=static_cast<intmp_t>(30*hurt_rate_planet*hurt_rate_meteorite)*(is_neg?-1:1);
								}});
		ako_meteorite.push_back({std::make_pair(2000,2130),6,1000,2.5e6,
								[](intmp_t &health,const double &complete_rate,bool is_neg,const double &hurt_rate_planet,const double &hurt_rate_meteorite)
								{
									health=static_cast<intmp_t>(exp(log(static_cast<floatmp_t>(health))-log(static_cast<floatmp_t>(1.2))*complete_rate*hurt_rate_planet*hurt_rate_meteorite*(is_neg?-1:1)));
								}});
	}
	//生成武器列表
	{
		ako_weapon.push_back({5,18,1,0,false,[](intmp_t &x,const double &power_rate_pill,const double &power_rate_meteorite/*or power_rate_box*/)
							  {
								  x-=static_cast<intmp_t>(3*power_rate_pill*power_rate_meteorite);
							  },2e6});
	}
	//生成食物列表
	{
		ako_food.push_back({4000,3,0});
	}
	//生成武器收到的效果的列表
	{
		ako_weapon_effect.push_back({false,false,false,true,false,1.5,1,1});
	}

	//生成效果列表
	{
		ako_effect.push_back({1500,10,EFFECT_RECIVER_CURRENT_WEAPON,0,0,false,
							  std::function<void(void*)>()});
	}
	//生成补给箱列表
	{
		ako_box.push_back({std::make_pair(200,300),std::make_pair(0,0),
						   {},
						   {{compress16(CONTAIN_TYPE_PILL,0),30}},
						  0,15,3.7e6});
		ako_box.push_back({std::make_pair(500,550),std::make_pair(17,19),
						   {compress16(CONTAIN_TYPE_FOOD,0),compress16(CONTAIN_TYPE_PILL,0)},
						   {{compress16(CONTAIN_TYPE_EFFECT,0),1}},
						   1,15,2.3e6});
	}

	//初始化随机数引擎
	rand64.seed(time(nullptr));
}

void start_game(const std::u32string &name,uint16_t difficulty)
{
	std::cout<<"kernel:开始游戏"<<std::endl;
	kernel::difficulty=difficulty;
	game_clock=0;succeeded=0;
	comu_menu::should_pause=false;
	comu_menu::game_ended=false;
	comu_paint::ready=false;
	comu_control::weapon=10;
	comu_control::move=0;
	comu_control::active_effect=compress16(10,0);
	comu_control::weapon_direct=0;
	comu_control::change_dropped_item=0;
    //新建关卡
	if(!save_load.load(name,difficulty,level,counter,score,player,planet))
    {
		std::cout<<"加载存档失败"<<std::endl;
        level=0;
        counter=0;
		score=0;
		player.pills=attribute::player_init_pills[difficulty];
		player.hunger=attribute::player_init_hunger[difficulty];
		player.speed=attribute::player_base_speed[difficulty];
		player.chosen_effect=0;
		player.chosen_weapon=0;
		player.weapon.fill(weapon_t());
		player.weapon[0].from_0(ako_weapon[0]);
		player.effect.clear();
		player.received_effect.clear();
		player.combined_effect=received_effect_player_t();
		player.position=0;
		player.weapon_direct=0;
		player.score=0;
		player.name=name;
		dropped_box_list.clear();
		///////////
		planet.size=frand_between(attribute::planet_size);
		planet.GM=frand_between(attribute::planet_GM);
		planet.health=attribute::planet_init_health[difficulty];
		planet.received_effect.clear();
		planet.combined_effect=received_effect_planet_t();
	}
	if(level>=trans_level[difficulty].size())
	{
		std::cerr<<"关卡"<<level<<"不存在"<<std::endl;
		level=0;
		//abort();
	}
	for(auto &i:player.weapon)
		i.last_use_time=0;
	boxes_and_meteorites_left=0;
	//生成这一关的陨石和补给箱的出现时刻
	for(const auto &i:meteorites[trans_level[difficulty][level]])
	{
		meteorites_thisround[urand_between(i)].push_back(std::get<2>(i));
		boxes_and_meteorites_left++;
	}
	for(const auto &i:boxes[trans_level[difficulty][level]])
	{
		boxes_thisround[urand_between(i)].push_back(std::get<2>(i));
		boxes_and_meteorites_left++;
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
	comu_menu::should_pause=true;
	process_thread.join();
	meteorites_thisround.clear();
	boxes_thisround.clear();
	meteorite_list.clear();
	pill_list.clear();
	dropped_box_list.clear();
	box_list.clear();
	comu_paint::box_list.clear();
	comu_paint::meteorite_list.clear();
	comu_paint::dropped_box_list.clear();
	comu_paint::dropped_item.first=0xFFFFFFFF;
	if(succeeded==1)
	{
		if(save_load.save(player.name,difficulty,level,counter,score,player,planet))
			std::cout<<"保存成功"<<std::endl;
	}
	else if(succeeded==-1)
	{
		if(save_load.remove(player.name,difficulty))
			std::cout<<"保存成功"<<std::endl;
	}
}

void clear()
{

}

std::tuple<orbit_t,double,double> generate_orbit(double t,double msize)
{
	double R=planet.size+msize;
	auto generate_0=[t,R]()->std::tuple<orbit_t,double,double>
	{
		//FIXIT:
		//暂时无法生成椭圆轨道，因为判断生成的椭圆轨道是否合法比较困难
		std::lognormal_distribution<double> lnormal(-0.6,0.5);
		double epsilon=lnormal(rand64)+1;
		double theta0=frand_between(0,M_PI*2);
		double r0;
		if(epsilon<1)
			r0=frand_between(R*(1-epsilon),R*(1+epsilon));
		else
			r0=frand_between(0,R*(1+epsilon));
		bool direction=rand64()%2;
		orbit_t orbit;
		orbit.set_orbit(r0,epsilon,theta0,direction);
		//计算沿轨道反方向与行星表面的交点
		double theta_cross=orbit.calc_theta_fromr(R);
		if(!direction)
		{
			theta_cross=2*theta0-theta_cross;
		}
		//从交点到“近日点”（位于行星内）的时间
		double t1=orbit.calc_time(theta_cross);
		//从陨石/补给箱出发到到达“近日点”的时间
		double t2=t1-t;
		double theta_start=orbit.calc_theta(t2);
		return {orbit,theta_start,theta_cross};
	};
	auto result=generate_0();
	double tmp;
	while(tmp=std::get<0>(result).calc_r(std::get<1>(result)),tmp>attribute::map_size*0.5||tmp<planet.size*4)
	{
		result=generate_0();
	}
	return result;
}

//TODO: 允许补给箱中一定包含某些物品
std::vector<std::pair<uint32_t,uint64_t>> generate_contains(std::pair<uint64_t,uint64_t> value,const std::vector<uint32_t> &items,
															const std::vector<std::pair<uint32_t,uint64_t>> &compulsory_items)
{
	std::vector<uint64_t> value_list;//value_list[i]:items[i]的价值
	value_list.resize(items.size());
	for(size_t i=0;i<items.size();++i)
	{
		switch(items[i]&0xFFFFu)
		{
		case CONTAIN_TYPE_FOOD:
			value_list[i]=ako_food[items[i]>>16].value;
			break;
		case CONTAIN_TYPE_PILL:
			value_list[i]=1;
			break;
		case CONTAIN_TYPE_EFFECT:
			value_list[i]=ako_effect[items[i]>>16].value;
			break;
		case CONTAIN_TYPE_WEAPON:
			value_list[i]=ako_weapon[items[i]>>16].value;
			break;
		default:
			assert(0);
		}
	}
	////////////////////////////////////
	static bool cnt[1024];//cnt[i]：是否存在一种取法，使总价值为i
	static size_t valid_size[1024];
	size_t valid_size_size=0;
	static std::pair<size_t,uint32_t> from[1024];
	uint64_t minvalue=value.first,maxvalue=value.second;
	memset(cnt,0,maxvalue+1);
	cnt[0]=true;
	//使用多重背包确定可能取到的总价值
	for(uint64_t i=0;i<=maxvalue;++i)
	{
		//以随机顺序遍历value_list数组
		auto random_dfs=[&](auto &&self,uint64_t l,uint64_t r)->bool
		{
			if(!~r||l>r)return false;
			uint64_t p=urand_between(l,r);
			if(value_list[p]<=i&&cnt[i-value_list[p]])
			{
				cnt[i]=true;
				from[i]={i-value_list[p],p};
				return true;
			}
			return self(self,l,p-1)||self(self,p+1,r);
		};
		random_dfs(random_dfs,0,items.size()-1);
	}
	//在可能总价值中随机选择一个可以取到的价值
	for(uint64_t i=minvalue;i<=maxvalue;++i)
		if(cnt[i])
			valid_size[valid_size_size++]=i;
	size_t choosen=valid_size[urand_between(0,valid_size_size-1)];
	////////////////////////////
	//通过回溯法得到所有被选中的物品
	std::vector<uint32_t> contain_list0;
	for(size_t i=choosen;i;i=from[i].first)
	{
		contain_list0.push_back(from[i].second);
	}
	////////////////////////
	//将重复的物品合并
	std::sort(contain_list0.begin(),contain_list0.end());
	std::vector<std::pair<uint32_t,uint64_t>> contain_list;
	if(contain_list0.size())
		contain_list.push_back({items[contain_list0[0]],1});
	for(size_t i=1;i<contain_list0.size();++i)
	{
		if(items[contain_list0[i]]==(--contain_list.end())->first)
			++(--contain_list.end())->second;
		else
			contain_list.push_back({items[contain_list0[i]],1});
	}
	for(auto &i:compulsory_items)
	{
		contain_list.push_back(i);
	}
	std::shuffle(contain_list.begin(),contain_list.end(),rand64);
	return contain_list;
}

//根据meteorites_thisround和boxes_thisround的信息生成陨石或补给箱
void generate_mete_and_box()
{
	if(auto it=meteorites_thisround.find(game_clock);it!=meteorites_thisround.end())
	{
		for(const auto &i:it->second)
		{
			meteorite_t tmp;
			std::tie(tmp.orbit,tmp.theta,std::ignore)
					=generate_orbit(urand_between(ako_meteorite[i].fly_time),ako_meteorite[i].size);
			tmp.hurt=ako_meteorite[i].hurt;
			tmp.size=ako_meteorite[i].size;
			tmp.type=i;
			assert(i==ako_meteorite[i].type);
			tmp.strength=ako_meteorite[i].strength;
			tmp.strength_left=ako_meteorite[i].strength;
			meteorite_list[++counter]=std::move(tmp);
		}
	}
	if(auto it=boxes_thisround.find(game_clock);it!=boxes_thisround.end())
	{
		for(const auto &i:it->second)
		{
			box_t tmp;
			std::tie(tmp.orbit,tmp.theta,std::ignore)
					=generate_orbit(urand_between(ako_box[i].fly_time),ako_box[i].size);
			tmp.size=ako_box[i].size;
			tmp.type=i;
			assert(i==ako_box[i].type);
			tmp.strength=ako_box[i].strength;
			tmp.strength_left=ako_box[i].strength;
			tmp.contains=generate_contains(ako_box[i].total_value,ako_box[i].probal_contain,ako_box[i].defint_contain);
			box_list[++counter]=std::move(tmp);
		}
	}
}

//将内核信息复制到comu_paint
void prepare_data_for_painting()
{
	if(!comu_paint::ready)
	{
		comu_paint::planet=planet;
		comu_paint::player=player;
		comu_paint::meteorite_list.resize(meteorite_list.size());
		comu_paint::box_list.resize(box_list.size());
		comu_paint::pill_list.resize(pill_list.size());
		comu_paint::dropped_box_list.resize(dropped_box_list.size());
		size_t i=0;
		for(auto it=meteorite_list.cbegin();it!=meteorite_list.cend();++it,++i)
		{
			it->second.to_p(comu_paint::meteorite_list[i]);
		}
		i=0;
		for(auto it=box_list.cbegin();it!=box_list.cend();++it,++i)
		{
			it->second.to_p(comu_paint::box_list[i]);
		}
		i=0;
		for(auto it=dropped_box_list.cbegin();it!=dropped_box_list.cend();++it,++i)
		{
			it->second.first.to_p(comu_paint::dropped_box_list[i]);
		}
		i=0;
		for(auto it=pill_list.cbegin();it!=pill_list.cend();++it,++i)
		{
			comu_paint::pill_list[i]=(it->second);
		}
		if(~opened_dropped_box)
			comu_paint::dropped_item=dropped_box_list[opened_dropped_box].first.contains[selected_item_in_dropped_items_list];
		else
			comu_paint::dropped_item={0xFFFFFFFF,0};
		comu_paint::game_clock=game_clock;
		comu_paint::score=score;
		comu_paint::level=level;
		comu_paint::ready=true;
	}
}

//移动陨石和补给箱
void move_mete_and_box()
{
	for(auto &i:meteorite_list)
	{
		i.second.theta=i.second.orbit.calc_theta(i.second.orbit.calc_time(i.second.theta)+1);
	}
	for(auto &i:box_list)
	{
		i.second.theta=i.second.orbit.calc_theta(i.second.orbit.calc_time(i.second.theta)+1);
	}
}

//移动正在飞行的子弹
void move_pill()
{
	for(auto it=pill_list.begin();it!=pill_list.end();)
	{
		it->second.x+=it->second.dx;
		it->second.y+=it->second.dy;
		if(std::abs(it->second.x)>attribute::map_size*2||std::abs(it->second.y)>attribute::map_size*2)
			it=pill_list.erase(it);
		else ++it;
	}
}

//检测是否有陨石或补给箱被子弹射中，以及子弹是否打中了行星
void check_shooted_by_pill()
{
	std::pair<double,double> ret[2];
	auto sqr=[](double x){return x*x;};
	int num;
	auto circle_cross_line=[&num,&ret,&sqr](double x1,double y1,double x2,double y2,double x0,double y0,double r)
	{
		num=0;
		double dx=x2-x1,dy=y2-y1;
		double A=dx*dx+dy*dy,B=2*dx*(x1-x0)+2*dy*(y1-y0),C=sqr(x1-x0)+sqr(y1-y0)-r*r;
		double delta=B*B-4*A*C;
		//不把相切视为相交
		if(delta>EPS64)
		{
			double t1=(-B-sqrt(delta))/(2*A);
			double t2=(-B+sqrt(delta))/(2*A);
			if(t1-1<EPS64&&t1>-EPS64)
				ret[num++]={x1+t1*dx,y1+t1*dy};
			if(t2-1<EPS64&&t2>-EPS64)
				ret[num++]={x1+t2*dx,y1+t2*dy};
		}
	};
	//game_clock-1~game_clock时间段内，与一个子弹相遇的陨石和补给箱的列表
	//first: 绝对编号 second: 第一个交点与(x1,y1)的距离
	std::vector<std::pair<uint64_t,double>> cross_points;
	for(auto i=pill_list.begin();i!=pill_list.end();)
	{
		cross_points.clear();
		double x1=i->second.x-i->second.dx,y1=i->second.y-i->second.dy,x2=i->second.x,y2=i->second.y;
		for(auto &j:meteorite_list)
		{
			double r=j.second.orbit.calc_r(j.second.theta);
			circle_cross_line(x1,y1,x2,y2,r*cos(j.second.theta),r*sin(j.second.theta),j.second.size);
			if(num==1)
			{
				cross_points.push_back({j.first,hypot(x1-ret[0].first,y1-ret[0].second)});
			}
			else if(num==2)
			{
				double dis1=sqr(x1-ret[0].first)+sqr(y1-ret[0].second);
				double dis2=sqr(x1-ret[1].first)+sqr(y1-ret[1].second);
				cross_points.push_back({j.first,sqrt(std::min(dis1,dis2))});
			}
		}
		for(auto &j:box_list)
		{
			double r=j.second.orbit.calc_r(j.second.theta);
			circle_cross_line(x1,y1,x2,y2,r*cos(j.second.theta),r*sin(j.second.theta),j.second.size);
			if(num==1)
			{
				cross_points.push_back({j.first,hypot(x1-ret[0].first,y1-ret[0].second)});
			}
			else if(num==2)
			{
				double dis1=sqr(x1-ret[0].first)+sqr(y1-ret[0].second);
				double dis2=sqr(x1-ret[1].first)+sqr(y1-ret[1].second);
				cross_points.push_back({j.first,sqrt(std::min(dis1,dis2))});
			}
		}
		{
			circle_cross_line(x1,y1,x2,y2,0,0,planet.size);
			if(num==1)
			{
				cross_points.push_back({0xFFFFFFFFFFu,hypot(x1-ret[0].first,y1-ret[0].second)});
			}
			else if(num==2)
			{
				double dis1=sqr(x1-ret[0].first)+sqr(y1-ret[0].second);
				double dis2=sqr(x1-ret[1].first)+sqr(y1-ret[1].second);
				cross_points.push_back({0xFFFFFFFFFFu,sqrt(std::min(dis1,dis2))});
			}
		}
		std::sort(cross_points.begin(),cross_points.end(),
				  [](const std::pair<uint64_t,double> &a,const std::pair<uint64_t,double> &b)->bool
		{
			return a.second<b.second;
		});
		//是否需要删除当前子弹
		bool flag=false;
		for(auto &j:cross_points)
		{
			//如果打中行星
			if(j.first==0xFFFFFFFFFFu)
			{
				flag=true;
				break;
			}
			//如果打中陨石
			else if(auto k=meteorite_list.find(j.first);k!=meteorite_list.end())
			{
				ako_weapon[i->second.type].use(k->second.strength_left,i->second.combined_effect.power_rate,k->second.combined_effect.power_rate);
				if(k->second.strength_left<=0)
				{
					score+=static_cast<uint64_t>((k->second.strength>64?64+log2(static_cast<floatmp_t>(k->second.strength-63)):k->second.strength));
					meteorite_list.erase(k);
					boxes_and_meteorites_left--;
					if(!boxes_and_meteorites_left)
						last_destroy_clock=game_clock;
				}
				if(--i->second.hurt_count==0)
				{
					flag=true;
					break;
				}
			}
			//如果打中补给箱
			else
			{
				auto l=box_list.find(j.first);
				if(l->second.combined_effect.hurt_by_weapon)
				{
					ako_weapon[i->second.type].use(l->second.strength_left,i->second.combined_effect.power_rate,l->second.combined_effect.power_rate);
					if(l->second.strength_left<=0)
					{
						box_list.erase(l);
						boxes_and_meteorites_left--;
						if(!boxes_and_meteorites_left)
							last_destroy_clock=game_clock;
					}
					if(--i->second.hurt_count==0)
					{
						flag=true;
						break;
					}
				}
			}
		}
		if(flag)i=pill_list.erase(i);
		else ++i;
	}
}

//检测陨石或补给箱是否到达行星
void check_hit_planet()
{
	for(auto i=meteorite_list.begin();i!=meteorite_list.end();)
	{
		if(i->second.orbit.calc_r(i->second.theta)<planet.size+i->second.size)
		{
			i->second.hurt(planet.health,(double)i->second.strength_left/(double)i->second.strength,planet.combined_effect.negtive_hurt
						   ,planet.combined_effect.hurt_rate,i->second.combined_effect.hurt_rate);
			i=meteorite_list.erase(i);
			--boxes_and_meteorites_left;
			if(!boxes_and_meteorites_left)
				last_destroy_clock=game_clock;
		}
		else ++i;
	}
	for(auto i=box_list.begin();i!=box_list.end();)
	{
		if(i->second.orbit.calc_r(i->second.theta)<planet.size+i->second.size)
		{
			boxd_t boxd;
			i->second.to_d(boxd);
			dropped_box_list.insert({i->first,{boxd,game_clock}});
			i=box_list.erase(i);
		}
		else ++i;
	}
}

void change_weapon_and_effect()
{

}

void weapon_shoot()
{
	player.weapon_direct=comu_control::weapon_direct;
	if(uint16_t tmp=kernel::comu_control::weapon;tmp!=10)
	{
		if(tmp==11)
		{
			auto &weap=player.weapon[player.chosen_weapon];
			auto &weap0=ako_weapon[weap.type];
			if((player.pills||weap.combined_effect.infinate_pills)&&//有子弹并且已过冷却时间并且饥饿值足够
					(weap.last_use_time+weap0.shoot_speed*weap.combined_effect.shoot_speed_rate<game_clock
					 ||weap.combined_effect.infinate_pill_speed)&&
					player.hunger>=HD_SHOOT)
			{
				player.weapon[player.chosen_weapon].last_use_time=game_clock;
				if(weap.combined_effect.infinate_pill_speed)
				{
					std::cerr<<"暂不支持infinate_pill_speed"<<std::endl;
				}
				else
				{
					pill_list[++counter]={(planet.size+attribute::player_height)*cos(player.position),
									  (planet.size+attribute::player_height)*sin(player.position),
									  weap0.pill_speed*weap.combined_effect.pill_speed_rate*cos(player.position+player.weapon_direct),
									  weap0.pill_speed*weap.combined_effect.pill_speed_rate*sin(player.position+player.weapon_direct),
									  weap.type,weap0.hurt_count,weap.combined_effect};
				}
				player.hunger-=HD_SHOOT;
				if(!weap.combined_effect.infinate_pills)
				{
					player.pills--;
				}
			}
		}
		//kernel::comu_control::weapon=10;
	}
}

void use_effect()
{

}

void player_move()
{
	if(int16_t dir=comu_control::move;dir&&player.hunger>=HD_MOVE)
	{
		if(dir==1)
			player.position+=player.speed,player.hunger-=HD_MOVE;
		else
			player.position-=player.speed,player.hunger-=HD_MOVE;
		//comu_control::move=0;
	}
}

void check_open_dropped_box()
{
	for(auto &i:dropped_box_list)
	{
		if(planet.size*sqrt(2-2*cos(player.position-i.second.first.theta))<i.second.first.size*2)
		{
			if(opened_dropped_box!=i.first)
			{
				opened_dropped_box=i.first;
				selected_item_in_dropped_items_list=0;
			}
			return;
		}
	}
	opened_dropped_box=0xFFFFFFFFFFFFFFFF;
}

void clear_up_dropped_box()
{
	for(auto it=dropped_box_list.begin();it!=dropped_box_list.end();)
	{
		if(it->second.second+attribute::maximum_dropped_box_stay_time<game_clock)
		{
			if(opened_dropped_box==it->first)
			{
				opened_dropped_box=0xFFFFFFFFFFFFFFFF;
				selected_item_in_dropped_items_list=0;
			}
			it=dropped_box_list.erase(it);
			--boxes_and_meteorites_left;
			if(!boxes_and_meteorites_left)
				last_destroy_clock=game_clock;
		}
		else ++it;
	}
}

void change_selected_item()
{
	static uint64_t last_change_clock=0;
	if(int16_t tmp=comu_control::change_dropped_item)
	{
		//两次更换的最小间隔是0.4s
		if(~opened_dropped_box&&game_clock-last_change_clock>=attribute::minimum_select_dropped_item_skip)
		{
			last_change_clock=game_clock;
			switch(tmp)
			{
			case 1:
				selected_item_in_dropped_items_list++;
				if(selected_item_in_dropped_items_list>=static_cast<int64_t>(dropped_box_list[opened_dropped_box].first.contains.size()))
								selected_item_in_dropped_items_list=0;
				break;
			case -1:
				selected_item_in_dropped_items_list--;
				if(selected_item_in_dropped_items_list<0)
					selected_item_in_dropped_items_list=dropped_box_list[opened_dropped_box].first.contains.size()-1;
				break;
			case 2:
			{
				auto & item=dropped_box_list[opened_dropped_box].first.contains[selected_item_in_dropped_items_list];
				bool success=false;
				switch(item.first&0xFFFF)
				{
				case CONTAIN_TYPE_FOOD:
					player.hunger+=ako_food[item.first>>16].add_hunger*item.second;
					success=true;
					break;
				case CONTAIN_TYPE_PILL:
					player.pills+=item.second;
					success=true;
					break;
				case CONTAIN_TYPE_EFFECT:
				{
					//每个效果可能有多个
					for(uint64_t k=0;k<item.second;++k)
					{
						for(uint16_t i=0;true;++i)
						{
							for(uint16_t j=0;j<10;++j)
							{
								if(player.effect.count(compress16(i,j))==0||player.effect[compress16(i,j)]==65535)
								{
									player.effect[compress16(i,j)]=item.first>>16;
									goto endfor;
								}
							}
						}
						endfor:;
					}
					success=true;
					break;
				}
				case CONTAIN_TYPE_WEAPON:
				{
					for(int i=0;i<10;++i)
					{
						if(player.weapon[i].type==65535)
						{
							player.weapon[i].from_0(ako_weapon[item.first>>16]);
							success=true;
							break;
						}
					}
					break;
				}
				}//switch(item.first&0xFFFF)
				if(success)
				{
					dropped_box_list[opened_dropped_box].first.contains.erase(dropped_box_list[opened_dropped_box].first.contains.begin()+selected_item_in_dropped_items_list);
					if(size_t size=dropped_box_list[opened_dropped_box].first.contains.size();size==0)
					{
						dropped_box_list.erase(opened_dropped_box);
						--boxes_and_meteorites_left;
						if(!boxes_and_meteorites_left)
							last_destroy_clock=game_clock;
						opened_dropped_box=0xFFFFFFFFFFFFFFFF;
					}
					else
					{
						selected_item_in_dropped_items_list++;
						if(selected_item_in_dropped_items_list>=static_cast<int64_t>(size))
							selected_item_in_dropped_items_list=0;
					}
				}
				break;
			}//case 2
			}//switch(tmp)

		}
	}
}

void check_win_or_lose()
{
	//std::cout<<boxes_and_meteorites_left<<std::endl;
	if(planet.health<0)
	{
		//TODO:失败
		succeeded=-1;
		comu_menu::game_ended=true;
	}
	//警惕线程通信的延时！
	else if(!boxes_and_meteorites_left&&last_destroy_clock+150<game_clock&&!succeeded)
	{
		//TODO:成功
		succeeded=1;
		level++;
		comu_menu::game_ended=true;
	}
}

void process_oneround()
{
	generate_mete_and_box();
	move_mete_and_box();
	move_pill();
	check_shooted_by_pill();
	check_hit_planet();
	change_weapon_and_effect();
	weapon_shoot();
	use_effect();
	player_move();
	check_open_dropped_box();
	clear_up_dropped_box();
	change_selected_item();
	check_win_or_lose();
	////////////////////////////////////
	prepare_data_for_painting();
}
}//namespace kernel
