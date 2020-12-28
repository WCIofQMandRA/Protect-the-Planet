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
#include "kernel.init.hpp"
#include "file.hpp"
#include "paint.hpp"
#include "menu.hpp"
#include <fstream>
#include <random>
#include <ctime>
#include <cmath>
#include <QMessageBox>
#include "mainwindow.hpp"

//HD=hunger decrease
//不同事件造成的饥饿值下降量
const uint64_t HD_SHOOT=500;
const uint64_t HD_MOVE=1;
const uint64_t HD_EFFECT=2000;

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
double map_size=3e8;
double player_height=5e6;

uint64_t minimum_keyboard_operating_skip=10;
uint64_t maximum_dropped_box_stay_time=750;
uint64_t infinate_speed_pill_exsit_time=0;
uint64_t hint_subtitle_exsit_time=75;
uint64_t nearly_timeout_time=50;
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
//13：尝试丢弃武器，但未确认
//1/2：通过按键选择武器
//3/4：通过滚轮选择武器
volatile std::atomic<int16_t> weapon;
volatile std::atomic<double> weapon_direct;
//触发的效果
//10,0：不触发
//11,0：触发当前效果
//12,0：丢弃当前效果
//0~9,0~9：选择效果
volatile std::atomic<uint16_t> active_effect;
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
std::vector<std::tuple<double,double,double,double>> infinate_speed_weapon_path_list;
std::vector<std::u32string> hint_subtitle;
}

std::thread process_thread;

/////////////////////////////////////////////////////////////////////

//随机数生成引擎
std::mt19937_64 rand64;

//ako: all kinds of，所有可能出现的陨石的列表
extern std::vector<meteorite0_t> ako_meteorite;
extern std::vector<box0_t> ako_box;
extern std::vector<weapon0_t> ako_weapon;
extern std::vector<effect_t> ako_effect;
extern std::vector<food_t> ako_food;
extern std::vector<received_effect_box_t> ako_box_effect;
extern std::vector<received_effect_meteorite_t> ako_meteorite_effect;
extern std::vector<received_effect_planet_t> ako_planet_effect;
extern std::vector<received_effect_weapon_t> ako_weapon_effect;
extern std::vector<received_effect_player_t> ako_player_effect;

//uint64_t是绝对编号，从游戏开始运行时记
std::map<uint64_t,meteorite_t> meteorite_list;
std::map<uint64_t,box_t> box_list;
std::map<uint64_t,std::pair<boxd_t,uint64_t>> dropped_box_list;
std::map<uint64_t,pill_t> pill_list;
//子弹速度为无穷大的武器在使用时产生一条红线，持续时间为0.15s(3tk)
//值得注意的是，map.first是使用武器的游戏刻，而非绝对编号
std::multimap<uint64_t,std::tuple<double,double,double,double>> infinate_speed_weapon_path_list;
std::vector<pill_t> infinate_speed_pill_list;
//游戏状态提示，显示在行星下方，这是接近玩家视野中心的位置，同样map.first是提示字幕被触发的游戏刻
//一条提示词显示1.5s
std::multimap<uint64_t,std::u32string> hint_subtitle;
planet_t planet;
player_t player;
uint64_t counter;//绝对编号
uint64_t level;//玩家通过的关卡数
uint64_t game_clock;//游戏时钟
uint64_t score;
uint64_t opened_dropped_box;
int64_t selected_item_in_dropped_items_list;
int32_t succeeded;
uint16_t difficulty;//游戏难度


//meteorites[k]是第k关(编号从0开始)计划生成的所有陨石，get<0>是可能生成的最早时刻，get<1>是可能生成的最晚时刻，get<2>是计划生成的陨石
std::vector<std::vector<std::tuple<uint64_t,uint64_t,uint16_t>>> meteorites;
std::vector<std::vector<std::tuple<uint64_t,uint64_t,uint16_t>>> boxes;
std::vector<std::array<std::pair<uint32_t,uint64_t>,3>> presents;

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
uint64_t meteorites_left;
//最后一个被毁灭的陨石或补给箱被毁灭的时刻
uint64_t last_destroy_clock;
//胜利/失败的时刻
uint64_t win_clock,lose_clock;


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
		uint64_t version;
		configin.read(reinterpret_cast<char*>(&version),8);
		if(version>1)
		{
			std::cerr<<"levels文件的版本过新，请升级游戏或使用旧版的gen-levels工具生成levels文件。\n\
本游戏支持的levels文件的最高版本为1。"<<std::endl;
			abort();
		}
		uint64_t n;//预制的关卡数，每个关卡先存陨石，再存补给箱
		configin.read(reinterpret_cast<char*>(&n),8);
		meteorites.resize(n);
		boxes.resize(n);
		presents.resize(n);
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
			///////////////////
			for(int j=0;j<3;++j)
			{
				configin.read(reinterpret_cast<char*>(&presents[i][j].first),4);
				configin.read(reinterpret_cast<char*>(&presents[i][j].second),8);
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
	get_ako_memeorite();
	get_ako_weapon();
	get_ako_food();
	get_ako_effect();
	get_ako_box();
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
	comu_control::active_effect=0;
	comu_control::weapon_direct=0;
	comu_control::change_dropped_item=0;
	//新建关卡
	if(!save_load->load(name,difficulty,level,counter,score,player,planet))
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
		player.weapon[0].from_0(ako_weapon[0]);
		for(int i=1;i<10;++i)
			player.weapon[i].type=65535;
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
		planet.type=urand_between(0,2);
		planet.health=attribute::planet_init_health[difficulty];
		planet.received_effect.clear();
		planet.combined_effect=received_effect_planet_t();
		///////
		hint_subtitle.insert({0,U"按 F11 退出全屏"});
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
	meteorites_left=0;
	win_clock=0;
	lose_clock=0;
	//生成这一关的陨石和补给箱的出现时刻
	for(const auto &i:meteorites[trans_level[difficulty][level]])
	{
		meteorites_thisround[urand_between(i)].push_back(std::get<2>(i));
		boxes_and_meteorites_left++;
		meteorites_left++;
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

std::pair<std::u32string,uint16_t> stop_game()
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
	hint_subtitle.clear();
	infinate_speed_weapon_path_list.clear();
	comu_paint::box_list.clear();
	comu_paint::meteorite_list.clear();
	comu_paint::dropped_box_list.clear();
	comu_paint::dropped_item.first=0xFFFFFFFF;
	if(succeeded==1)
	{
		if(level>=trans_level[difficulty].size())
		{
			QMessageBox::about(nullptr,"",QString::fromUtf8("通关！"));
			if(save_load->remove(player.name,difficulty,level-1,score))
				std::cout<<"保存成功"<<std::endl;
			return {player.name,65535};
		}
		auto [choice,is_continue]=menu::show_congrat(presents[trans_level[difficulty][level-1]]);
		auto &tmp=presents[trans_level[difficulty][level-1]][choice];
		switch(tmp.first&0xFFFF)
		{
		case CONTAIN_TYPE_PILL:
			player.pills+=tmp.second;
			break;
		case CONTAIN_TYPE_FOOD:
			player.hunger+=tmp.second*ako_food[tmp.first>>16].add_hunger;
			break;
		case CONTAIN_TYPE_WEAPON:
		{
			bool flag=false;//是否成功获得武器
			for(size_t i=0;i<10;++i)
			{
				if(player.weapon[i].type==65535)
				{
					player.weapon[i].from_0(ako_weapon[tmp.first>>16]);
					flag=true;
					break;
				}
			}
			if(!flag)
			{
				//TODO
				std::cout<<"背包已满，无法获得武器"<<std::endl;
			}
			break;
		}
		case CONTAIN_TYPE_EFFECT:
		{
			for(uint64_t k=0;k<tmp.second;++k)
			{
				for(uint16_t i=0;true;++i)
				{
					for(uint16_t j=0;j<5;++j)
					{
						if(player.effect.count(compress16(i,j))==0)
						{
							player.effect[compress16(i,j)]=tmp.first>>16;
							goto endfor;
						}
					}
				}
				endfor:;
			}
			break;
		}
		default:
			assert(0);
		}//switch(tmp.first&0xFFFF)
		if(save_load->save(player.name,difficulty,level,counter,score,player,planet))
			std::cout<<"保存成功"<<std::endl;
		return {player.name,is_continue?difficulty:65535};
	}
	else if(succeeded==-1)
	{
		if(save_load->remove(player.name,difficulty,level-1,score))
			std::cout<<"保存成功"<<std::endl;
		return {player.name,65535};
	}
	return {player.name,65535};
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
	size_t try_times=1;
	while(tmp=std::get<0>(result).calc_r(std::get<1>(result)),tmp>attribute::map_size*0.5||tmp<planet.size*4)
	{
		result=generate_0();
		if(++try_times>100)
		{
			std::cerr<<"无法生成满足要求的轨道 t="<<t<<" r="<<tmp<<std::endl;
			break;
		}
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
		comu_paint::hint_subtitle.resize(hint_subtitle.size());
		comu_paint::infinate_speed_weapon_path_list.resize(infinate_speed_weapon_path_list.size());
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
		i=hint_subtitle.size()-1;
		for(auto it=hint_subtitle.begin();it!=hint_subtitle.end();++it,--i)
		{
			comu_paint::hint_subtitle[i]=it->second;
		}
		i=0;
		for(auto it=infinate_speed_weapon_path_list.begin();it!=infinate_speed_weapon_path_list.end();++it,++i)
		{
			comu_paint::infinate_speed_weapon_path_list[i]=it->second;
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
	for(auto i=meteorite_list.begin();i!=meteorite_list.end();)
	{
		if(i->second.strength_left>0)
		{
			i->second.theta=i->second.orbit.calc_theta(i->second.orbit.calc_time(i->second.theta)+i->second.combined_effect.speed_rate);
			++i;
		}
		else//清理IV效果可能将陨石的strength_left设为0
		{
			i=meteorite_list.erase(i);
			meteorites_left--;
			boxes_and_meteorites_left--;
		}
	}
	for(auto &i:box_list)
	{
		i.second.theta=i.second.orbit.calc_theta(i.second.orbit.calc_time(i.second.theta)+i.second.combined_effect.speed_rate);
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
	//有限速度的子弹
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
				if(i->second.combined_effect.infinate_power||k->second.combined_effect.kill_after_shooted)
					k->second.strength_left=0;
				else
					ako_weapon[i->second.type].use(k->second.strength_left,k->second.strength,i->second.combined_effect.power_rate,k->second.combined_effect.power_rate);
				hint_subtitle.insert({game_clock,std::u32string(U"击中陨石 陨石剩余强度: ")+to_u32(k->second.strength_left.str())});
				if(k->second.strength_left<=0)
				{
					score+=static_cast<uint64_t>((k->second.strength>64?64+log2(static_cast<floatmp_t>(k->second.strength-63)):k->second.strength));
					meteorite_list.erase(k);
					boxes_and_meteorites_left--;
					meteorites_left--;
					if(!boxes_and_meteorites_left)
						last_destroy_clock=game_clock;
				}
				if(!i->second.combined_effect.infinate_hurt_count&&--i->second.hurt_count==0)
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
					if(i->second.combined_effect.infinate_power)
						l->second.strength_left=0;
					else
						ako_weapon[i->second.type].use(l->second.strength_left,l->second.strength,i->second.combined_effect.power_rate,l->second.combined_effect.power_rate);
					hint_subtitle.insert({game_clock,std::u32string(U"击中补给箱 补给箱剩余强度: ")+to_u32(k->second.strength_left.str())});
					if(l->second.strength_left<=0)
					{
						box_list.erase(l);
						boxes_and_meteorites_left--;
						if(!boxes_and_meteorites_left)
							last_destroy_clock=game_clock;
					}
					if(!i->second.combined_effect.infinate_hurt_count&&--i->second.hurt_count==0)
					{
						flag=true;
						break;
					}
				}
			}
		}//for(auto &j:cross_points)
		if(flag)i=pill_list.erase(i);
		else ++i;
	}//for(auto i=pill_list.begin();i!=pill_list.end();)
	//无限速度的子弹
	for(auto &i:infinate_speed_pill_list)
	{
		cross_points.clear();
		double x1=i.x,y1=i.y,x2=i.x+i.dx,y2=i.y+i.dy;
		double theta=atan2(i.dy,i.dx);
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
		bool flag=true;//子弹没有打中目标
		for(auto &j:cross_points)
		{
			//如果打中行星
			if(j.first==0xFFFFFFFFFFu)
			{
				infinate_speed_weapon_path_list.insert({game_clock,{x1,y1,x1+j.second*cos(theta),y1+j.second*sin(theta)}});
				flag=false;
				break;
			}
			//如果打中陨石
			else if(auto k=meteorite_list.find(j.first);k!=meteorite_list.end())
			{
				if(i.combined_effect.infinate_power||k->second.combined_effect.kill_after_shooted)
					k->second.strength_left=0;
				else
					ako_weapon[i.type].use(k->second.strength_left,k->second.strength,i.combined_effect.power_rate,k->second.combined_effect.power_rate);
				hint_subtitle.insert({game_clock,std::u32string(U"击中陨石 陨石剩余强度: ")+to_u32(k->second.strength_left.str())});
				if(k->second.strength_left<=0)
				{
					score+=static_cast<uint64_t>((k->second.strength>64?64+log2(static_cast<floatmp_t>(k->second.strength-63)):k->second.strength));
					meteorite_list.erase(k);
					boxes_and_meteorites_left--;
					meteorites_left--;
					if(!boxes_and_meteorites_left)
						last_destroy_clock=game_clock;
				}
				if(!i.combined_effect.infinate_hurt_count&&--i.hurt_count==0)
				{
					infinate_speed_weapon_path_list.insert({game_clock,{x1,y1,x1+j.second*cos(theta),y1+j.second*sin(theta)}});
					flag=false;
					break;
				}
			}
			//如果打中补给箱
			else
			{
				auto l=box_list.find(j.first);
				if(l->second.combined_effect.hurt_by_weapon)
				{
					if(i.combined_effect.infinate_power)
						l->second.strength_left=0;
					else
						ako_weapon[i.type].use(l->second.strength_left,l->second.strength,i.combined_effect.power_rate,l->second.combined_effect.power_rate);
					hint_subtitle.insert({game_clock,std::u32string(U"击中补给箱 补给箱剩余强度: ")+to_u32(k->second.strength_left.str())});
					if(l->second.strength_left<=0)
					{
						box_list.erase(l);
						boxes_and_meteorites_left--;
						if(!boxes_and_meteorites_left)
							last_destroy_clock=game_clock;
					}
					if(!i.combined_effect.infinate_hurt_count&&--i.hurt_count==0)
					{
						infinate_speed_weapon_path_list.insert({game_clock,{x1,y1,x1+j.second*cos(theta),y1+j.second*sin(theta)}});
						flag=false;
						break;
					}
				}
			}
		}//for(auto &j:cross_points)
		if(flag)
		{
			infinate_speed_weapon_path_list.insert({game_clock,{x1,y1,x2,y2}});
		}
	}//for(auto &i:infinate_speed_pill_list)
	infinate_speed_pill_list.clear();
}

void remove_timeout_infinate_speed_weapon_path()
{
	for(auto i=infinate_speed_weapon_path_list.begin();i!=infinate_speed_weapon_path_list.end();)
	{
		//using std::get;
		//std::cout<<get<0>(i->second)<<" "<<get<1>(i->second)<<" "<<get<2>(i->second)<<" "<<get<3>(i->second)<<" "<<std::endl;
		if(game_clock-i->first>attribute::infinate_speed_pill_exsit_time)
			i=infinate_speed_weapon_path_list.erase(i);
		else
			++i;
	}
}

//检测陨石或补给箱是否到达行星
void check_hit_planet()
{
	for(auto i=meteorite_list.begin();i!=meteorite_list.end();)
	{
		if(i->second.orbit.calc_r(i->second.theta)<planet.size+i->second.size)
		{
			if(!planet.combined_effect.stop_heath_decrease)
			{
				i->second.hurt(planet.health,(double)i->second.strength_left/(double)i->second.strength,planet.combined_effect.negtive_hurt,
							   planet.combined_effect.hurt_rate,i->second.combined_effect.hurt_rate);
				hint_subtitle.insert({game_clock,std::u32string(U"行星完整度：")+to_u32(planet.health.str())});
			}
			i=meteorite_list.erase(i);
			--boxes_and_meteorites_left;
			--meteorites_left;
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

void change_and_throw_weapon()
{
	static uint64_t last_change_clock=0;
	if(uint16_t tmp=comu_control::weapon;tmp!=10
			//||last_...非必要，因为无符号整数的自然溢出
			&&(game_clock-last_change_clock>=attribute::minimum_keyboard_operating_skip/*||last_change_clock>game_clock*/))
	{
		last_change_clock=game_clock;
		switch(tmp)
		{
		case 3:
			comu_control::weapon=10;
			goto lb1;//只是为了消去 implicit-fallthrough 警告
		case 1:
		lb1:
			if(player.chosen_weapon)
				--player.chosen_weapon;
			else
				player.chosen_weapon=9;
			if(player.weapon[player.chosen_weapon].type!=65535)
			hint_subtitle.insert({game_clock,U"选中"+paint::get_name(compress16(CONTAIN_TYPE_WEAPON,player.weapon[player.chosen_weapon].type))});
			break;
		case 4:
			comu_control::weapon=10;
			goto lb2;
		case 2:
		lb2:
			if(player.chosen_weapon<9)
				++player.chosen_weapon;
			else
				player.chosen_weapon=0;
			if(player.weapon[player.chosen_weapon].type!=65535)
			hint_subtitle.insert({game_clock,U"选中"+paint::get_name(compress16(CONTAIN_TYPE_WEAPON,player.weapon[player.chosen_weapon].type))});
			break;
		case 12:
			if(player.weapon[player.chosen_weapon].type!=65535)
			{
				hint_subtitle.insert({game_clock,U"丢弃"+paint::get_name(compress16(CONTAIN_TYPE_WEAPON,player.weapon[player.chosen_weapon].type))});
				player.weapon[player.chosen_weapon].type=65535;
				player.weapon[player.chosen_weapon].received_effect.clear();
			}
			break;
		}
	}
}

void change_effect()
{
	static uint64_t last_change_clock=0;
	if(uint32_t tmp=comu_control::active_effect;tmp
			//||last_...非必要，因为无符号整数的自然溢出
			&&(game_clock-last_change_clock>=attribute::minimum_keyboard_operating_skip/*||last_change_clock>game_clock*/))
	{
		last_change_clock=game_clock;
		int i=player.chosen_effect&0xFFFF,j=player.chosen_effect>>16;
		switch(tmp)
		{
		case 1:if(i)--i;break;
		case 2:++i;break;
		case 3:if(j<4)++j;break;
		case 4:if(j)--j;break;
		}
		if(tmp>=1&&tmp<=4)
		{
			player.chosen_effect=compress16(i,j);
			if(auto it=player.effect.find(player.chosen_effect);it!=player.effect.end())
			{
				hint_subtitle.insert({game_clock,U"选中"+paint::get_name(compress16(CONTAIN_TYPE_EFFECT,it->second))});
			}
		}
	}
}

void weapon_shoot()
{
	player.weapon_direct=comu_control::weapon_direct;
	if(uint16_t tmp=kernel::comu_control::weapon;tmp!=10)
	{
		if(tmp==11)
		{
			auto &weap=player.weapon[player.chosen_weapon];
			if(weap.type!=65535)
			{
				auto &weap0=ako_weapon[weap.type];
				if((player.pills||weap.combined_effect.infinate_pills)&&//有子弹并且已过冷却时间并且饥饿值足够
						(weap.last_use_time+weap0.shoot_speed*weap.combined_effect.shoot_speed_rate<game_clock
						 ||weap.combined_effect.infinate_pill_speed)&&
						(player.combined_effect.stop_hunger_decrease||player.hunger>=HD_SHOOT))
				{
					player.weapon[player.chosen_weapon].last_use_time=game_clock;
					if(ako_weapon[weap.type].infinate_speed||weap.combined_effect.infinate_pill_speed)
					{
						infinate_speed_pill_list.
								push_back({(planet.size+attribute::player_height)*cos(player.position),
								(planet.size+attribute::player_height)*sin(player.position),
								attribute::map_size*2*cos(player.position+player.weapon_direct),
								attribute::map_size*2*sin(player.position+player.weapon_direct),
								weap.type,uint16_t(weap0.hurt_count*weap.combined_effect.hurt_count_rate),weap.combined_effect});
						//std::cerr<<"暂不支持infinate_pill_speed"<<std::endl;
					}
					else
					{
						pill_list[++counter]={(planet.size+attribute::player_height)*cos(player.position),
										  (planet.size+attribute::player_height)*sin(player.position),
										  weap0.pill_speed*weap.combined_effect.pill_speed_rate*cos(player.position+player.weapon_direct),
										  weap0.pill_speed*weap.combined_effect.pill_speed_rate*sin(player.position+player.weapon_direct),
										  weap.type,uint16_t(weap0.hurt_count*weap.combined_effect.hurt_count_rate),weap.combined_effect};
					}
					if(!player.combined_effect.stop_hunger_decrease)
						player.hunger-=HD_SHOOT;
					if(!weap.combined_effect.infinate_pills)
					{
						player.pills--;
					}
				}
			}//if(weap.type!=65535)
		}//if(tmp==11)
		//kernel::comu_control::weapon=10;
	}
}

void player_move()
{
	if(int16_t dir=comu_control::move;dir&&(player.combined_effect.stop_hunger_decrease||player.hunger>=HD_MOVE))
	{
		if(dir==1)
		{
			player.position+=player.speed*player.combined_effect.speed_rate;
			if(!player.combined_effect.stop_hunger_decrease)
				player.hunger-=HD_MOVE;
		}
		else
		{
			player.position-=player.speed*player.combined_effect.speed_rate;
			if(!player.combined_effect.stop_hunger_decrease)
				player.hunger-=HD_MOVE;
		}
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
		//两次更换的最小间隔是0.4s,last_change_clock>game_clock说明进入了新的一轮游戏
		//||last_...非必要，因为无符号整数的自然溢出
		if(~opened_dropped_box&&(game_clock-last_change_clock>=attribute::minimum_keyboard_operating_skip/*||last_change_clock>game_clock*/))
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
							for(uint16_t j=0;j<5;++j)
							{
								if(player.effect.count(compress16(i,j))==0)
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
	if(lose_clock&&lose_clock+attribute::hint_subtitle_exsit_time<game_clock&&!succeeded)
	{
		succeeded=-1;
		comu_menu::game_ended=true;
	}
	//警惕线程通信的延时！
	else if(!boxes_and_meteorites_left&&last_destroy_clock+attribute::hint_subtitle_exsit_time<game_clock&&!succeeded)
	{
		succeeded=1;
		level++;
		comu_menu::game_ended=true;
	}
}

//处理effect中的health/strength add/mul
void add_and_mul_health()
{
	planet.health=static_cast<intmp_t>(static_cast<floatmp_t>(planet.health)*planet.combined_effect.health_mul)
			+planet.combined_effect.health_add;
	for(auto &i:box_list)
	{
		i.second.strength_left=static_cast<intmp_t>(static_cast<floatmp_t>(i.second.strength_left)*i.second.combined_effect.strength_mul)
				+i.second.combined_effect.strength_add;
	}
	for(auto &i:meteorite_list)
	{
		i.second.strength_left=static_cast<intmp_t>(static_cast<floatmp_t>(i.second.strength_left+i.second.combined_effect.strength_add)
													*i.second.combined_effect.strength_mul);
	}
}

void use_effect()
{
	uint64_t last_use_clock=0;
	if(uint16_t tmp=comu_control::active_effect;tmp==5&&game_clock-last_use_clock>attribute::minimum_keyboard_operating_skip)
	{
		last_use_clock=game_clock;
		if(auto it=player.effect.find(player.chosen_effect);it!=player.effect.end()&&it->second!=65535)
		{
			bool active_success=false;
			if(!player.combined_effect.stop_hunger_decrease||player.hunger>=HD_EFFECT)
			{
				switch (ako_effect[it->second].reciever)
				{
				case EFFECT_RECIVER_CURRENT_WEAPON:
				{
					if(player.weapon[player.chosen_weapon].type!=65535)
					{
						if(ako_effect[it->second].instant)
						{
							ako_effect[it->second].use(&player.weapon[player.chosen_weapon]);
						}
						else
						{
							player.weapon[player.chosen_weapon].received_effect[ako_effect[it->second].type]=ako_effect[it->second].time;
						}
						active_success=true;
					}
					break;
				}
				case EFFECT_RECIVER_ALL_WEAPONS:
				{
					if(ako_effect[it->second].instant)
					{
						for(int i=0;i<10;++i)
						{
							if(player.weapon[i].type!=65535)
							{
								ako_effect[it->second].use(&player.weapon[i]);
								active_success=true;
							}
						}
					}
					else
					{
						for(int i=0;i<10;++i)
						{
							if(player.weapon[i].type!=65535)
							{
								player.weapon[i].received_effect[ako_effect[it->second].type]=ako_effect[it->second].time;
								active_success=true;
							}
						}
					}
					break;
				}
				case EFFECT_RECIVER_PLAYER:
				{
					if(ako_effect[it->second].instant)
					{
						ako_effect[it->second].use(&player);
					}
					else
					{
						player.received_effect[ako_effect[it->second].type]=ako_effect[it->second].time;
					}
					active_success=true;
					break;
				}
				case EFFECT_RECIVER_BOX:
				{
					if(ako_effect[it->second].instant)
					{
						for(auto &i:box_list)
						{
							ako_effect[it->second].use(&i.second);
							active_success=true;
						}
					}
					else
					{
						for(auto &i:box_list)
						{
							i.second.received_effect[ako_effect[it->second].type]=ako_effect[it->second].time;
							active_success=true;
						}
					}
					break;
				}
				case EFFECT_RECIVER_METE:
				{
					if(ako_effect[it->second].instant)
					{
						for(auto &i:meteorite_list)
						{
							ako_effect[it->second].use(&i.second);
							active_success=true;
						}
					}
					else
					{
						for(auto &i:meteorite_list)
						{
							i.second.received_effect[ako_effect[it->second].type]=ako_effect[it->second].time;
							active_success=true;
						}
					}
					break;
				}
				case EFFECT_RECIVER_PLANET:
				{
					if(ako_effect[it->second].instant)
					{
						ako_effect[it->second].use(&planet);
					}
					else
					{
						planet.received_effect[ako_effect[it->second].type]=ako_effect[it->second].time;
					}
					active_success=true;
				}
				}
			}
			if(active_success)
			{
				//被use_effect触发的效果不会改变combined_effect，
				//故player.combined_effect.stop_hunger_decrease不会因此改变
				if(!player.combined_effect.stop_hunger_decrease)
					player.hunger-=HD_EFFECT;
				player.effect.erase(it);
			}
		}
	}
}

void remove_timeout_effect()
{
	for(auto it=player.received_effect.begin();it!=player.received_effect.end();)
	{
		//在t时刻触发的效果在t+1时刻开始作用，在(t+持续时间)结束时被清除
		if(it->second--==0)
			it=player.received_effect.erase(it);
		else
		{
			if(it->second==attribute::nearly_timeout_time)
			{
				hint_subtitle.insert({game_clock,U"效果"+paint::get_name(compress16(CONTAIN_TYPE_EFFECT,it->first))+U"即将失效"});
			}
			++it;
		}
	}
	if(player.weapon[player.chosen_weapon].type!=65535)
	{
		auto &i=player.weapon[player.chosen_weapon];
		for(auto it=i.received_effect.begin();it!=i.received_effect.end();)
		{
			if(it->second--==0)
				it=i.received_effect.erase(it);
			else
				++it;
		}
	}
	for(auto it=planet.received_effect.begin();it!=planet.received_effect.end();)
	{
		if(it->second--==0)
			it=planet.received_effect.erase(it);
		else
			++it;
	}
	for(auto &i:meteorite_list)
	{
		for(auto it=i.second.received_effect.begin();it!=i.second.received_effect.end();)
		{
			if(it->second--==0)
				it=i.second.received_effect.erase(it);
			else
				++it;
		}
	}
	for(auto &i:box_list)
	{
		for(auto it=i.second.received_effect.begin();it!=i.second.received_effect.end();)
		{
			if(it->second--==0)
				it=i.second.received_effect.erase(it);
			else
				++it;
		}
	}
}

void calc_combined_effect()
{
	player.combined_effect=received_effect_player_t();
	for(auto &i:player.received_effect)
	{
		player.combined_effect+=ako_player_effect[ako_effect[i.first].detail];
	}
	for(auto &i:player.weapon)
	{
		if(i.type!=65535)
		{
			i.combined_effect=received_effect_weapon_t();
			for(auto &j:i.received_effect)
			{
				i.combined_effect+=ako_weapon_effect[ako_effect[j.first].detail];
			}
		}
	}
	planet.combined_effect=received_effect_planet_t();
	for(auto &i:planet.received_effect)
	{
		planet.combined_effect+=ako_planet_effect[ako_effect[i.first].detail];
	}
	for(auto &i:meteorite_list)
	{
		i.second.combined_effect=received_effect_meteorite_t();
		for(auto &j:i.second.received_effect)
		{
			i.second.combined_effect+=ako_meteorite_effect[ako_effect[j.first].detail];
		}
	}
	for(auto &i:box_list)
	{
		i.second.combined_effect=received_effect_box_t();
		for(auto &j:i.second.received_effect)
		{
			i.second.combined_effect+=ako_box_effect[ako_effect[j.first].detail];
		}
	}
}

//删除、添加提示字幕，但注意，一些提示字幕会在其他函数中添加
void update_hint_subtitles()
{
	//清除过时提示
	for(auto it=hint_subtitle.begin();it!=hint_subtitle.end();)
	{
		if(it->first+attribute::hint_subtitle_exsit_time<game_clock)
			it=hint_subtitle.erase(it);
		else
			++it;
	}
	//添加新提示
	//失败/胜利提示
	if(planet.health<=0&&!lose_clock)
	{
		lose_clock=game_clock;
		hint_subtitle.insert({game_clock,U"行星被毁！"});
	}
	else if(planet.health>0&&meteorites_left==0&&!win_clock)
	{
		win_clock=game_clock;
		hint_subtitle.insert({game_clock,U"已清除所有陨石！"});
	}
}

void process_oneround()
{
	generate_mete_and_box();
	move_mete_and_box();
	move_pill();
	add_and_mul_health();
	check_shooted_by_pill();
	remove_timeout_infinate_speed_weapon_path();
	check_hit_planet();
	change_and_throw_weapon();
	change_effect();
	weapon_shoot();
	player_move();
	check_open_dropped_box();
	clear_up_dropped_box();
	change_selected_item();

	use_effect();
	remove_timeout_effect();
	calc_combined_effect();

	check_win_or_lose();

	update_hint_subtitles();
	////////////////////////////////////
	prepare_data_for_painting();
}
}//namespace kernel
