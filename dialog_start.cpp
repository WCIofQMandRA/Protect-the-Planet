//dialog_start.cpp: 类dialog_start的实现

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

#include "dialog_start.hpp"
#include "ui_dialog_start.h"

dialog_start::dialog_start(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_start)
{
	ui->setupUi(this);
}

dialog_start::~dialog_start()
{
	delete ui;
}

void dialog_start::on_button_start_clicked()
{
	name=ui->editor_player_name->toPlainText().toStdU32String();
	difficulty=1;//TODO!!!
	//difficulty=static_cast<uint16_t>(ui->diffculty->checkedId());
	done(1);
}

void dialog_start::on_button_exit_clicked()
{
	done(0);
}
