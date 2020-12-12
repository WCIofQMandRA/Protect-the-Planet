//orbit.hpp: 类orbit_t的声明

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

#ifndef ORBIT_HPP
#define ORBIT_HPP

#include "include.hpp"
//参见doc/轨道生成/orbit.pdf
//r0 为轨道近日点的曲率半径
//epsilon 为轨道离心率
//theat0 为行星近日点的极角
class orbit_t
{
private:
	double r0,epsilon,theta0;//轨道基本信息
	static thread_local double GM;//环绕的中心天体的信息
	static thread_local uint64_t GM_version;//GM的版本，每次更新set_GM，GM_version++
	//缓存
	//tmp1=r0*sqrt(r0_GM)（积分号前的系数）
	//tmp2=sqrt(abs((epsilon-1)/(epsilon+1)))
	//tmp3=abs(1-epsilon*epsilon)
	//tmp4=1/tmp3*sqrt(tmp3)
	//tmp5=acos(-1/epsilon)
	mutable double tmp1;
	double tmp2,tmp3,tmp4,tmp5;
	mutable uint64_t tmp1_version=0;//当前对象的tmp1的版本，如果tmp1_version<GM_version则需要重新计算tmp1
	bool direction;//是否反向运动（即\theta随t的增加而减少）
	void calc_tmp1()const;
	void calc_tmp2345();
	double integral(const double &x)const;
public:
	double calc_time(const double &theta)const;
	double calc_theta(const double &time)const;
	double calc_r(const double &theta)const;
	double calc_theta_fromr(const double &r)const;
	inline const double &get_r0()const{return r0;}
	inline const double &get_epsilon()const{return epsilon;}
	inline const double &get_theta0()const{return theta0;}
	void set_orbit(const double &r0_,const double &epsilon_,const double &theta0_,bool direction_);
	static void set_GM(const double &GM_);
};

#endif//ORBIT_HPP
