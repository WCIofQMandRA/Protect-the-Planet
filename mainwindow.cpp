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
//	along with 保卫行星 .
//	If not, see https://www.gnu.org/licenses/.
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>

mainwindow::mainwindow(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::mainwindow)
{
	ui->setupUi(this);
}

mainwindow::~mainwindow()
{
	delete ui;
}

void mainwindow::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	//std::cout<<width()<<" "<<height()<<std::endl;
	painter.drawLine(80*width()/1000,100*height()/750,650*width()/1000,500*height()/750);
	painter.setPen(Qt::red);
	painter.drawRect(10*width()/1000,10*height()/750,100*width()/1000,400*height()/750);
	painter.setBrush(Qt::blue);
	painter.drawEllipse(50*width()/1000,150*height()/750,400*width()/1000,200*height()/750);
}
