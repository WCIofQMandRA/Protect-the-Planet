//include.cpp 文件路径转换

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
#include "file.hpp"
#include <QCoreApplication>
#include <QStandardPaths>

#ifdef NDEBUG
const std::string program_dir=QCoreApplication::applicationDirPath().toStdString();
const std::string storage_dir=QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toStdString()+"/ProPlanet";
const std::string tmp_dir=QStandardPaths::writableLocation(QStandardPaths::TempLocation).toStdString()+"/ProPlanet";
#else
const std::string program_dir=QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString()+"/ProPlanet";
const std::string storage_dir=QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toStdString()+"/ProPlanet";
const std::string tmp_dir=QStandardPaths::writableLocation(QStandardPaths::TempLocation).toStdString()+"/ProPlanet";
#endif
//考虑到该函数会被频繁地调用，不放在命名空间内
//将文件的程序内路径转换为绝对路径
//路径一律用正斜杠/
//使用[program]表示应用程序所在的路径
//使用[storage]表示储存存档的路径
//使用[tmp]表示临时路径
//示例：[program]/levels/是储存关卡信息的目录
//[]必须出现在路径开头
#ifdef _WIN32
std::string trpath(const std::string &path)
{
	assert(path[0]=='[');
	size_t i;
	std::string ans;
	for(i=0;i<path.size();++i)
	{
		if(path[i]==']')break;
	}
	auto prefix=path.substr(0,i+1);
	if(prefix=="[program]")
	{
		ans=program_dir+path.substr(i+1);
	}
	else if(prefix=="[storage]")
	{
		ans=storage_dir+path.substr(i+1);
	}
	else
	{
		assert(prefix=="[tmp]"&&"未知的相对路径");
		ans=storage_dir+path.substr(i+1);
	}
	for(auto &j:ans)
		if(j=='/')j='\\';
	return ans;
}
#else
std::string trpath(const std::string &path)
{
	assert(path[0]=='[');
	size_t i;
	for(i=0;i<path.size();++i)
	{
		if(path[i]==']')break;
	}
	auto prefix=path.substr(0,i+1);
	if(prefix=="[program]")
	{
		return program_dir+path.substr(i+1);
	}
	else if(prefix=="[storage]")
	{
		return storage_dir+path.substr(i+1);
	}
	else
	{
		assert(prefix=="[tmp]"&&"未知的相对路径");
		return storage_dir+path.substr(i+1);
	}
}
#endif
