#Prtect-the-Planet.pro: 保卫行星游戏的Qt项目

#   -* mode: Qt project file	encoding:UTF-8 *-
#   Copyright 2020 张子辰 & 吕航 (GitHub: WCIofQMandRA & LesterLv)
#
#   This file is part of the game 保卫行星
#
#   This game is free software; you can redistribute it and/or modify it
#   under the terms of the GNU Lesser General Public License as published by
#   the Free Software Foundation; either version 3, or (at your option) any
#   later version.
#
#   This game is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
#   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
#   License for more details.
#
#	You should have received copies of the GNU Lesser General Public License
#	and the GNU Gerneral Public License along with 保卫行星 .
#	If not, see https://www.gnu.org/licenses/.

#	作者的英文水平有限，只能用简体中文写注释，部分标识符的名称很生硬
QT       += core gui

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
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    paint.cpp \
    main.cpp \
    mainwindow.cpp \
	kernel.cpp

LIBS += -lmpfr -lgmp

HEADERS += \
    kernel.hpp \
    paint.hpp \
    include.hpp \
    mainwindow.hpp \
    menu.hpp

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Protect-the-Planet_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
