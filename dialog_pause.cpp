//dialog_pause.cpp: 类dialog_pause的实现

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

#include "dialog_pause.hpp"
#include "ui_dialog_pause.h"
#include <QPalette>
#include <iostream>
dialog_pause::dialog_pause(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_pause)
{
	ui->setupUi(this);
#ifdef _WIN32
	QFont font;
	font.setFamily("Microsoft Yahei");
	setFont(font);
#endif
	QPalette pal;
	pal.setColor(QPalette::ButtonText,Qt::white);
	ui->button_stop->setPalette(pal);
	ui->button_stop->setStyleSheet("background-color:rgb(0,0,0)");
	ui->button_continue->setPalette(pal);
	ui->button_continue->setStyleSheet("background-color:rgb(0,0,0)");
}

dialog_pause::~dialog_pause()
{
	delete ui;
}

void dialog_pause::on_button_stop_clicked()
{
	done(1);
}

void dialog_pause::on_button_continue_clicked()
{
	done(0);
}

/*void dialog_pause::on_button_save_clicked()
{
	//todo
	std::cout<<"a";
	ui->success_save->setText("进度已储存！");
}*/
