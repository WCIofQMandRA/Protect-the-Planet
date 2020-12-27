//dialog_newgame.hpp: 类dialog_newgame的声明

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
#ifndef DIALOG_NEWGAME_H
#define DIALOG_NEWGAME_H

#include <QDialog>

namespace Ui {
class dialog_newgame;
}

class dialog_newgame : public QDialog
{
	Q_OBJECT

public:
	explicit dialog_newgame(QWidget *parent = nullptr);
	~dialog_newgame();

private:
	Ui::dialog_newgame *ui;
public:
	std::u32string new_name;
	int new_dif;
	bool clicked_back;
private slots:
	void on_nameedit_textEdited(const QString &arg1);
	void on_but_sure_clicked();
	void on_but_back_clicked();
	void on_but_help_clicked();
	void on_nameedit_textChanged(const QString &arg1);
};
#endif // DIALOG_NEWGAME_H
