//kernel.h: 游戏内核的相关函数和类的声明

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
//	along with 保卫行星 .
//	If not, see https://www.gnu.org/licenses/.

/*
关于数据类型的使用：
整数：
uint16_t uint64_t
int16_t int64_t intmp_t

浮点数
floatmp_t

关于时间:
50ms为1游戏刻（token，缩写tk）

关于位置：
以“米”为单位

数据类型 单位：
时间：uint64_t tk
速度：floatmp_t m/tk
位矢：floatmp_t m
质量×G：floatmp_t m^3/tk^2
计数器：uint64_t
种类：uint16_t

行星的大小为0.8~1.5×10^7m
地图的大小为4×10^8m×4*10^8m，只渲染2×10^8m×2*10^8m的区域，渲染时的像素数由渲染模块确定
陨石/补给箱的大小为1~10×10^6m

*/
#ifndef KERNEL_H
#define KERNEL_H
#include "include.h"

//未触发的效果在触发时的接受者
#define EFFECT_RECIVER_PLAYER 1//玩家
#define EFFECT_RECIVER_BOX 2//全体未落下的补给箱
#define EFFECT_RECIVER_CURRENT_WEAPON 3//玩家当前使用的武器
#define EFFECT_RECIVER_ALL_WEAPONS 4//玩家持有的所有武器
#define EFFECT_RECIVER_METE 5//全体陨石

//补给箱内容物的种类
#define CONTAIN_TYPE_WEAPON 1//武器
#define CONTAIN_TYPE_FOOD 2//食物
#define CONTAIN_TYPE_PILL 3//子弹
#define CONTAIN_TYPE_EFFECT 4//效果

#include "kernel/weapon.h"
#include "kernel/food.h"
#include "kernel/effect.h"
#include "kernel/meteorite.h"
#include "kernel/box.h"
#include "kernel/player.h"

#include "kernel/default_attribute.h"

#endif // KERNEL_H
