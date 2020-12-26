//mainwindow.cpp: 类mainwindow的实现

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
#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "dialog_start.hpp"
#include "dialog_pause.hpp"
#include "paint.hpp"
#include "menu.hpp"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>

namespace attribute
{
extern double player_max_weapon_direct;
extern double map_size;
extern double player_height;
}//namespace attribute

//由于某种不明原因，std::tread和QTimer不能做mainwindow的成员，否则mainwindow将无法接收到任何事件
QTimer *autoupdate_timer;

mainwindow::mainwindow(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::mainwindow)
	, state(STATE_STOP)
{
	ui->setupUi(this);
#ifdef _WIN32
	QFont font;
	font.setFamily("Microsoft Yahei");
	setFont(font);
#endif
	setFocusPolicy(Qt::StrongFocus);
	auto [name,difficulty]=menu::show_welcome();
	autoupdate_timer=new QTimer(this);
	connect(autoupdate_timer,&QTimer::timeout,this,&mainwindow::auto_repaint);
	connect(autoupdate_timer,&QTimer::timeout,this,&mainwindow::game_end);
	is_choosing=false;
	if(difficulty!=65535)
	{
		state=STATE_PLAYING;
		setMouseTracking(true);
		kernel::comu_menu::game_ended=false;
		kernel::start_game(name,difficulty);
		autoupdate_timer->start(5);
		should_close=false;
	}
	else
	{
		//close()	//无法在构造函数中调用close()
		should_close=true;
	}
}

mainwindow::~mainwindow()
{
	delete ui;
}

void mainwindow::paintEvent(QPaintEvent *event)
{
	if(should_close)close();
	QWidget::paintEvent(event);
	paint::draw_map(this,state);
}

void mainwindow::mouseMoveEvent(QMouseEvent *event)
{
	if(is_choosing)return;
	//QWidget::mouseMoveEvent(event);
	int maxsize=std::min(height(),width());
	auto trs=[maxsize](double s)->int
	{
		return s/attribute::map_size*maxsize;
	};
	int x=width()/2,y=height()/2-trs(attribute::player_height)-trs(kernel::comu_paint::planet.size);
	double theta=atan2(x-event->x(),y-event->y());
	if(theta<0&&theta<-attribute::player_max_weapon_direct)theta=-attribute::player_max_weapon_direct;
	else if(theta>0&&theta>attribute::player_max_weapon_direct)theta=attribute::player_max_weapon_direct;
	kernel::comu_control::weapon_direct=theta;
}

void mainwindow::mousePressEvent(QMouseEvent *event)
{
	if(is_choosing)return;
	//QWidget::mousePressEvent(event);
	if(event->button()==Qt::LeftButton)
	{
		kernel::comu_control::weapon=11;
	}
}

void mainwindow::mouseReleaseEvent(QMouseEvent *event)
{
	if(is_choosing)return;
	//QWidget::mouseReleaseEvent(event);
	if(event->button()==Qt::LeftButton)
	{
		kernel::comu_control::weapon=10;
	}
}

void mainwindow::keyPressEvent(QKeyEvent *event)
{
	if(is_choosing)return;
	switch(event->key())
	{
	case Qt::Key::Key_Escape:
	{
		if(state==STATE_PLAYING)
		{
			state=STATE_PAUSE;
			setMouseTracking(false);
			kernel::comu_menu::should_pause=true;
			autoupdate_timer->stop();
			update();
			is_choosing=true;
			if(menu::show_pause())
			{
				stop_game();
			}
			else
			{
				state=STATE_PLAYING;
				is_choosing=false;
				setMouseTracking(true);
				kernel::continue_game();
				autoupdate_timer->start(5);
			}
		}
		break;
	}
	case Qt::Key::Key_A:kernel::comu_control::move=1;break;
	case Qt::Key::Key_D:kernel::comu_control::move=-1;break;
	case Qt::Key::Key_Z:kernel::comu_control::change_dropped_item=-1;break;
	case Qt::Key::Key_X:kernel::comu_control::change_dropped_item=2;break;
	case Qt::Key::Key_C:kernel::comu_control::change_dropped_item=1;break;
	case Qt::Key::Key_1:kernel::comu_control::weapon=1;break;
	case Qt::Key::Key_2:kernel::comu_control::weapon=2;break;
	case Qt::Key::Key_W:kernel::comu_control::active_effect=4;break;
	case Qt::Key::Key_S:kernel::comu_control::active_effect=3;break;
	case Qt::Key::Key_Q:kernel::comu_control::active_effect=1;break;
	case Qt::Key::Key_E:kernel::comu_control::active_effect=2;break;
	case Qt::Key::Key_F:kernel::comu_control::active_effect=5;break;
	case Qt::Key::Key_F11:if(isFullScreen())showNormal();else showFullScreen();break;
	}
}

void mainwindow::keyReleaseEvent(QKeyEvent *event)
{
	if(is_choosing)return;
	//QWidget::keyReleaseEvent(event);
	switch(event->key())
	{
	case Qt::Key::Key_A:
	case Qt::Key::Key_D:
		kernel::comu_control::move=0;
		break;
	case Qt::Key::Key_Z:
	case Qt::Key::Key_X:
	case Qt::Key::Key_C:
		kernel::comu_control::change_dropped_item=0;
		break;
	case Qt::Key::Key_1:
	case Qt::Key::Key_2:
		kernel::comu_control::weapon=10;
		break;
	case Qt::Key::Key_W:
	case Qt::Key::Key_S:
	case Qt::Key::Key_Q:
	case Qt::Key::Key_E:
	case Qt::Key::Key_F:
		kernel::comu_control::active_effect=0;
		break;
	}
}

void mainwindow::auto_repaint()
{
	if(state==STATE_PLAYING&&kernel::comu_paint::ready)
	{
		update();
	}
}

void mainwindow::stop_game()
{
	state=STATE_STOP;
	is_choosing=false;
	kernel::stop_game();
	autoupdate_timer->stop();
	hide();
	is_choosing=true;
	auto [name,difficulty]=menu::show_welcome();
	is_choosing=false;
	if(difficulty!=65535)
	{
		state=STATE_PLAYING;
		show();
		//paint::draw_map(this,STATE_PLAYING);
		setMouseTracking(true);
		kernel::comu_menu::game_ended=false;
		kernel::start_game(name,difficulty);
		autoupdate_timer->start(5);
	}
	else
	{
		close();
	}
}

void mainwindow::game_end()
{
	if(kernel::comu_menu::game_ended)
	{
		state=STATE_PAUSE;
		setMouseTracking(false);
		autoupdate_timer->stop();
		update();
		stop_game();
	}
}

void mainwindow::wheelEvent(QWheelEvent *event)
{
	if(event->delta()>0)
	{
		kernel::comu_control::weapon=3;
	}
	else
	{
		kernel::comu_control::weapon=4;
	}
}
