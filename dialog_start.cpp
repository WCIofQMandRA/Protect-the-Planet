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
#include <QPixmap>
#include <QImage>
#include "file.hpp"
#include "dialog_names.hpp"
#include "save_load.hpp"
#include "dialog_newgame.h"
#include "dialog_difficulty.h"
#include "dialog_load.h"
dialog_start::dialog_start(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_start)
{
	ui->setupUi(this);
#ifdef _WIN32
	QFont font;
	font.setFamily("Microsoft Yahei");
	setFont(font);
#endif
	wel_img.load(":/pictures/resources/game_name.png");
	ui->label1->setPixmap(wel_img.scaled(ui->label1->size(),Qt::KeepAspectRatio));
	//ui->button_diff1->setChecked(true);
	//font.setPointSize(12);
	//ui->label_dif->setFont(font);
	if(name_list=save_load.get_userlist();name_list.empty()){
		ui->label_static->setText("欢迎您！ 用户");
		dialog_newgame dia;
		dia.setWindowTitle("New Player");
		dia.show();
		dia.exec();
		name=dia.new_name;
		save_load.add_user(name);
		ui->label_lastname->setText(QString::fromStdU32String(name));
	}
	else
	{
		ui->label_static->setText("欢迎回来 用户");
		ui->label_lastname->setText(QString::fromStdU32String(name_list[0]));
	}
}

dialog_start::~dialog_start()
{
	delete ui;
}

void dialog_start::paintEvent(QPaintEvent *event)
{
	QDialog::paintEvent(event);
}

void dialog_start::on_button_start_clicked()
{
	//name=ui->editor_player_name->toPlainText().toStdU32String();//TODO!!!
	//difficulty=static_cast<uint16_t>(ui->diffculty->checkedId());
	dialog_difficulty dia(save_load.get_last_diff(name));
	dia.setWindowTitle("Difficulty");
	dia.show();
	dia.exec();
	difficulty=dia.dif;
	//ui->chosed_name ->setText(QString::fromStdU32String(name));
	//QFont ft; ft.setPixelSize(30);
	//ui->chosed_name->setFont(ft);
	//ui->button_user->setText("您好用户");
	done(1);
}

void dialog_start::on_button_exit_clicked()
{
	done(0);
}

void dialog_start::on_but_changeuser_clicked()
{
	Dialog_load dia;
	dia.setWindowTitle("Change Player");
	dia.show();
	dia.exec();
	name = dia.name;
	ui->label_lastname->setText(QString::fromStdU32String(name));
}
