//save_load.cpp: 配置和存档的读取和写入

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
#include "save_load.hpp"
#include "file.hpp"
#include <QFileDialog>
#include <fstream>
#include <filesystem>
namespace fs=std::filesystem;
//QCoreApplication::applicationDirPath函数必须在主函数中定义，
//而save_load_class的构造函数中调用的trpath函数会调用QCoreApplication::applicationDirPath
//故不能定义save_load_class的全局变量
save_load_class *save_load;

void save_load_class::load_user_list()
{
	using namespace std;
	auto filename=trpath("[storage]/userlist");
	if(fs::is_regular_file(filename+".bak"))
	{
		std::cout<<"检测到上次保存用户列表时出现错误，并且很可能引起了程序崩溃"<<std::endl;
		fs::remove(filename);
		fs::rename(filename+".bak",filename);
	}
	ifstream fin(filename,ios_base::in|ios_base::binary);
	if(!fin)
	{
		fin.close();
		if(!fs::is_directory(trpath("[storage]")))
			fs::create_directory(trpath("[storage]").c_str());
		ofstream fout(trpath("[storage]/userlist"),ios_base::out|ios_base::binary);
		if(!fout)
		{
			cerr<<"无法创建存档文件，因为"<<trpath("[storage]/")<<"目录不可写。"<<endl;
			fout.close();
			abort();
		}
		cout<<"无法读取用户信息，已创建新用户列表"<<trpath("[storage]/userlist")<<"。"<<endl;
		uint64_t a=0;
		fout.write(reinterpret_cast<char*>(&a),8);
		fout.write(reinterpret_cast<char*>(&a),8);//没错，写入两个0ull，第二个表示最后一个进行游戏的玩家是0号玩家
		fout.close();
		return;
	}
	uint64_t n;
	fin.read(reinterpret_cast<char*>(&n),8);
	for(size_t i=0;i<n;++i)
	{
		u32string a;
		uint64_t m,b;
		uint16_t c;
		fin.read(reinterpret_cast<char*>(&m),8);
		a.resize(m);
		fin.read(reinterpret_cast<char*>(a.data()),4*m);
		fin.read(reinterpret_cast<char*>(&b),8);
		fin.read(reinterpret_cast<char*>(&c),2);
		user_list[a]={b,c};
	}
	fin.read(reinterpret_cast<char*>(&last_play_user),8);
	fin.close();
}

void save_load_class::save_user_list()
{
	using namespace std;
	string filename=trpath("[storage]/userlist");
	if(fs::is_regular_file(filename))
	{
		if(fs::is_regular_file(filename+".bak"))
			fs::remove(filename+".bak");
		fs::rename(filename,filename+".bak");
		//由于不明原因，overwrite_existing选择在文件已存在时仍然会报错
		//fs::copy_file(filename,filename+".bak",fs::copy_options::overwrite_existing);
	}
	ofstream fout(filename);
	if(!fout)
	{
		cerr<<"无法保存用户信息"<<endl;
		fout.close();
		return;
	}
	uint64_t n=user_list.size();
	fout.write(reinterpret_cast<const char*>(&n),8);
	for(auto &i:user_list)
	{
		uint64_t m;
		m=i.first.size();
		fout.write(reinterpret_cast<const char*>(&m),8);
		fout.write(reinterpret_cast<const char*>(i.first.data()),4*m);
		fout.write(reinterpret_cast<const char*>(&i.second.first),8);
		fout.write(reinterpret_cast<const char*>(&i.second.second),2);
	}
	fout.write(reinterpret_cast<char*>(&last_play_user),8);
	if(fout)
	{
		fout.close();
		fs::remove(filename+".bak");
	}
	else
	{
		cerr<<"保存用户信息失败"<<endl;
		fout.close();
		fs::remove(filename);
		fs::rename(filename+".bak",filename);
	}
}

void save_load_class::load_ranking_list()
{
	using namespace std;
	auto filename=trpath("[storage]/ranking");
	if(fs::is_regular_file(filename+".bak"))
	{
		std::cout<<"检测到上次保存排行榜时出现错误，并且很可能引起了程序崩溃"<<std::endl;
		fs::remove(filename);
		fs::rename(filename+".bak",filename);
	}
	ifstream fin(filename,ios_base::binary);
	if(!fin)
	{
		fin.close();
		ofstream fout(filename,ios_base::binary);
		if(!fout)
		{
			cerr<<"无法创建排行榜文件，因为"<<trpath("[storage]/")<<"目录不可写。"<<endl;
			fout.close();
			abort();
		}
		cout<<"无法读取排行榜信息，已创建新排行榜"<<filename<<"。"<<endl;
		uint64_t a=0;
		fout.write(reinterpret_cast<char*>(&a),8);
		fout.write(reinterpret_cast<char*>(&a),8);
		fout.write(reinterpret_cast<char*>(&a),8);
		fout.close();
		return;
	}
	for(int i=0;i<3;++i)
	{
		uint64_t size;
		fin.read(reinterpret_cast<char*>(&size),8);
		for(uint64_t j=0;j<size;++j)
		{
			uint64_t a,b,c;
			std::u32string d;
			fin.read(reinterpret_cast<char*>(&a),8);
			fin.read(reinterpret_cast<char*>(&b),8);
			fin.read(reinterpret_cast<char*>(&c),8);
			d.resize(c);
			fin.read(reinterpret_cast<char*>(d.data()),c*4);
			ranking_list[i].insert({{a,b},d});
		}
	}
	fin.close();
}

void save_load_class::save_ranking_list()
{
	using namespace std;
	auto filename=trpath("[storage]/ranking");
	if(fs::is_regular_file(filename))
	{
		if(fs::is_regular_file(filename+".bak"))
			fs::remove(filename+".bak");
		fs::rename(filename,filename+".bak");
	}
	ofstream fout(filename);
	if(!fout)
	{
		cerr<<"无法保存排行榜"<<endl;
		fout.close();
		return;
	}
	for(int i=0;i<3;++i)
	{
		uint64_t size;
		size=ranking_list[i].size();
		fout.write(reinterpret_cast<char*>(&size),8);
		for(auto &j:ranking_list[i])
		{
			uint64_t s=j.second.size();
			fout.write(reinterpret_cast<const char*>(&j.first.first),8);
			fout.write(reinterpret_cast<const char*>(&j.first.second),8);
			fout.write(reinterpret_cast<const char*>(&s),8);
			fout.write(reinterpret_cast<const char*>(j.second.data()),s*4);
		}
	}
	if(fout)
	{
		fout.close();
		fs::remove(filename+".bak");
	}
	else
	{
		cerr<<"保存排行榜失败"<<endl;
		fout.close();
		fs::remove(filename);
		fs::rename(filename+".bak",filename);
	}
}

save_load_class::save_load_class()
{
	load_user_list();
	load_ranking_list();
}

save_load_class::~save_load_class()
{
	save_user_list();
	save_ranking_list();
}

std::vector<std::u32string> save_load_class::get_userlist()
{
	std::vector<std::tuple<std::u32string,uint64_t,uint16_t>> tmp(user_list.size());
	std::vector<std::u32string> tmp2(user_list.size());
	size_t j=0;
	for(const auto &i:user_list)
	{
		tmp[j++]={i.first,i.second.first,i.second.second};
	}
	for(size_t i=0;i<tmp.size();++i)
	{
		if(std::get<1>(tmp[i])==last_play_user)
		{
			std::swap(tmp[i],tmp[0]);
			break;
		}
	}
	if(tmp.size()>2)
	{
		std::sort(tmp.begin()+1,tmp.end(),
		[](const std::tuple<std::u32string,uint64_t,uint16_t> &x,std::tuple<std::u32string,uint64_t,uint16_t> &y)->bool
		{
			return std::get<1>(x)<std::get<1>(y);
		});
	}
	for(size_t i=0;i<tmp.size();++i)
	{
		tmp2[i]=std::get<0>(tmp[i]);
	}
	return tmp2;
}

uint16_t save_load_class::get_last_diff(const std::u32string &name)
{
	return user_list[name].second;
}

bool save_load_class::add_user(const std::u32string &name)
{
	if(user_list.count(name))
		return false;
	//默认难度：普通
	user_list[name]={user_list.size(),1};
	last_play_user=user_list[name].first;
	save_user_list();
	return true;
}

bool save_load_class::delete_user(const std::u32string &name)
{
	if(auto tmp=user_list.find(name);tmp!=user_list.end())
	{
		if(last_play_user==tmp->second.first)
			last_play_user=0;
		user_list.erase(tmp);
		save_user_list();
		return true;
	}
	else
	{
		return false;
	}
}

bool save_load_class::load(const std::u32string &name,uint16_t difficulty,uint64_t &level,uint64_t &counter,
						uint64_t &score,player_t &player,planet_t &planet)
{
	player.name=name;
	if(auto tmp=user_list.find(name);tmp==user_list.end())
	{
		return false;
	}
	else
	{
		using namespace std;
		last_play_user=tmp->second.first;
		//更新用户最后一次进行游戏时的难度
		user_list[name].second=difficulty;
		string filename=trpath("[storage]/user.")+to_string(tmp->second.first)+"-"+to_string(difficulty);
		if(fs::is_regular_file(filename+".bak"))
		{
			cout<<"检测到上次保存存档时出现错误，并且很可能引起了程序崩溃"<<endl;
			fs::remove(filename);
			fs::rename(filename+".bak",filename);
		}
		ifstream fin(filename,ios_base::in|ios_base::binary);
		if(!fin)
		{
			cout<<"未找到存档"<<endl;
			return false;
		}
		auto read64=[&fin](uint64_t &x){fin.read(reinterpret_cast<char*>(&x),8);};
		auto readdb=[&fin](double &x){fin.read(reinterpret_cast<char*>(&x),8);};
		auto read32=[&fin](uint32_t &x){fin.read(reinterpret_cast<char*>(&x),4);};
		auto read16=[&fin](uint16_t &x){fin.read(reinterpret_cast<char*>(&x),2);};
		auto readstr=[&fin,&read64](std::string &x){uint64_t s;read64(s);x.resize(s);fin.read(x.data(),s);};
		auto readmp=[&readstr](intmp_t &x){string s;readstr(s);x=static_cast<intmp_t>(s);};//TODO: 读入原始信息
		read64(level);
		read64(counter);
		read64(score);
		//读取player
		{
			read64(player.hunger);read64(player.pills);
			read32(player.chosen_effect);read16(player.chosen_weapon);
			for(auto &i:player.weapon)
			{
				read16(i.type);read64(i.last_use_time);
				uint64_t size;
				read64(size);
				for(uint64_t j=0;j<size;++j)
				{
					uint16_t first;uint64_t second;
					read16(first);read64(second);
					i.received_effect.insert({first,second});
				}
				fin.read(reinterpret_cast<char*>(&i.combined_effect),sizeof(i.combined_effect));
			}
			uint64_t size;
			read64(size);
			for(uint64_t i=0;i<size;++i)
			{
				uint32_t first;uint16_t second;
				read32(first);read16(second);
				player.effect[first]=second;
			}
			read64(size);
			for(uint64_t i=0;i<size;++i)
			{
				uint16_t first;uint64_t second;
				read16(first);read64(second);
				player.received_effect.insert({first,second});
			}
			fin.read(reinterpret_cast<char*>(&player.combined_effect),sizeof(player.combined_effect));
			readdb(player.position);
			readdb(player.speed);
			readdb(player.weapon_direct);
			readmp(player.score);
		}
		//读取planet
		{
			readmp(planet.health);
			readdb(planet.size);
			readdb(planet.GM);
			uint64_t size;
			read64(size);
			for(uint64_t i=0;i<size;++i)
			{
				uint16_t first;uint64_t second;
				read16(first);read64(second);
				planet.received_effect.insert({first,second});
			}
			//不能直接读，因为received_effect_planet_t中含有intmp
			//fin.read(reinterpret_cast<char*>(&planet.combined_effect),sizeof(planet.combined_effect));
			fin.read(reinterpret_cast<char*>(&planet.combined_effect.stop_heath_decrease),1);
			fin.read(reinterpret_cast<char*>(&planet.combined_effect.negtive_hurt),1);
			readdb(planet.combined_effect.hurt_rate);
			readdb(planet.combined_effect.health_mul);
			readmp(planet.combined_effect.health_add);
			read16(planet.type);
		}
		if(fin)
		{
			fin.close();
			return true;
		}
		else
		{
			fin.close();
			return false;
		}
	}
}

bool save_load_class::save(const std::u32string &name,uint16_t difficulty,uint64_t level,uint64_t counter,
						   uint64_t score,const player_t &player,const planet_t &planet)
{
	if(player.name!=name)return false;
	if(auto tmp=user_list.find(name);tmp==user_list.end())
	{
		return false;
	}
	else
	{
		using namespace std;
		string filename=trpath("[storage]/user.")+to_string(tmp->second.first)+"-"+to_string(difficulty);
		if(fs::is_regular_file(filename))
		{
			if(fs::is_regular_file(filename+".bak"))
				fs::remove(filename+".bak");
			fs::rename(filename,filename+".bak");
		}
		ofstream fout(filename,ios_base::out|ios_base::binary);
		if(!fout)
		{
			cerr<<"写入存档失败"<<endl;
			return false;
		}
		auto write64=[&fout](uint64_t x){fout.write(reinterpret_cast<char*>(&x),8);};
		auto writedb=[&fout](double x){fout.write(reinterpret_cast<char*>(&x),8);};
		auto write32=[&fout](uint32_t x){fout.write(reinterpret_cast<char*>(&x),4);};
		auto write16=[&fout](uint16_t x){fout.write(reinterpret_cast<char*>(&x),2);};
		auto writestr=[&fout,&write64](const std::string &x){write64(x.size());fout.write(x.c_str(),x.size());};
		auto writemp=[&writestr](const intmp_t &x){writestr(x.str());};//TODO: 输出原始信息
		write64(level);
		write64(counter);
		write64(score);
		//写入player
		{
			write64(player.hunger);write64(player.pills);
			write32(player.chosen_effect);write16(player.chosen_weapon);
			for(auto &i:player.weapon)
			{
				write16(i.type);write64(i.last_use_time);
				write64(i.received_effect.size());
				for(auto &j:i.received_effect)
				{
					write16(j.first);write64(j.second);
				}
				fout.write(reinterpret_cast<const char*>(&i.combined_effect),sizeof(i.combined_effect));
			}
			write64(player.effect.size());
			for(auto &i:player.effect)
			{
				write32(i.first);write16(i.second);
			}
			write64(player.received_effect.size());
			for(auto &i:player.received_effect)
			{
				write16(i.first);write64(i.second);
			}
			fout.write(reinterpret_cast<const char*>(&player.combined_effect),sizeof(player.combined_effect));
			writedb(player.position);
			writedb(player.speed);
			writedb(player.weapon_direct);
			writemp(player.score);
		}
		//写入planet
		{
			writemp(planet.health);
			writedb(planet.size);
			writedb(planet.GM);
			write64(planet.received_effect.size());
			for(auto &i:planet.received_effect)
			{
				write16(i.first);write64(i.second);
			}
			//不能直接写，因为received_effect_planet_t中含有intmp
			//fout.write(reinterpret_cast<const char*>(&planet.combined_effect),sizeof(planet.combined_effect));
			fout.write(reinterpret_cast<const char*>(&planet.combined_effect.stop_heath_decrease),1);
			fout.write(reinterpret_cast<const char*>(&planet.combined_effect.negtive_hurt),1);
			writedb(planet.combined_effect.hurt_rate);
			writedb(planet.combined_effect.health_mul);
			writemp(planet.combined_effect.health_add);
			write16(planet.type);
		}
		if(fout)
		{
			fout.close();
			fs::remove(filename+".bak");
			save_user_list();
			return true;
		}
		else
		{
			fout.close();
			fs::remove(filename);
			fs::rename(filename+".bak",filename);
			return false;
		}
	}
}

bool save_load_class::remove(const std::u32string &name, uint16_t difficulty,uint64_t level,uint64_t score)
{
	if(auto tmp=user_list.find(name);tmp==user_list.end())
	{
		return false;
	}
	else
	{
		using namespace std;
		ranking_list[difficulty].insert({{level,score},name});
		auto filename=trpath("[storage]/user.")+to_string(tmp->second.first)+"-"+to_string(difficulty);
		if(fs::is_regular_file(filename))
			fs::remove(filename);
		if(fs::is_regular_file(filename+".bak"))
			fs::remove(filename+".bak");
		save_ranking_list();
		return true;
	}
}

std::vector<std::tuple<std::u32string,uint64_t,uint64_t>> save_load_class::get_ranking(uint16_t difficulty,uint64_t max_count)
{
	std::vector<std::tuple<std::u32string,uint64_t,uint64_t>> result;
	uint64_t i=0;
	for(auto &it:ranking_list[difficulty])
	{
		if(i++<max_count)
			result.push_back({it.second,it.first.first,it.first.second});
		else break;
	}
	return result;
}
