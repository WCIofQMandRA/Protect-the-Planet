//kernel/weapon.h: 只能被kernel.h包含

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

//武器
struct weapon_t
{
	uint64_t value;//武器的价值
	uint16_t type;//武器类型
	//use(const intmp_t &x)
	//对某个剩余强度为x的陨石成功使用一次该武器后，该陨石新的剩余强度
	std::function<void(intmp_t&)> use;
	QString name;//武器的名称
	//子弹的速率，速率为inf的子弹可瞬间击中目标，即在下一次跟新场景时，清楚子弹前进方向上的第一个目标
	floatmp_t speed;
};