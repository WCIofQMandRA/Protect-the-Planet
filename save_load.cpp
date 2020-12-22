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
save_load_class save_load;

save_load_class::save_load_class()
{
	using namespace std;
	ifstream fin(trpath("[storage]/userlist"),ios_base::in|ios_base::binary);
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
		fout.close();
		return;
	}
	uint64_t n;
	fin.read(reinterpret_cast<char*>(&n),8);
	for(size_t i=0;i<n;++i)
	{
		u32string a;
		uint64_t m,b;
		fin.read(reinterpret_cast<char*>(&m),8);
		a.resize(m);
		fin.read(reinterpret_cast<char*>(a.data()),4*m);
		fin.read(reinterpret_cast<char*>(&b),8);
		user_list[a]=b;
	}
}

std::vector<std::u32string> save_load_class::get_userlist()
{
	std::vector<std::u32string> tmp(user_list.size());
	size_t j=0;
	for(const auto &i:user_list)
	{
		tmp[j++]=i.first;
	}
	return tmp;
}

bool save_load_class::add_user(const std::u32string &name)
{
	if(user_list.count(name))
		return false;
	user_list[name]=user_list.size();
	return true;
}

bool save_load_class::delete_user(const std::u32string &name)
{
	if(auto tmp=user_list.find(name);tmp!=user_list.end())
	{
		user_list.erase(tmp);
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
	if(player.name!=name)return false;
	if(auto tmp=user_list.find(name);tmp==user_list.end())
	{
		return false;
	}
	else
	{
		using namespace std;
		string filename=trpath("[storage]/user.")+to_string(user_list[name])+"-"+to_string(difficulty);
		ifstream fin(filename,ios_base::in|ios_base::binary);
		if(!fin)
		{
			cout<<"未找到存档"<<endl;
			return false;
		}
		auto read64=[&fin](uint64_t &x){fin.read(reinterpret_cast<char*>(&x),8);};
		auto reads64=[&fin](int64_t &x){fin.read(reinterpret_cast<char*>(&x),8);};
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
			reads64(player.hunger);read64(player.pills);
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
			fin.read(reinterpret_cast<char*>(&planet.combined_effect),sizeof(planet.combined_effect));
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
		string filename=trpath("[storage]/user.")+to_string(user_list[name])+"-"+to_string(difficulty);
		if(fs::is_regular_file(filename))
			fs::copy(filename,filename+".bak");
		ofstream fout(filename,ios_base::out|ios_base::binary);
		if(!fout)
		{
			cerr<<"写入存档失败"<<endl;
			return false;
		}
		auto write64=[&fout](uint64_t x){fout.write(reinterpret_cast<char*>(&x),8);};
		auto writes64=[&fout](int64_t x){fout.write(reinterpret_cast<char*>(&x),8);};
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
			writes64(player.hunger);write64(player.pills);
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
			fout.write(reinterpret_cast<const char*>(&planet.combined_effect),sizeof(planet.combined_effect));
		}
		if(fout)
		{
			fout.close();
			return true;
		}
		else
		{
			fout.close();
			fs::copy(filename+".bak",filename);
			return false;
		}
	}
}
