//dialog_load.hpp: 类dialog_load的声明

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
#ifndef DIALOG_LOAD_H
#define DIALOG_LOAD_H

#include <QDialog>

namespace Ui {
class Dialog_load;
}

class Dialog_load : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog_load(QWidget *parent = nullptr);
	~Dialog_load();

private slots:
	void on_but_sure_clicked();

	void on_but_up_clicked();

	void on_but_down_clicked();

	void on_but_1_clicked();

	void on_but_2_clicked();

	void on_but_3_clicked();

	void on_but_4_clicked();

	void on_but_5_clicked();

	void on_but_6_clicked();

	void on_but_back_clicked();

	void on_but_create_clicked();

private:
	Ui::Dialog_load *ui;

public:
	std::u32string name;
	std::vector<std::u32string> name_list;

private:
	int16_t max_down_num;
	int16_t cur_down_num;
	int16_t tot_user_num;
	std::u32string prename;
	QPalette pal_label;
	QPalette pal_but;
};

#endif // DIALOG_LOAD_H
