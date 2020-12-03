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


mainwindow::mainwindow(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::mainwindow)
	, state(STATE_STOP)
{
	ui->setupUi(this);
	auto [name,difficulty]=menu::show_welcome();
	if(difficulty!=65535)
	{
		state=STATE_PLAYING;
		setMouseTracking(true);
		kernel::start_game(name,difficulty);
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
	std::cout<<"mouseMoveEvent"<<event->x()<<" "<<event->y()<<std::endl;
}

void mainwindow::mousePressEvent(QMouseEvent *event)
{
	if(is_choosing)return;
	//QWidget::mousePressEvent(event);
	std::cout<<"mousePressEvent"<<event->x()<<" "<<event->y()<<std::endl;
}

void mainwindow::mouseReleaseEvent(QMouseEvent *event)
{
	if(is_choosing)return;
	//QWidget::mouseReleaseEvent(event);
	std::cout<<"mouseReleaseEvent"<<event->x()<<" "<<event->y()<<std::endl;
}

void mainwindow::keyPressEvent(QKeyEvent *event)
{
	if(is_choosing)return;
	//QWidget::keyPressEvent(event);
	std::cout<<"keyPressEvent"<<event->key()<<std::endl;
	std::cout<<event->text().toStdString()<<std::endl;
	if(event->key()==Qt::Key::Key_Escape)
	{
		if(state==STATE_PLAYING)
		{
			state=STATE_PAUSE;
			setMouseTracking(false);
			update();
			is_choosing=true;
			if(menu::show_pause())
			{
				state=STATE_STOP;
				is_choosing=false;
				kernel::stop_game();
				update();
				is_choosing=true;
				auto [name,difficulty]=menu::show_welcome();
				is_choosing=false;
				if(difficulty!=65535)
				{
					state=STATE_PLAYING;
					paint::draw_map(this,STATE_PLAYING);
					setMouseTracking(true);
					kernel::start_game(name,difficulty);
				}
				else
				{
					close();
				}
			}
			else
			{
				state=STATE_PLAYING;
				is_choosing=false;
				setMouseTracking(true);
				kernel::continue_game();
			}
		}

	}
}

void mainwindow::keyReleaseEvent(QKeyEvent *event)
{
	if(is_choosing)return;
	//QWidget::keyReleaseEvent(event);
	std::cout<<"keyReleaseEvent"<<event->key()<<std::endl;
	std::cout<<event->text().toStdString()<<std::endl;
}
