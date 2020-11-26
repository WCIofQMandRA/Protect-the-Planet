//paint.cpp: 渲染模块的函数和类的定义

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

#include "paint.hpp"
#include <QPainter>

//在特定区域绘制游戏地图
void draw_map(QWidget *place)
{
	QPainter painter(place);
	//std::cout<<width()<<" "<<height()<<std::endl;
	painter.drawLine(80*place->width()/1000,100*place->height()/750,650*place->width()/1000,500*place->height()/750);
	painter.setPen(Qt::red);
	painter.drawRect(10*place->width()/1000,10*place->height()/750,100*place->width()/1000,400*place->height()/750);
	painter.setBrush(Qt::blue);
	painter.drawEllipse(50*place->width()/1000,150*place->height()/750,400*place->width()/1000,200*place->height()/750);
}
