//paint.hpp: 渲染模块的函数和类的声明

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
#ifndef PAINT_H
#define PAINT_H
#include <QWidget>

namespace paint
{
//在特定区域绘制游戏地图
void draw_map(QWidget *place,uint16_t state);

//获得指定物品的图像资源
const QPixmap& get_image_resource(uint32_t);

//返回指定物品的名称
const std::u32string& get_name(uint32_t);
}

#endif // PAINT_H
