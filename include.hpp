//include.hpp 常在项目的个文件中使用的外部依赖项和基本的类型重定义

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

#ifndef INCLUDE_H
#define INCLUDE_H
#ifndef QT_QML_DEBUG
#define NDEBUG
#endif
#include <iostream>
#include <vector>
#include <array>
#include <utility>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <variant>
#include <cstdint>
#include <functional>
#include <string>
#include <cassert>
#include <boost/multiprecision/gmp.hpp>

using namespace boost::multiprecision;
typedef mpz_int intmp_t;
const double EPS64=1e-10;
#endif
