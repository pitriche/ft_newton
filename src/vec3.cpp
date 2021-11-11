/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:14:54 by pitriche          #+#    #+#             */
/*   Updated: 2021/11/10 17:25:41 by pitriche         ###   ########.fr       */
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

vec3			&operator-=(vec3 &lhs, const vec3 &rhs)
{
	lhs[0] -= rhs[0];
	lhs[1] -= rhs[1];
	lhs[2] -= rhs[2];
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

	// std::cout << "lhs before " << lhs;

	tmp[0] = lhs[0] * rhs;
	tmp[1] = lhs[1] * rhs;
	tmp[2] = lhs[2] * rhs;
	// std::cout << "lhs after " << lhs;
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

/*
	cross product
	R = A x B
	Rx = Ay * Bz - Az * By
	Ry = Az * Bx - Ax * Bz
	Rz = Ax * By - Ay * Bx
*/
vec3			vec3_cross_product(const vec3 &lhs, const vec3 &rhs)
{
	vec3	ret;

	ret[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
	ret[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
	ret[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
	return (ret);
}


float			vec3_length(const vec3 &vec)
{ return (std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2])); }

/* could be optimized with rsqrt */
vec3			&vec3_normalize(vec3 &vec)
{
	float inv_sqrt;

	inv_sqrt = 1 / sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	vec[0] *= inv_sqrt;
	vec[1] *= inv_sqrt;
	vec[2] *= inv_sqrt;
	return (vec);
}

/*
	rotation matrix for Z => X => Y rotations (roll => pitch => yaw)
	Y * X * Z
	CzCy + SzSxSy	| -SzCy + CzSxSy	| CxSy
	SzCx			| CzCx				| -Sy
	-CzSy + SzSxCy	| SzSy + CzSxCy		| CxCy
*/
void			vec3_rotate(vec3 &vec, float x, float y, float z)
{
	vec3	tmp;
	vec3	rotation[3];
	float	cos_x, cos_y, cos_z;
	float	sin_x, sin_y, sin_z;

	tmp = vec;
	cos_x = cos(x);
	cos_y = cos(y);
	cos_z = cos(z);
	sin_x = sin(x);
	sin_y = sin(y);
	sin_z = sin(z);
	rotation[0][0] = cos_z * cos_y + sin_z * sin_x * sin_y;
	rotation[0][1] = -sin_z * cos_y + cos_z * sin_x * sin_y;
	rotation[0][2] = cos_x * sin_y;
	rotation[1][0] = sin_z * cos_x;
	rotation[1][1] = cos_z * cos_x;
	rotation[1][2] = -sin_x;
	rotation[2][0] = -cos_z * sin_y + sin_z * sin_x * cos_y;
	rotation[2][1] = sin_z * sin_y + cos_z * sin_x * cos_y;
	rotation[2][2] = cos_x * cos_y;
	vec[0] = rotation[0] * tmp;
	vec[1] = rotation[1] * tmp;
	vec[2] = rotation[2] * tmp;
}

void			vec3_rotate(vec3 &vec, const vec3 &rotation)
{ vec3_rotate(vec, rotation[0], rotation[1], rotation[2]); }

/* inverse both matrix and rotation order */
/* big, like VERY BIG optimisation potential if i manually compute the rotation
matrix for the Y => X => Z rotation (~30min-1h, error prone) */
void			vec3_rotate_inverse(vec3 &vec, const vec3 &rotation)
{
	vec3_rotate(vec, 0, -rotation[1], 0);	/* Y */
	vec3_rotate(vec, -rotation[0], 0, 0);	/* X */
	vec3_rotate(vec, 0, 0, -rotation[2]);	/* Z */
}

void			vec3_rotate_euler(vec3 &vec, const vec3 &euler_vector)
{
	vec3	tmp;
	vec3	rotation[3];
	vec3	axis;
	float	theta;
	float	cos_th;
	float	sin_th;

	theta = vec3_length(euler_vector);
	if (theta == 0.0f)
		return ;
	axis = euler_vector * (1 / theta);
	cos_th = std::cos(theta);
	sin_th = std::sin(theta);
	rotation[0][0] = (1 - cos_th) * axis[0] * axis[0] + cos_th;
	rotation[0][1] = (1 - cos_th) * axis[0] * axis[1] - sin_th * axis[2];
	rotation[0][2] = (1 - cos_th) * axis[0] * axis[2] + sin_th * axis[1];
	rotation[1][0] = (1 - cos_th) * axis[1] * axis[0] + sin_th * axis[2];
	rotation[1][1] = (1 - cos_th) * axis[1] * axis[1] + cos_th;
	rotation[1][2] = (1 - cos_th) * axis[1] * axis[2] - sin_th * axis[0];
	rotation[2][0] = (1 - cos_th) * axis[2] * axis[0] - sin_th * axis[1];
	rotation[2][1] = (1 - cos_th) * axis[2] * axis[1] + sin_th * axis[0];
	rotation[2][2] = (1 - cos_th) * axis[2] * axis[2] + cos_th;
	tmp = vec;
	vec[0] = rotation[0] * tmp;
	vec[1] = rotation[1] * tmp;
	vec[2] = rotation[2] * tmp;
}
