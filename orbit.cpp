//orbit.cpp: 类orbit_t的定义

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

#include "orbit.hpp"
#include <cmath>

const double pi2=M_PI*2;
thread_local double orbit_t::GM;//环绕的中心天体的信息
thread_local uint64_t orbit_t::GM_version=0;//GM的版本，每次更新set_GM，GM_version++

void orbit_t::calc_tmp1()const
{
	tmp1=r0*sqrt(r0/GM);
	if(direction)tmp1=-tmp1;
	tmp1_version=GM_version;
}

void orbit_t::calc_tmp2345()
{
	double a=abs(epsilon-1),b=epsilon+1;
	tmp2=sqrt(a/b);
	tmp3=a*b;
	tmp4=1/(tmp3*sqrt(tmp3));
	if(epsilon>=1)
		tmp5=acos(-1/epsilon);
	else
		tmp5=boost::math::constants::pi<double>();
}

void orbit_t::set_GM(const double &GM_)
{
	GM=GM_;
	GM_version++;
}

void orbit_t::set_orbit(const double &r0_,const double &epsilon_,const double &theta0_,bool direction_)
{
	r0=r0_,epsilon=epsilon_,theta0=theta0_,direction=direction_;
	calc_tmp2345();
}

//计算\int_0^{\theat-\theta_0}\frac{\mathrm{d}x}{(1+\epsilon\cos x)^2}
//x=\theat-\theta_0
double orbit_t::integral(const double &x)const
{
	if(epsilon<-EPS64)return nan("");
	if(abs(epsilon-1.0)<EPS64)//e==1
	{
		double tmp=tan(x*0.5);
		return tmp*(1+tmp*tmp/3)*0.5;
	}
	else if(epsilon<EPS64)//e==0
	{
		return x;
	}
	else if(epsilon>1.0)
	{
		return epsilon*sin(x)/(tmp3*(epsilon*cos(x)+1))-2*atanh(tmp2*tan(x*0.5))*tmp4;
	}
	else
	{
		return 2*atan(tmp2*tan(x*0.5))*tmp4-epsilon*sin(x)/(tmp3*(epsilon*cos(x)+1))+floor(x/pi2+0.5)*pi2*tmp4;
	}
}

double orbit_t::calc_time(const double &theta)const
{
	if(tmp1_version<GM_version)
		calc_tmp1();
	return tmp1*integral(theta-theta0);
}

double orbit_t::calc_theta(const double &t)const
{
	if(tmp1_version<GM_version)
		calc_tmp1();
	double l=theta0-tmp5+EPS64,r=theta0+tmp5-EPS64,mid;
	if(direction)
	{
		if(epsilon<1)
		{
			while(calc_time(r)>t)
				l=r,r*=2;
			while(calc_time(l)<t)
				r=l,l*=2;
		}
		while(r-l>EPS64)
		{
			mid=(r+l)*0.5;
			if(calc_time(mid)<t)
				r=mid;
			else
				l=mid;
		}
	}
	else
	{
		if(epsilon<1)
		{
			while(calc_time(r)<t)
				l=r,r*=2;
			while(calc_time(l)>t)
				r=l,l*=2;
		}
		while(r-l>EPS64)
		{
			mid=(r+l)*0.5;
			if(calc_time(mid)>t)
				r=mid;
			else
				l=mid;
		}
	}
	return (r+l)*0.5;
}

double orbit_t::calc_r(const double &theta)const
{
	return r0/(1+epsilon*cos(theta-theta0));
}

double orbit_t::calc_theta_fromr(const double &r)const
{
	return acos(((r0/r)-1)/epsilon)+theta0;
}
