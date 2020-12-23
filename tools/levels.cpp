//levels.cpp: config/levels文件的生成
//本文件位于公有领域。
//This file is in public domain.

//g++ level.cpp -o level

#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
using namespace std;

int main(int argc,char **argv)
{
	ofstream fout;
	fout.open(argc>=2?argv[1]:"config",ios_base::out|ios_base::binary);
	auto set=[&fout](uint64_t from,uint64_t to,uint16_t type)
	{
		fout.write(reinterpret_cast<char*>(&from),8);
		fout.write(reinterpret_cast<char*>(&to),8);
		fout.write(reinterpret_cast<char*>(&type),2);
	};
	uint64_t n=1;//关卡数
	fout.write(reinterpret_cast<char*>(&n),8);
	///////////////////////////
	//第一关
	for(uint64_t i=0;i<n;++i)
	{
		uint64_t m=5;//陨石数
		uint64_t from,to;uint16_t type;
		fout.write(reinterpret_cast<char*>(&m),8);
		set(30,40,0);
		set(190,210,0);
		set(360,390,0);
		set(500,530,1);
		set(600,610,2);
		m=2;//补给箱数
		fout.write(reinterpret_cast<char*>(&m),8);
		set(365,400,1);
		set(650,660,0);
	}
	//////////////////////////////
	
	//关卡换算
	vector<uint16_t> trans[3]=
	{
		{0},
		{0},
		{0}
	};
	for(size_t i=0;i<3;++i)
	{
		uint64_t tmp=static_cast<uint64_t>(trans[i].size());
		fout.write(reinterpret_cast<char*>(&tmp),8);
		fout.write(reinterpret_cast<char*>(trans[i].data()),2*trans[i].size());
	}
	fout.close();
	return 0;
}
