//type.cpp: 游戏内核的相关结构体的定义

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

#include "type.hpp"
#include <cmath>

received_effect_player_t& received_effect_player_t::operator+=(const received_effect_player_t &x)
{
	stop_hunger_decrease=stop_hunger_decrease||x.stop_hunger_decrease;
	speed_rate*=x.speed_rate;
	return *this;
}

received_effect_weapon_t& received_effect_weapon_t::operator+=(const received_effect_weapon_t &x)
{
	infinate_pills=infinate_pills||x.infinate_pills;
	infinate_pill_speed=infinate_pill_speed||x.infinate_pill_speed;
	infinate_power=infinate_power||x.infinate_power;
	hurt_box=hurt_box&&x.hurt_box;
	infinate_hurt_count=infinate_hurt_count||x.infinate_hurt_count;
	shoot_speed_rate*=x.shoot_speed_rate;
	pill_speed_rate*=x.pill_speed_rate;
	power_rate*=x.power_rate;
	hurt_count_rate*=x.hurt_count_rate;
	return *this;
}

received_effect_planet_t& received_effect_planet_t::operator+=(const received_effect_planet_t &x)
{
	stop_heath_decrease=stop_heath_decrease||x.stop_heath_decrease;
	negtive_hurt=negtive_hurt||x.negtive_hurt;
	hurt_rate*=x.hurt_rate;
	health_add+=x.health_add;
	health_mul*=x.health_mul;
	return *this;
}

received_effect_meteorite_t& received_effect_meteorite_t::operator+=(const received_effect_meteorite_t &x)
{
	kill_after_shooted=kill_after_shooted||x.kill_after_shooted;
	hurt_rate*=x.hurt_rate;
	power_rate*=x.power_rate;
	strength_add+=x.strength_add;
	strength_mul*=x.strength_mul;
	speed_rate*=x.speed_rate;
	return *this;
}

received_effect_box_t& received_effect_box_t::operator+=(const received_effect_box_t &x)
{
	hurt_by_weapon=hurt_by_weapon&&x.hurt_by_weapon;
	power_rate*=x.power_rate;
	speed_rate*=x.speed_rate;
	strength_mul*=x.strength_mul;
	strength_add+=x.strength_add;
	return *this;
}

void meteorite_t::to_p(meteoritep_t &p)const
{
	double r=orbit.calc_r(theta);
	p.x=r*cos(theta);
	p.y=r*sin(theta);
	p.size=size;
	p.type=type;
	p.complete_rate=static_cast<uint16_t>(3*(strength_left-1)/strength)+1;
	p.effects.resize(received_effect.size());
	size_t i=0;
	for(auto it=received_effect.cbegin();it!=received_effect.cend();++it,++i)
	{
		p.effects[i]=it->first;
	}
}

void box_t::to_p(boxp_t &p)const
{
	double r=orbit.calc_r(theta);
	p.x=r*cos(theta);
	p.y=r*sin(theta);
	p.size=size;
	p.type=type;
	p.complete_rate=static_cast<uint16_t>(3*(strength_left-1)/strength)+1;
	size_t i=0;
	p.effects.resize(received_effect.size());
	for(auto it=received_effect.cbegin();it!=received_effect.cend();++it,++i)
	{
		p.effects[i]=it->first;
	}
}

void box_t::to_d(boxd_t &d) const
{
	d.r=orbit.calc_r(theta);
	d.theta=theta;
	d.complete_rate=static_cast<uint16_t>(3*(strength_left-1)/strength)+1;
	d.size=size;
	d.contains=contains;
	d.type=type;
}

void boxd_t::to_p(boxp_t &p) const
{
	p.x=r*cos(theta);
	p.y=r*sin(theta);
	p.size=size;
	p.type=type;
	p.complete_rate=complete_rate;
	p.effects.clear();
}

void weapon_t::from_0(const weapon0_t &x)
{
	type=x.type;
	last_use_time=0;
	received_effect.clear();
	combined_effect=received_effect_weapon_t();
}
