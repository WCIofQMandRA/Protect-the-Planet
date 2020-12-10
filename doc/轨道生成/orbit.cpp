//This file is NOT part of the game 保卫行星.

//	This program is free software; you can redistribute it and/or modify it
//	under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 3, or (at your option) any
//	later version.
#include <iostream>
#include <iomanip>
#include <boost/multiprecision/mpfr.hpp>
#include <boost/math/constants/constants.hpp>
using namespace std;
using namespace boost::multiprecision;

typedef number<mpfr_float_backend<75,allocate_stack>,et_on> floatmp_t;

floatmp_t GM;//恒星数据

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
struct planet_t
{
	floatmp_t r0,theta0,epsilon;//轨道的基本数据
	floatmp_t tmp;//辅助数据，tmp=r_0^2\sqrt{\frac{1+\epsilon}{GMr_0}}
};

//计算\int_0^{\theat-\theta_0}\frac{\mathrm{d}x}{(1+\epsilon\cos x)^2}
//x=\theat-\theta_0
//e=epsilon
floatmp_t integral(const floatmp_t &x,const floatmp_t &e)
{
	static floatmp_t double_pi=boost::math::constants::pi<floatmp_t>()*2;
	if(e<-1e-50)return nan("");
	if(abs(e-1.0)<1e-50)//e==1
	{
		floatmp_t tmp=tan(x*0.5);
		return tmp*(1+tmp*tmp/3)*0.5;
	}
	else if(e<1e-50)//e==0
	{
		return x;
	}
	else if(e>1.0)
	{
		floatmp_t tmp1=e-1,tmp2=e+1;
		floatmp_t tmp3=tmp1*tmp2;
		return e*sin(x)/(tmp3*(e*cos(x)+1))-2*atanh(sqrt(tmp1/tmp2)*tan(x*0.5))/(tmp3*sqrt(tmp3));
	}
	else
	{
		floatmp_t tmp1=1-e,tmp2=1+e;
		floatmp_t tmp3=tmp1*tmp2;
		floatmp_t tmp4=tmp3*sqrt(tmp3);
		return 2*atan(sqrt(tmp1/tmp2)*tan(x*0.5))/tmp4-e*sin(x)/(tmp3*(e*cos(x)+1))+floor(x/double_pi+0.5)*double_pi/tmp4;
	}
}

//计算行星到达theta处的时间
floatmp_t calc_time(const floatmp_t &theta,const planet_t &planet)
{
	return planet.tmp*integral(theta-planet.theta0,planet.epsilon);
}

floatmp_t calc_theta(const floatmp_t &t,const planet_t &planet)
{
	floatmp_t l=0,r=0.5,mid;
	while(calc_time(r,planet)<t)
	{
		l=r;
		r*=2;
	}
	while(r-l>1e-50)
	{
		mid=(r+l)*0.5;
		if(calc_time(mid,planet)>t)
			r=mid;
		else
			l=mid;
	}
	return (r+l)*0.5;
}

void calc_tmp(planet_t &p)
{
	p.tmp=p.r0*sqrt(p.r0/GM);
}

int main()
{
	planet_t planet;
	cin>>GM>>planet.r0>>planet.epsilon>>planet.theta0;
	calc_tmp(planet);
	floatmp_t theta;
	cin>>theta;
	cout<<setprecision(50)<<fixed;
	cout<<calc_time(theta,planet)<<endl;
	floatmp_t t;
	cin>>t;
	cout<<calc_theta(t,planet)<<endl;
	return 0;
}

/*
太阳：GM=6.6743e-11*1.9891e30=1.327585013e20
地球：
\epsilon=0.0167086
r_m=1.471e11
r0=149557835060
v_m=3.029e4
T=31558149.7635456

2pi=6.283185307179586476925286766559

//calc_time(2pi)=31553267.96945434814622214184585735103375899990859993628497
//calc_theta(T)=6.28419059371324618382656803040540833884752285111638
*/
