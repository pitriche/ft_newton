/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:14:54 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/07 18:03:59 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "General.hpp"
#include <iostream>

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