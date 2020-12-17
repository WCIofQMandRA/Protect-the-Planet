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

namespace attribute
{
//地图属性
extern double MAP_SIZE;
}//namespace attribute

namespace paint
{
void draw_pixmap(QPixmap *pix,int maxsize)
{
	double msize=attribute::MAP_SIZE;
	auto trp=[maxsize,msize](double x,double y)->QPoint
	{
		return QPoint(static_cast<int>(maxsize*(0.5+x/msize)),static_cast<int>(maxsize*(0.5-y/msize)));
	};
	auto trs=[maxsize,msize](double s)->int
	{
		return s/msize*maxsize;
	};
	QPainter painter(pix);
	painter.setPen(QPen(QColor(128,64,0)));
	painter.setBrush(QColor(128,64,0));
	painter.drawEllipse(trp(0,0),trs(kernel::comu_paint::planet.size),trs(kernel::comu_paint::planet.size));
	painter.setPen(QPen(QColor(255,255,255)));
	QFont font=painter.font();
	font.setBold(true);
	font.setPixelSize(14);
	painter.setFont(font);
	painter.drawText(trp(0,0),QString::fromStdString(kernel::comu_paint::planet.health.str()));
	///////////////////////////////////
	painter.setPen(QPen(QColor(146,139,129)));
	painter.setBrush(QColor(146,139,129));
	for(const auto &i:kernel::comu_paint::meteorite_list)
	{
		painter.drawEllipse(trp(i.x,i.y),trs(i.size),trs(i.size));
	}
	////////////////////////
	painter.setPen(QPen(QColor(58,70,216)));
	painter.setBrush(QColor(58,70,216));
	for(const auto &i:kernel::comu_paint::box_list)
	{
		painter.drawEllipse(trp(i.x,i.y),trs(i.size),trs(i.size));
	}
}

//在特定区域绘制游戏地图
void draw_map(QWidget *place,uint16_t state)
{
	QPainter painter(place);
	QPixmap pix;
	pix.load(":/pictures/resources/game_name.png");
	pix.fill();
	int width=place->width(),height=place->height();
	pix=pix.scaled(std::max(width,height),std::max(width,height));
	switch(state)
	{
	case STATE_PLAYING:
	{
		draw_pixmap(&pix,std::max(width,height));
		kernel::comu_paint::ready=false;
		QRect rect;
		if(width>height)
		{
			rect.setRect(0,(width-height)/2,width,height);
		}
		else if(width<height)
		{
			rect.setRect((height-width)/2,0,width,height);
		}
		else
		{
			rect.setRect(0,0,width,height);
		}
		painter.drawPixmap(0,0,pix.copy(rect));
		break;
	}
	case STATE_STOP:
		painter.drawText(100,100,QString::fromUtf8("终止游戏"));
		break;
	case STATE_PAUSE:
	{
		if(!kernel::comu_paint::ready)
			kernel::prepare_data_for_painting();
		draw_pixmap(&pix,std::max(place->width(),place->height()));
		QRect rect;
		if(width>height)
		{
			rect.setRect(0,(width-height)/2,width,height);
		}
		else if(width<height)
		{
			rect.setRect((height-width)/2,0,width,height);
		}
		else
		{
			rect.setRect(0,0,width,height);
		}
		painter.drawPixmap(0,0,pix.copy(rect));
		QFont font=place->font();
		font.setPointSize(24);
		font.setBold(true);
		QPen pen;
		pen.setColor(Qt::red);
		painter.setFont(font);
		painter.setPen(pen);
		painter.drawText(place->width()/2,place->height()/2,QString::fromUtf8("已暂停"));
		break;
	}
	default:std::cerr<<"!!!"<<std::endl;assert(0);
	}

	//std::cout<<width()<<" "<<height()<<std::endl;
}
}
