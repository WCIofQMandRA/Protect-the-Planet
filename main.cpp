//main.cpp: 主函数的定义

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
#include "mainwindow.hpp"

#include <QApplication>
#include "save_load.hpp"
#include "kernel.hpp"

static void init_program()
{
	kernel::init();
}

static void exit_program(int returnval)
{
	kernel::comu_menu::should_pause=true;
	using namespace std::chrono_literals;
	if(kernel::process_thread.joinable())
		kernel::process_thread.join();
	kernel::clear();
	exit(returnval);
}

int main(int argc, char **argv)
{
	init_program();
	QApplication app(argc, argv);
	mainwindow window;
	window.resize(1024,768);
	window.showMaximized();
	exit_program(app.exec());
	return 0;
}
