//kernel/planet.h: 只能被kernel.h包含

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

//行星
struct planet_t
{
	intmp_t health;//行星的完整程度
	floatmp_t size;//行星的大小
	//特殊状态，tuple<剩余时间,等级,剩余强度>
	std::map<uint16_t,std::tuple<uint64_t,std::variant<uint64_t,floatmp_t>,floatmp_t>> effect;
	floatmp_t m_mul_G;//质量×万有引力常数
};