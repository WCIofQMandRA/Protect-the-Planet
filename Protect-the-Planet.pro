#Prtect-the-Planet.pro: 保卫行星游戏的Qt项目

#	-* mode: Qt project file	encoding:UTF-8 *-
#	Copyright 2020 张子辰 & 吕航 (GitHub: WCIofQMandRA & LesterLv)
#
#	This file is part of the game 保卫行星
#
#	This game is free software; you can redistribute it and/or modify it
#	under the terms of the GNU Lesser General Public License as published by
#	the Free Software Foundation; either version 3, or (at your option) any
#	later version.
#
#	This game is distributed in the hope that it will be useful, but
#	WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
#	or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
#	License for more details.
#
#	You should have received copies of the GNU Lesser General Public License
#	and the GNU Gerneral Public License along with 保卫行星 .
#	If not, see https://www.gnu.org/licenses/.

#	作者的英文水平有限，只能用简体中文写注释，部分标识符的名称很生硬
QT		+= core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000	# disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	ako.box.cpp \
	ako.effect.cpp \
	ako.food.cpp \
	ako.meteorite.cpp \
	ako.weapon.cpp \
	dialog_congratulation.cpp \
	dialog_difficulty.cpp \
	dialog_help.cpp \
	dialog_load.cpp \
	dialog_newgame.cpp \
	dialog_pause.cpp \
	dialog_ranklist.cpp \
	dialog_start.cpp \
	file.cpp \
	menu.cpp \
	orbit.cpp \
	paint.cpp \
	main.cpp \
	mainwindow.cpp \
	kernel.cpp \
	save_load.cpp \
	type.cpp

LIBS += -Wl,-Bstatic -lmpfr -lgmp -Wl,-Bdynamic

CONFIG(release,debug|release):DEFINES += NDEBUG

QMAKE_CXXFLAGS += -Wno-deprecated-copy

HEADERS += \
	dialog_congratulation.hpp \
	dialog_difficulty.h \
	dialog_help.h \
	dialog_load.h \
	dialog_newgame.h \
	dialog_pause.hpp \
	dialog_ranklist.h \
	dialog_start.hpp \
	file.hpp \
	kernel.hpp \
	kernel.init.hpp \
	orbit.hpp \
	paint.hpp \
	include.hpp \
	mainwindow.hpp \
	menu.hpp \
	save_load.hpp \
	type.hpp

FORMS += \
	dialog_congratulation.ui \
	dialog_difficulty.ui \
	dialog_help.ui \
	dialog_load.ui \
	dialog_newgame.ui \
	dialog_pause.ui \
	dialog_ranklist.ui \
	dialog_start.ui \
	mainwindow.ui

TRANSLATIONS += \
	Protect-the-Planet_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	resources.qrc \
