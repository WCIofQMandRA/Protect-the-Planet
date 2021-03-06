//dialog_ranklist.hpp: 类dialog_ranklist的声明

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
#ifndef DIALOG_RANKLIST_H
#define DIALOG_RANKLIST_H

#include <QDialog>

namespace Ui {
class dialog_ranklist;
}

class dialog_ranklist : public QDialog
{
	Q_OBJECT

public:
	explicit dialog_ranklist(QWidget *parent = nullptr);
	~dialog_ranklist();

private slots:
	void on_but_back_clicked();

	void on_but_easy_clicked();

	void on_but_normal_clicked();

	void on_but_hard_clicked();

private:
	Ui::dialog_ranklist *ui;

public:
	std::vector<std::tuple<std::u32string,uint64_t,uint64_t>> ranklist;
};

#endif // DIALOG_RANKLIST_H
