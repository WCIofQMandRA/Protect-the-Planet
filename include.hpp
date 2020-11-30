//include.h 外部依赖项和基本的类型重定义

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
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <variant>
#include <cstdint>
#include <functional>
#include <stack>
#include <queue>
#include <string>
#include <thread>
#include <memory>
#include <random>
#include <atomic>
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/mpfr.hpp>
#include <boost/multiprecision/float128.hpp>
#include <boost/math/constants/constants.hpp>

using namespace boost::multiprecision;
typedef mpz_int intmp_t;
typedef mpfr_float_50 floatmp_t;
typedef float float32_t;
typedef double float64_t;
typedef float128 float128_t;
#endif
