//dialog_difficulty: 类dialog_difficulty的实现

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
#include "dialog_difficulty.h"
#include "ui_dialog_difficulty.h"
#include "save_load.hpp"
#include "dialog_start.hpp"
#include <QPalette>
dialog_difficulty::dialog_difficulty(uint16_t dif_,QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_difficulty)
{
	ui->setupUi(this);
	setFixedSize(width(),height());
	QFont font;
#ifdef _WIN32
	font.setFamily("Microsoft Yahei");
#endif
	font.setBold(true);
	setFont(font);

	QPalette pal;
	pal.setColor(QPalette::ButtonText,Qt::white);
	ui->but_back->setPalette(pal);
	ui->but_back->setStyleSheet("background-color:rgb(0,0,0)");
	ui->but_start->setPalette(pal);
	ui->but_start->setStyleSheet("background-color:rgb(0,0,0)");
	game_entered=false;
	if(save_load->get_userlist().empty()){
		ui->but_dif0->setChecked(true);
	}else{
	   dif = dif_;
	   switch (dif_)
	   {
	   case 0:ui->but_dif0->setChecked(true);break;
	   case 1:ui->but_dif1->setChecked(true);break;
	   case 2:ui->but_dif2->setChecked(true);break;
	   }
	}
}

dialog_difficulty::~dialog_difficulty()
{
	delete ui;
}

void dialog_difficulty::on_but_dif0_clicked()
{
	dif=0;
}

void dialog_difficulty::on_but_dif1_clicked()
{
	dif=1;
}

void dialog_difficulty::on_but_dif2_clicked()
{
	dif=2;
}

void dialog_difficulty::on_but_start_clicked()
{
	game_entered=true;
	done(0);
}

void dialog_difficulty::on_but_back_clicked()
{
	done(0);
}
