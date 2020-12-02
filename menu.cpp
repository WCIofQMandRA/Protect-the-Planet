//menu.cpp: 菜单模块的函数和类的定义

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
#include "menu.hpp"
#include "dialog_pause.hpp"
#include "dialog_start.hpp"
namespace menu
{
//绘制开始界面
std::tuple<std::u32string,uint16_t> show_welcome()
{
	auto dia=new dialog_start;
	std::tuple<std::u32string,uint16_t> result;
	dia->show();
	if(dia->exec())
		result=std::make_tuple(dia->name,dia->difficulty);
	else
		result=std::make_tuple(std::u32string(),static_cast<uint16_t>(65535));
	delete dia;
	return result;
}

//绘制暂停界面
bool show_pause()
{
	dialog_pause dia;
	kernel::comu_menu::should_pause=true;
	dia.show();
	return dia.exec();
}
}
