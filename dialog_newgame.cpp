//dialog_newgame: 类dialog_newgame的实现

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
#include "dialog_newgame.h"
#include "ui_dialog_newgame.h"
#include "save_load.hpp"
#include "dialog_start.hpp"
#include "dialog_help.h"
#include <QPalette>
dialog_newgame::dialog_newgame(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_newgame)
{
	ui->setupUi(this);
	setFixedSize(width(),height());
#ifdef _WIN32
	QFont font;
	font.setFamily("Microsoft Yahei");
	font.setBold(true);
	setFont(font);
#endif
	if(save_load->get_userlist().empty()) ui->but_back->setDisabled(true);
	else ui->but_back->setEnabled(true);
	clicked_back=false;
	QPalette pal;
	pal.setColor(QPalette::ButtonText,Qt::white);
	ui->but_back->setPalette(pal);
	ui->but_back->setStyleSheet("background-color:rgb(0,0,0)");
	ui->but_sure->setPalette(pal);
	ui->but_sure->setStyleSheet("background-color:rgb(0,0,0)");
	ui->but_help->setPalette(pal);
	ui->but_help->setStyleSheet("background-color:rgb(0,0,0)");
}

dialog_newgame::~dialog_newgame()
{
	delete ui;
}

void dialog_newgame::on_nameedit_textEdited(const QString &)
{
	new_name = ui->nameedit->text().toStdU32String();
}

void dialog_newgame::on_but_sure_clicked()
{
	if(ui->nameedit->text().isEmpty()) ;
	else done(0);
}

void dialog_newgame::on_but_back_clicked()
{
	clicked_back=true;
	done(0);
}

void dialog_newgame::on_but_help_clicked()
{
	dialog_help dia;
	dia.show();
	dia.exec();
}

void dialog_newgame::on_nameedit_textChanged(const QString &)
{
	if(ui->nameedit->text().isEmpty()==false) ui->but_sure->setText("确定");
	else ui->but_sure->setText("用户名不能为空");
}
