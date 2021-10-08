/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:14:54 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/08 17:05:05 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "General.hpp"
#include <iostream>	/* ostream */
#include <cmath>	/* sqrt */

std::ostream	&operator<<(std::ostream &lhs, const vec3 &rhs)
{
	lhs << "[" << rhs[0] << ", " << rhs[1] << ", " << rhs[2] << "]";
	return (lhs);
}

vec3			&operator+=(vec3 &lhs, const vec3 &rhs)
{
	lhs[0] += rhs[0];
	lhs[1] += rhs[1];
	lhs[2] += rhs[2];
	return (lhs);
}

vec3			&operator*=(vec3 &lhs, float rhs)
{
	lhs[0] *= rhs;
	lhs[1] *= rhs;
	lhs[2] *= rhs;
	return (lhs);
}

vec3			operator*(const vec3 &lhs, float rhs)
{
	vec3	tmp;

	tmp[0] = lhs[0] * rhs;
	tmp[1] = lhs[1] * rhs;
	tmp[2] = lhs[2] * rhs;
	return (tmp);
}

vec3			operator/(const vec3 &lhs, float rhs)
{
	vec3	tmp;

	tmp[0] = lhs[0] / rhs;
	tmp[1] = lhs[1] / rhs;
	tmp[2] = lhs[2] / rhs;
	return (tmp);
}

vec3			operator-(const vec3 &lhs, const vec3 &rhs)
{
	vec3	tmp;

	tmp[0] = lhs[0] - rhs[0];
	tmp[1] = lhs[1] - rhs[1];
	tmp[2] = lhs[2] - rhs[2];
	return (tmp);
}

vec3			operator+(const vec3 &lhs, const vec3 &rhs)
{
	vec3	tmp;

	tmp[0] = lhs[0] + rhs[0];
	tmp[1] = lhs[1] + rhs[1];
	tmp[2] = lhs[2] + rhs[2];
	return (tmp);
}

/* dot product */
float			operator*(const vec3 &lhs, const vec3 &rhs)
{ return (lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2]); }

/* USE NORMALIZE IF POSSIBLE */
float			vec3_length(const vec3 &vec)
{
	return (sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]));
}

void			vec3_normalize(vec3 &vec)
{
	float inv_sqrt;

	inv_sqrt = 1 / sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	vec[0] *= inv_sqrt;
	vec[1] *= inv_sqrt;
	vec[2] *= inv_sqrt;
}