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
#include <cmath>

namespace attribute
{
//地图属性
extern double map_size;
extern double player_height;
}//namespace attribute

namespace paint
{
std::vector<QPixmap> meteorite_resources;
std::vector<QPixmap> player_resources;
std::vector<QPixmap> food_resources;
std::vector<QPixmap> weapon_resources;
std::vector<QPixmap> effect_resources;
std::vector<size_t> meteorite_view={0,0,0,0,0,0,0};
std::vector<size_t> effect_view={0,0,0,0,0,1,1,1};
std::vector<std::u32string> food_namelist={U"糖果"};
std::vector<std::u32string> weapon_namelist={U"手枪"};
std::vector<std::u32string> effect_namelist={U"快速射击I"};
uint64_t animate;
bool inited=false;

void drawText(QPainter &painter,qreal x,qreal y,Qt::Alignment flags,const QString &text,QRectF *boundingRect=0)
{
	const qreal size=32767.0;
	QPointF corner(x,y-size);
	if(flags&Qt::AlignHCenter)corner.rx()-=size/2.0;
	else if(flags&Qt::AlignRight)corner.rx()-=size;
	if(flags&Qt::AlignVCenter)corner.ry()+=size/2.0;
	else if(flags&Qt::AlignTop)corner.ry()+=size;
	else flags|=Qt::AlignBottom;
	QRectF rect{corner.x(),corner.y(),size,size};
	painter.drawText(rect,flags,text,boundingRect);
}

//QPixmap: Must construct a QGuiApplication before a QPixmap
void init()
{
	meteorite_resources.resize(1);
	meteorite_resources[0].load(":/pictures/resources/meteorite0.png");
	player_resources.resize(3);
	player_resources[0].load(":/pictures/resources/player0.png");
	player_resources[1].load(":/pictures/resources/player1.png");
	player_resources[2].load(":/pictures/resources/player2.png");
	weapon_resources.resize(1);
	weapon_resources[0].load(":/pictures/resources/weapon0.png");
	effect_resources.resize(2);
	effect_resources[0].load(":/pictures/resources/effect0.png");
	effect_resources[1].load(":/pictures/resources/effect1.png");
}

void draw_pixmap(QPixmap *pix,int maxsize,int deltax,int deltay)
{
	using namespace kernel::comu_paint;
	if(!inited)init();
	double msize=attribute::map_size;
	auto trp=[maxsize,msize,deltax,deltay](double x,double y)->QPoint
	{
		double cos_theta=cos(M_PI_2-player.position);
		double sin_theta=sin(M_PI_2-player.position);
		double x_=x*cos_theta-y*sin_theta,y_=x*sin_theta+y*cos_theta;
		return QPoint(static_cast<int>(maxsize*(0.5+x_/msize))+deltax,static_cast<int>(maxsize*(0.5-y_/msize))+deltay);
	};
	auto trs=[maxsize,msize](double s)->int
	{
		return s/msize*maxsize;
	};
	QPainter painter(pix);
	QFont font=painter.font();
#ifdef _WIN32
	font.setFamily("Microsoft Yahei");
	painter.setFont(font);
#endif
	int width=pix->width(),height=pix->height();
	////////////////////////////////////
	//绘制行星
	painter.setPen(QPen(QColor(128,64,0)));
	painter.setBrush(QColor(128,64,0));
	painter.drawEllipse(trp(0,0),trs(planet.size),trs(planet.size));
	///////////////////////////////////
	//绘制陨石
	painter.setPen(QPen(QColor(146,139,129)));
	painter.setBrush(QColor(146,139,129));
	for(const auto &i:meteorite_list)
	{
		//painter.drawEllipse(trp(i.x,i.y),trs(i.size),trs(i.size));
		auto tmp=meteorite_resources[meteorite_view[i.type]].scaled(trs(i.size)*2,trs(i.size)*2);
		auto point=trp(i.x,i.y);
		painter.drawPixmap(point.x()-tmp.width()/2,point.y()-tmp.height()/2,tmp);
	}
	////////////////////////
	//绘制补给箱
	painter.setPen(QPen(QColor(58,70,216)));
	painter.setBrush(QColor(58,70,216));
	for(const auto &i:box_list)
	{
		painter.drawEllipse(trp(i.x,i.y),trs(i.size),trs(i.size));
	}
	////////////////////////
	//绘制掉落的补给箱
	painter.setPen(QPen(QColor(58,70,128)));
	painter.setBrush(QColor(58,70,128));
	for(const auto &i:dropped_box_list)
	{
		painter.drawEllipse(trp(i.x,i.y),trs(i.size),trs(i.size));
	}
	//////////////////////
	//绘制玩家
	painter.setPen(QPen(Qt::black));
	painter.setBrush(Qt::black);
	QPoint ptmp=trp(0,0);
	int which=(animate++)%9/3;
	int ysize=trs(attribute::player_height)*3/2;
	int xsize=player_resources[which].size().width()*ysize/player_resources[which].size().height();
	painter.drawPixmap(ptmp.x()-xsize/2,ptmp.y()-trs(planet.size)-trs(attribute::player_height),player_resources[which].scaled(xsize,ysize));
	////////////////////
	//绘制子弹
	painter.setPen(QPen(Qt::red,trs(1e6)*0.6));
	painter.setBrush(Qt::yellow);
	for(const auto &i:pill_list)
	{
		painter.drawEllipse(trp(i.x,i.y),trs(1e6),trs(1e6));
	}
	///////////////////
	//绘制选择补给箱内物品的界面
	if(dropped_item.first!=0xFFFFFFFF)
	{
		painter.setPen(QColor(88,88,88));
		painter.setBrush(QColor(127,127,127));
		int icon_size=std::min(64,trs(1.5e7));
		painter.drawRect(ptmp.x()-icon_size/2,ptmp.y()-icon_size/2,icon_size,icon_size);
		QString name;
		auto &item=dropped_item;
		switch(item.first&0xFFFF)
		{
		case CONTAIN_TYPE_FOOD:
		{
			//TODO：绘制图标
			name=QString::fromUtf8("%1 ×%2").arg(QString::fromStdU32String(food_namelist[item.first>>16])).arg(item.second);
			break;
		}
		case CONTAIN_TYPE_PILL:
		{
			//TODO：绘制图标
			name=QString::fromUtf8("子弹 ×%1").arg(item.second);
			break;
		}
		case CONTAIN_TYPE_WEAPON:
		{
			painter.drawPixmap(ptmp.x()-icon_size/2,ptmp.y()-icon_size/2,weapon_resources[item.first>>16].scaled(icon_size,icon_size));
			name=QString::fromUtf8("%1 ×%2").arg(QString::fromStdU32String(weapon_namelist[item.first>>16])).arg(item.second);
			break;
		}
		case CONTAIN_TYPE_EFFECT:
		{
			painter.drawPixmap(ptmp.x()-icon_size/2,ptmp.y()-icon_size/2,effect_resources[effect_view[item.first>>16]].scaled(icon_size,icon_size));
			name=QString::fromUtf8("%1 ×%2").arg(QString::fromStdU32String(effect_namelist[item.first>>16])).arg(item.second);
			break;
		}
		}
		painter.setPen(Qt::white);
		font.setBold(true);
		painter.setFont(font);
		drawText(painter,ptmp.x(),ptmp.y()+66,Qt::AlignTop|Qt::AlignHCenter,name);
	}
	///////////////
	//绘制状态栏
	font.setPointSize(12);
	font.setBold(false);
	painter.setFont(font);
	painter.setPen(Qt::white);
	drawText(painter,10,height-50,Qt::AlignLeft|Qt::AlignBottom,QString::fromUtf8("行星完整度：%1").arg(QString::fromStdString(planet.health.str())));
	drawText(painter,10,height-30,Qt::AlignLeft|Qt::AlignBottom,QString::fromUtf8("剩余子弹：%1").arg(player.pills));
	drawText(painter,10,height-10,Qt::AlignLeft|Qt::AlignBottom,QString::fromUtf8("饥饿值：%1").arg(player.hunger));
	////////////////
	//绘制计分板
	drawText(painter,width-10,30,Qt::AlignRight|Qt::AlignTop,QString::fromUtf8("第 %1 关").arg(level));
	drawText(painter,width-10,50,Qt::AlignRight|Qt::AlignTop,QString::fromUtf8("得分：%1").arg(score));
	painter.setPen(QColor(90,200,255));
	drawText(painter,width-10,10,Qt::AlignRight|Qt::AlignTop,QString::fromStdU32String(player.name));
	//绘制物品栏
	{
		int icon_size=std::min(64,trs(1.5e7));
		//效果
		{
			painter.setPen(QColor(88,88,88));
			painter.setBrush(QColor(127,127,127));
			int i=player.chosen_effect&0xFFFF;
			for(int j=0;j<5;++j)
			{
				painter.drawRect(10,10+j*(icon_size+10),icon_size,icon_size);
				if(auto it=player.effect.find(compress16(i,j));it!=player.effect.end())
				{
					painter.drawPixmap(10,10+j*(icon_size+10),effect_resources[effect_view[it->second]].scaled(icon_size,icon_size));
					auto old_pen=painter.pen();
					painter.setPen(Qt::white);
					drawText(painter,12+icon_size,10+j*(icon_size+10)+icon_size/2,Qt::AlignLeft|Qt::AlignVCenter,
							 QString::fromStdU32String(effect_namelist[it->second]));
					painter.setPen(old_pen);
				}
				if(j==static_cast<int>(player.chosen_effect>>16))
				{
					auto old_pen=painter.pen();
					QPen pen;
					pen.setColor(QColor(0,190,0));
					pen.setWidth(4);
					painter.setPen(pen);
					int x0=10,y0=10+j*(icon_size+10);
					painter.drawLine(x0,y0,x0+icon_size,y0);
					painter.drawLine(x0+icon_size,y0,x0+icon_size,y0+icon_size);
					painter.drawLine(x0+icon_size,y0+icon_size,x0,y0+icon_size);
					painter.drawLine(x0,y0+icon_size,x0,y0);
					painter.setPen(old_pen);
				}
			}
			painter.setPen(Qt::white);
			drawText(painter,10+icon_size/2,10+5*(icon_size+10)+2,Qt::AlignTop|Qt::AlignHCenter,QString::fromUtf8("[%1]").arg(i));
		}
		//武器
		{
			painter.setPen(QColor(88,88,88));
			painter.setBrush(QColor(127,127,127));
			for(int i=0;i<5;++i)
			{
				painter.drawRect(width-(10+icon_size)*2,height-(10+icon_size)*(5-i),icon_size,icon_size);
				if(uint16_t wtype=player.weapon[i].type;wtype!=65535)
				{
					painter.drawPixmap(width-(10+icon_size)*2,height-(10+icon_size)*(5-i),weapon_resources[wtype].scaled(icon_size,icon_size));
				}
				if(i==player.chosen_weapon)
				{
					auto old_pen=painter.pen();
					QPen pen;
					pen.setColor(QColor(0,190,0));
					pen.setWidth(4);
					painter.setPen(pen);
					int x0=width-(10+icon_size)*2,y0=height-(10+icon_size)*(5-i);
					painter.drawLine(x0,y0,x0+icon_size,y0);
					painter.drawLine(x0+icon_size,y0,x0+icon_size,y0+icon_size);
					painter.drawLine(x0+icon_size,y0+icon_size,x0,y0+icon_size);
					painter.drawLine(x0,y0+icon_size,x0,y0);
					painter.setPen(old_pen);
				}
			}
			for(int i=5;i<10;++i)
			{
				painter.drawRect(width-(10+icon_size),height-(10+icon_size)*(10-i),icon_size,icon_size);
				if(uint16_t wtype=player.weapon[i].type;wtype!=65535)
				{
					painter.drawPixmap(width-(10+icon_size),height-(10+icon_size)*(10-i),weapon_resources[wtype].scaled(icon_size,icon_size));
				}
				if(i==player.chosen_weapon)
				{
					auto old_pen=painter.pen();
					QPen pen;
					pen.setColor(QColor(0,190,0));
					pen.setWidth(4);
					painter.setPen(pen);
					int x0=width-(10+icon_size),y0=height-(10+icon_size)*(10-i);
					painter.drawLine(x0,y0,x0+icon_size,y0);
					painter.drawLine(x0+icon_size,y0,x0+icon_size,y0+icon_size);
					painter.drawLine(x0+icon_size,y0+icon_size,x0,y0+icon_size);
					painter.drawLine(x0,y0+icon_size,x0,y0);
					painter.setPen(old_pen);
				}
			}
			if(auto weapon_id=player.weapon[player.chosen_weapon].type;weapon_id!=65535)
			{
				painter.setPen(Qt::white);
				drawText(painter,width-icon_size-5,height-5*(icon_size+10)-2,Qt::AlignBottom|Qt::AlignHCenter,
						 QString::fromStdU32String(weapon_namelist[weapon_id]));
			}
		}
	}
}

//在特定区域绘制游戏地图
void draw_map(QWidget *place,uint16_t state)
{
	QPainter painter(place);
	QPixmap pix;
	pix.load(":/pictures/resources/background.png");
	//pix.fill();
	int width=place->width(),height=place->height();
	pix=pix.scaled(width,height);
	switch(state)
	{
	case STATE_PLAYING:
	{
		kernel::comu_paint::ready=false;
		if(width>height)
		{
			draw_pixmap(&pix,height,(width-height)/2,0);
		}
		else if(width<height)
		{
			draw_pixmap(&pix,width,0,(height-width)/2);
		}
		else
		{
			draw_pixmap(&pix,height,0,0);
		}
		painter.drawPixmap(0,0,pix);
		break;
	}
	case STATE_STOP:
		//painter.drawText(100,100,QString::fromUtf8("终止游戏"));
		break;
	case STATE_PAUSE:
	{
		if(!kernel::comu_paint::ready)
			kernel::prepare_data_for_painting();
		if(width>height)
		{
			draw_pixmap(&pix,height,(width-height)/2,0);
		}
		else if(width<height)
		{
			draw_pixmap(&pix,width,0,(height-width)/2);
		}
		else
		{
			draw_pixmap(&pix,height,0,0);
		}
		painter.drawPixmap(0,0,pix);
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
