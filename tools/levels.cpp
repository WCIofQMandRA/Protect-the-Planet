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
	uint64_t n=1;
	fout.write(reinterpret_cast<char*>(&n),8);
	for(uint64_t i=0;i<n;++i)
	{
		uint64_t m=5;
		uint64_t from,to;uint16_t type;
		fout.write(reinterpret_cast<char*>(&m),8);
		set(10,30,0);
		set(200,220,0);
		set(300,310,0);
		set(480,500,1);
		set(520,530,2);
		m=1;
		fout.write(reinterpret_cast<char*>(&m),8);
		set(300,310,0);
	}
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
