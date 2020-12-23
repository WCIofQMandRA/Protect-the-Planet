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
#include <boost/multiprecision/mpfr.hpp>

using namespace boost::multiprecision;
typedef mpz_int intmp_t;
typedef number<mpfr_float_backend<50,allocate_stack>,et_on> floatmp_t;
const double EPS64=1e-10;

inline uint32_t compress16(uint16_t a,uint16_t b)
{
	return static_cast<uint32_t>(a)|(static_cast<uint32_t>(b)<<16);
}

inline uint64_t compress16(uint16_t a,uint16_t b,uint16_t c,uint16_t d)
{
	return static_cast<uint64_t>(a)|(static_cast<uint64_t>(b)<<16)
			|(static_cast<uint64_t>(c)<<32|(static_cast<uint64_t>(d)<<48));
}

inline uint64_t compress32(uint32_t a,uint32_t b)
{
	return static_cast<uint64_t>(a)|(static_cast<uint64_t>(b)<<32);
}
#endif
