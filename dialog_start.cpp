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
#include "save_load.hpp"
#include "dialog_newgame.h"
#include "dialog_difficulty.h"
#include "dialog_load.h"
#include "QPalette"
dialog_start::dialog_start(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_start)
{
	ui->setupUi(this);
	setFixedSize(width(),height());
#ifdef _WIN32
	QFont font;
	font.setFamily("Microsoft Yahei");
	font.setBold(true);
	//font.setPointSize(15);
	setFont(font);
#endif
	pal_label.setColor(QPalette::WindowText,Qt::gray);
	//pal_label.setColor(QPalette::Background,Qt::black);
	//pal_but.setColor(QPalette::Border,Qt::white);
	//pal_but.setColor(QPalette::ButtonText,Qt::white);
	//pal_but.setColor(QPalette::Background,Qt::black);
	ui->but_changeuser->setPalette(pal_but);
	//ui->but_changeuser->setStyleSheet("background-color:rgb(0,0,0)");
	//wel_img.load(":/pictures/resources/title_bk.png");
	//ui->label1->setScaledContents(true);
	//ui->label1->setPixmap(wel_img.scaled(ui->label1->size(),Qt::KeepAspectRatio));
	//ui->label1->setPixmap(wel_img);
	//ui->label1->resize(wel_img.width(),wel_img.height());
	//ui->button_diff1->setChecked(true);
	//font.setPointSize(12);
	//ui->label_dif->setFont(font);
	if(name_list=save_load.get_userlist();name_list.empty()){
		//ui->label_static->setText("欢迎您！ 用户");
		dialog_newgame dia;
		dia.setWindowTitle("New Player");
		dia.show();
		dia.exec();
		name=dia.new_name;
		save_load.add_user(name);
		//ui->label_lastname->setText(ui->label_static->text()+QString::fromStdU32String(name));
		QFont ft=this->font();ft.setPointSize(15);ui->label_lastname->setFont(ft);
		ui->label_lastname->setText("用户  "+QString::fromStdU32String(name)+"  欢迎您！");
		//ui->label_lastname->setAutoFillBackground(true);
		ui->label_lastname->setPalette(pal_label);
	}
	else
	{
		QFont ft=this->font();ft.setPointSize(15);ui->label_lastname->setFont(ft);
		ui->label_lastname->setText("用户  "+QString::fromStdU32String(name_list[0])+"  欢迎您！");
		//ui->label_lastname->setAutoFillBackground(true);
		ui->label_lastname->setPalette(pal_label);
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
	name = save_load.get_userlist()[0];
	dialog_difficulty dia(save_load.get_last_diff(name));
	dia.setWindowTitle("Difficulty");
	dia.show();
	dia.exec();
	if(dia.game_entered==true){
	difficulty=dia.dif;
	done(1);
	}
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
	QFont ft=this->font();ft.setPointSize(15);ui->label_lastname->setFont(ft);
	ui->label_lastname->setText("用户  "+QString::fromStdU32String(name)+"  欢迎您！");
	ui->label_lastname->setPalette(pal_label);
}
