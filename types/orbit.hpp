//types/orbit.hpp: 只能被meteorite.hpp或box.hpp包含

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

//参见doc/轨道生成/orbit.pdf
//r=\frac{r_0}{1+\epsilon\cos(\theat-\theta_0)}
//行星近日点为(\frac{r_0}{1+\epsilon},\theta_0)
//epsilon 为轨道离心率
//theat_0 为行星近日点的极角
//v_max 为行星的最大运行速率
//h 为行星的掠面速度的2倍
//相关公式：
//r_m=\frac{r_0}{1+\epsilon}
//v_m=\sqrt{\frac{GM}{r_m}}=\sqrt{\frac{(1+\epsilon)GM}{r_0}}
//h=v_m r_m
//t=r_0^2\sqrt{\frac{1+\epsilon}{GMr_0}}\int_0^{\theat-\theta_0}\frac{\mathrm{d}x}{(1+\epsilon\cos x)^2}
//（设t=0时，行星位于近日点）
struct orbit_t
{
	floatmp_t r0,epsilon,theta0;//轨道基本信息
	bool direction;//是否反向运动（即\theta随t的增加而减少）
};