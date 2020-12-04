//dialog_pause.hpp: 类dialog_pause的声明和控制模块

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
#ifndef DIALOG_PAUSE_HPP
#define DIALOG_PAUSE_HPP

#include <QDialog>

namespace Ui {
class dialog_pause;
}

class dialog_pause : public QDialog
{
	Q_OBJECT

public:
	explicit dialog_pause(QWidget *parent = nullptr);
	~dialog_pause();

private slots:
	void on_button_stop_clicked();
	void on_button_continue_clicked();

private:
	Ui::dialog_pause *ui;
};

#endif // DIALOG_PAUSE_HPP
