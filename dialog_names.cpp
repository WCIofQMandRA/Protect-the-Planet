//dialog_names.cpp: 类dialog_names的实现
//用户此前存档名字的选择与新名字的键入

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

#include "dialog_names.hpp"
#include "ui_dialog_names.h"
#include <QListWidgetItem>
#include <QListWidget>
#include <QVBoxLayout>
#include "dialog_newgame.h"
#include "dialog_load.h"
#include "dialog_start.hpp"
dialog_names::dialog_names(QDialog *parent) :
	QDialog(parent),
	ui(new Ui::dialog_names)
{
	ui->setupUi(this);
#ifdef _WIN32
	QFont font;
	font.setFamily("Microsoft Yahei");
	setFont(font);
#endif
	/*QListWidgetItem *nitem1 = new QListWidgetItem(ui->name_list);
	ui->name_list->addItem(nitem1);
	ui->name_list->setItemWidget(nitem1,ui->but_1);
	QListWidgetItem *nitem2 = new QListWidgetItem(ui->name_list);
	ui->name_list->addItem(nitem2);
	ui->name_list->setItemWidget(nitem2,ui->but_2);
	QListWidgetItem *nitem3 = new QListWidgetItem(ui->name_list);
	ui->name_list->addItem(nitem3);
	ui->name_list->setItemWidget(nitem3,ui->but_3);*/
	/*QRadioButton *radiox = new QRadioButton(tr("name-%d"));
	QVBoxLayout *vbox = new QVBoxLayout (this);
	vbox ->addWidget(radiox);
	ui->name_list->setItemWidget(nitem3,radiox);*/
}

dialog_names::~dialog_names()
{
	delete ui;
}
void dialog_names::on_play_new_clicked()
{
	dialog_newgame dia;
	dia.show();
    dia.setWindowTitle("new player");
	dia.exec();
	player_name = dia.new_name;
	player_dif = dia.new_dif;
	done(0);
}

void dialog_names::on_play_last_clicked()
{
	;
	//player_name =
	//player_dif  =
	done(0);
}

void dialog_names::on_play_saved_clicked()
{
	Dialog_load dia;
	dia.show();
	dia.exec();
	//player_name =
	//player_dif =
	done(0);
}

void dialog_names::on_dialog_names_destroyed()
{
    ;
//    dialog_start dia;
//    dia.show();
//    dia.exec();
}
