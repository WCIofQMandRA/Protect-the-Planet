//dialog_difficulty.hpp: 类dialog_difficulty的声明

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
#ifndef DIALOG_DIFFICULTY_H
#define DIALOG_DIFFICULTY_H

#include <QDialog>

namespace Ui {
class dialog_difficulty;
}

class dialog_difficulty : public QDialog
{
	Q_OBJECT

public:
	explicit dialog_difficulty(uint16_t,QWidget *parent = nullptr);
	~dialog_difficulty();

private:
	Ui::dialog_difficulty *ui;

public:
	uint16_t dif;
	bool game_entered;
private slots:
	void on_but_dif0_clicked();
	void on_but_dif1_clicked();
	void on_but_dif2_clicked();
	void on_but_start_clicked();
	void on_but_back_clicked();
};
#endif // DIALOG_DIFFICULTY_H
