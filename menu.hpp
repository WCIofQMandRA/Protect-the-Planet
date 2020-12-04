//menu.hpp: 菜单模块的函数和类的声明

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
#ifndef MENU_H
#define MENU_H
#include "kernel.hpp"
#include <QWidget>
namespace menu
{
//绘制开始界面，若选择退出游戏则返回{"",65535}
std::tuple<std::u32string,uint16_t> show_welcome();

//绘制暂停界面
//终止游戏：true
//继续游戏：false
bool show_pause();
}
#endif // MENU_H
