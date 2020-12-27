//dialog_help.hpp: 类dialog_help的声明

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
#ifndef DIALOG_HELP_H
#define DIALOG_HELP_H

#include <QDialog>

namespace Ui {
class dialog_help;
}

class dialog_help : public QDialog
{
	Q_OBJECT

public:
	explicit dialog_help(QWidget *parent = nullptr);
	~dialog_help();

private slots:
	void on_but_sure_clicked();

private:
	Ui::dialog_help *ui;
};

#endif // DIALOG_HELP_H
