//dialog_congratulation.hpp: 类dialog_congratulation的声明

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
#ifndef DIALOG_CONGRATULATION_HPP
#define DIALOG_CONGRATULATION_HPP

#include <QDialog>

namespace Ui {
class dialog_congratulation;
}

class dialog_congratulation : public QDialog
{
	Q_OBJECT

public:
	explicit dialog_congratulation(QWidget *parent = nullptr);
	~dialog_congratulation();

private:
	Ui::dialog_congratulation *ui;
};

#endif // DIALOG_CONGRATULATION_HPP
