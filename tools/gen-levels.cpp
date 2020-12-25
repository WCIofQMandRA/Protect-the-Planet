//gen-levels.cpp: config/levels文件的生成

//Copyright (C) 2020  张子辰 (GitHub: WCIofQMandRA)

//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.

//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.

//	You should have received a copy of the GNU General Public License
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.

//g++ gen-level.cpp -o gen-level

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <string>
#include <fstream>
#include <tuple>
#include <map>
using namespace std;

string outfilename="levels",infilename="levels.in";
ifstream fin;
ofstream fout;
bool ended=false;

void output_help(string program_name)
{
	cout<<"用法："<<program_name<<" [-i <输入文件名>] [-o <输出文件名>]"<<endl;
	cout<<"默认的输入文件名为 levels.in\t默认的输出文件名为 levels"<<endl;
	cout<<"\n或使用\t"<<program_name<<" -h\t查看本帮助\n"<<endl;
	cout<<"输入文件的格式："<<endl;
	cout<<"使用l(<关卡编号>){<详细描述>}创建关卡"<<endl;
	cout<<"详细描述中，使用m(<最早出现游戏刻>,<最晚出现游戏刻>,<陨石类型>)添加一个陨石，\n使用b(<最早出现游戏刻>,<最晚出现游戏刻>,<补给箱类型>)添加一个补给箱\n"<<endl;
	cout<<"在所有的关卡描述结束后，使用d(<难度>){<详细描述>}定义关卡转换方式"<<endl;
	cout<<"详细描述中，使用<内部关卡编号>(<显示的关卡编号>)定义一条转换规则"<<endl;
}

void analyze_arg(int argc,char **argv)
{
	for(int i=1;i<argc;++i)
	{
		if(string(argv[i])=="-i")
		{
			if(i+1<argc)
			{
				infilename=argv[++i];
			}
			else
			{
				cerr<<"参数错误"<<endl;
				output_help(argv[0]);
				exit(2);
			}
		}
		else if(string(argv[i])=="-o")
		{
			if(i+1<argc)
			{
				outfilename=argv[++i];
			}
			else
			{
				cerr<<"参数错误"<<endl;
				output_help(argv[0]);
				exit(2);
			}
		}
		else if(string(argv[i])=="-h")
		{
			output_help(argv[0]);
			exit(0);
		}
		else
		{
			cerr<<"参数错误"<<endl;
			output_help(argv[0]);
			exit(2);
		}
	}
}

void format_error(int r)
{
	cerr<<"文件 "<<infilename<<" 格式错误，请使用 -h选项查看帮助"<<endl;
	cout<<r<<endl;
	exit(3);
}

char readchar()
{
	char ch;
	while(true)
	{
		fin.read(&ch,1);
		if(ch=='{'||ch=='}'||ch=='('||ch==')'||ch==','||ch>='0'&&ch<='9'||ch=='l'||ch=='m'||ch=='b'||ch=='d')
			break;
		if(!('\0'<=ch&&ch<' '))
		{
			cout<<"已忽略文件中的非法字符 `"<<ch<<"'"<<endl;
		}
		if(!fin)
		{
			ended=true;
			break;
		}
	}
	putchar(ch);
	return ch;
}

template <typename tp>
void write(tp x)
{
	fout.write(reinterpret_cast<const char*>(&x),sizeof(x));
}

void write(size_t x)
{
	write<uint64_t>(x);
}

void write(tuple<uint64_t,uint64_t,uint16_t> x)
{
	write(get<0>(x));
	write(get<1>(x));
	write(get<2>(x));
}

int main(int argc,char **argv)
{
	analyze_arg(argc,argv);
	fin.open(infilename);
	if(!fin)
	{
		cerr<<"打开文件失败"<<endl;
		exit(3);
	}
	map<int,pair<vector<tuple<uint64_t,uint64_t,uint16_t>>,vector<tuple<uint64_t,uint64_t,uint16_t>>>> levels;
	map<int,uint16_t> trans[3];
	char ch=readchar();
	while(!ended)
	{
		if(ch=='l')
		{
			pair<vector<tuple<uint64_t,uint64_t,uint16_t>>,vector<tuple<uint64_t,uint64_t,uint16_t>>> this_level;
			if((ch=readchar())!='(')
			{
				format_error(1);
			}
			int x=0;
			ch=readchar();
			while(ch!=')')
			{
				if(ch>='0'||ch<='9')
					x=x*10+(ch-48);
				else
					format_error(2);
				ch=readchar();
			}
			if((ch=readchar())!='{')
				format_error(3);
			ch=readchar();
			while(ch!='}')
			{
				char m_or_b;
				if(ch=='m')
				{
					m_or_b='m';
				}
				else if(ch=='b')
				{
					m_or_b='b';
				}
				else
				{
					format_error(4);
				}
				if((ch=readchar())!='(')
					format_error(5);
				ch=readchar();
				uint64_t from=0,to=0;
				uint16_t type=0;
				while(ch!=',')
				{
					if(ch>='0'||ch<='9')
						from=from*10+(ch-48);
					else
						format_error(6);
					ch=readchar();
				}
				ch=readchar();
				while(ch!=',')
				{
					if(ch>='0'||ch<='9')
						to=to*10+(ch-48);
					else
						format_error(7);
					ch=readchar();
				}
				ch=readchar();
				while(ch!=')')
				{
					if(ch>='0'||ch<='9')
						type=type*10+(ch-48);
					else
						format_error(8);
					ch=readchar();
				}
				if(m_or_b=='b')
					this_level.second.push_back({from,to,type});
				else
					this_level.first.push_back({from,to,type});
				ch=readchar();
			}
			levels[x]=this_level;
		}
		else if(ch=='d')
		{
			if((ch=readchar())!='(')
				format_error(9);
			int diff=0;
			ch=readchar();
			while(ch!=')')
			{
				if(ch>='0'&&ch<='9')diff=diff*10+(ch-48);
				else format_error(10);
				ch=readchar();
			}
			if(diff>=3)
			{
				cerr<<"难度为0~2"<<endl;
				exit(5);
			}
//			cout<<"{diff="<<diff<<"}";
			if((ch=readchar())!='{')
				format_error(11);
			ch=readchar();
			while(ch!='}')
			{
//				cout<<"while";
				int a=0;
				uint16_t b=0;
				while(ch!='(')
				{
					if(ch>='0'&&ch<='9')a=a*10+(ch-48);
					else {format_error(12);}
					ch=readchar();
				}
				ch=readchar();
				while(ch!=')')
				{
					if(ch>='0'&&ch<='9')b=b*10+(ch-48);
					else format_error(13);
					ch=readchar();
				}
				ch=readchar();
				trans[diff][a]=b;
			}
		}
		else
			format_error(14);
		ch=readchar();
	}
	fout.open(outfilename,ios_base::binary);
	write(levels.size());
	for(int i=0;i<levels.size();++i)
	{
		auto it=levels.find(i);
		if(it==levels.end())
		{
			cerr<<"不得跳过关卡的定义"<<endl;
			exit(6);
		}
		write(it->second.first.size());
		for(auto &j:it->second.first)
		{
			write(j);
		}
		write(it->second.second.size());
		for(auto &j:it->second.second)
			write(j);
	}
	for(int i=0;i<3;++i)
	{
		write(trans[i].size());
		for(int j=0;j<trans[i].size();++j)
		{
			auto it=trans[i].find(j);
			if(it==trans[i].end())
			{
				cerr<<"不得跳过关卡换算的定义"<<endl;
				exit(7);
			}
			write(it->second);
		}
	}
	fout.close();
	return 0;
}
