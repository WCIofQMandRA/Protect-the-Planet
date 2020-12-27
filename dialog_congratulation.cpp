//dialog_congratulation: 类dialog_congratulation的实现

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
#include "dialog_congratulation.hpp"
#include "ui_dialog_congratulation.h"
#include "paint.hpp"
#include <iostream>

dialog_congratulation::dialog_congratulation(const std::array<std::pair<uint32_t,uint64_t>,3> &choices_,QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_congratulation),
	choices(choices_)
{
	ui->setupUi(this);
	QFont font;
#ifdef _WIN32
	font.setFamily("Microsoft Yahei");
#endif
	font.setBold(true);
	setFont(font);
	ui->button_exit->setHidden(true);
	ui->button_next->setHidden(true);
	ui->label_c0->setPixmap(paint::get_image_resource(choices[0].first).scaled(64,64));
	ui->label_c1->setPixmap(paint::get_image_resource(choices[1].first).scaled(64,64));
	ui->label_c2->setPixmap(paint::get_image_resource(choices[2].first).scaled(64,64));
	ui->label_c0->installEventFilter(this);
	ui->label_c1->installEventFilter(this);
	ui->label_c2->installEventFilter(this);
	selected=0xFFFFFFFF;
}

dialog_congratulation::~dialog_congratulation()
{
	delete ui;
}

bool dialog_congratulation::eventFilter(QObject *obj, QEvent *event)
{
	auto f=[&](int x)
	{
		if(event->type()==QEvent::MouseButtonPress)
		{
			ui->button_exit->setHidden(false);
			ui->button_next->setHidden(false);
			selected=x;
			if(choices[x].second==1)
				ui->label_head->setText(QString::fromStdU32String(paint::get_name(choices[x].first)));
			else
				ui->label_head->setText(QString("%1 ×%2").arg(QString::fromStdU32String(paint::get_name(choices[x].first))).arg(choices[x].second));
			return true;
		}
		else
			return false;
	};
	if(obj==ui->label_c0)return f(0);
	else if(obj==ui->label_c1)return f(1);
	else if(obj==ui->label_c2)return f(2);
	else return QDialog::eventFilter(obj,event);
}

void dialog_congratulation::on_button_exit_clicked()
{
	done(selected<<1);
}

void dialog_congratulation::on_button_next_clicked()
{
	done(selected<<1|1);
}
