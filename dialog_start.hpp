//dialog_start.hpp: 类dialog_start的声明

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
#ifndef DIALOG_START_H
#define DIALOG_START_H

#include <QDialog>
namespace Ui {
class dialog_start;
}

class dialog_start : public QDialog
{
	Q_OBJECT

public:
	explicit dialog_start(QWidget *parent = nullptr);
	std::vector<std::u32string> name_list;
	std::u32string name;
	uint16_t difficulty=0;
	~dialog_start();

private slots:
	void on_button_start_clicked();
	void on_button_exit_clicked();
	void on_but_changeuser_clicked();

	void on_but_ranklist_clicked();

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	QPixmap wel_img;
	QPalette pal_label;
	QPalette pal_but;
private:
	Ui::dialog_start *ui;
};

#endif // DIALOG_START_H
