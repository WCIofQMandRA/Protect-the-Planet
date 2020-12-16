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

namespace paint
{
//在特定区域绘制游戏地图
void draw_map(QWidget *place,uint16_t state)
{
	QPainter painter(place);
	switch(state)
	{
	case STATE_PLAYING:
	{
		int y=80;
		painter.drawText(100,y+=20,QString("正在游戏%1").arg(kernel::comu_paint::game_clock));
		painter.drawText(100,y+=20,QString("陨石"));
		for(auto &i:kernel::comu_paint::meteorite_list)
		{
			painter.drawText(100,y+=20,QString("(%1,%2) type=%3").arg(i.x).arg(i.y).arg(i.type));
		}
		painter.drawText(100,y+=20,QString("补给箱"));
		for(auto &i:kernel::comu_paint::box_list)
		{
			painter.drawText(100,y+=20,QString("(%1,%2) type=%3").arg(i.x).arg(i.y).arg(i.type));
		}
		kernel::comu_paint::ready=false;
		break;
	}
	case STATE_STOP:
		painter.drawText(100,100,QString("终止游戏"));
		break;
	case STATE_PAUSE:
		if(!kernel::comu_paint::ready)
			kernel::prepare_data_for_painting();
		painter.drawText(100,100,QString("暂停游戏"));
		break;
	default:std::cerr<<"!!!"<<std::endl;assert(0);
	}

	//std::cout<<width()<<" "<<height()<<std::endl;
}
}
