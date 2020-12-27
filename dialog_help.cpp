//dialog_help: 类dialog_help的实现

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
#include "dialog_help.h"
#include "ui_dialog_help.h"
#include <QPalette>
dialog_help::dialog_help(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_help)
{
	ui->setupUi(this);
	setFixedSize(width(),height());
#ifdef _WIN32
	QFont font;
	font.setFamily("Microsoft Yahei");
	font.setBold(true);
	setFont(font);
	QPalette pal_label,pal_but;
	pal_but.setColor(QPalette::ButtonText,Qt::white);
	ui->but_sure->setPalette(pal_but);
	ui->but_sure->setStyleSheet("background-color:black");
	//ui->listWidget->setStyleSheet("background-color:black");
#endif
}

dialog_help::~dialog_help()
{
	delete ui;
}

void dialog_help::on_but_sure_clicked()
{
	done(0);
}
