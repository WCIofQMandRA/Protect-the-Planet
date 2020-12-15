//dialog_names.hpp

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

#ifndef NAMES_H
#define NAMES_H

#include <QDialog>

namespace Ui {
class dialog_names;
}

class dialog_names : public QDialog
{
	Q_OBJECT

public:
	explicit dialog_names(QDialog *parent = nullptr);
	~dialog_names();
	std::u32string player_name;

private slots:
	void on_but_1_clicked();

	void on_lineEdit_editingFinished();

//	void on_listWidget_clicked(const QModelIndex &index);

private:
	Ui::dialog_names *ui;
};

#endif // NAMES_H
