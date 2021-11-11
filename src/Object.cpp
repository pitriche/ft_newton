/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Object.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:14:54 by pitriche          #+#    #+#             */
/*   Updated: 2021/11/10 17:15:54 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Object.hpp"
#include <cmath>	/* cos, sin */

Object::Object(void) : rest(false) { }
Object::Object(const Object &src) { *this = src; }
Object::~Object(void) { }

Object	&Object::operator=(const Object &rhs)
{
	this->type = rhs.type;
	this->position = rhs.position;
	this->velocity = rhs.velocity;
	this->mass = rhs.mass;
	this->angular_position = rhs.angular_position;
	this->angular_velocity = rhs.angular_velocity;
	this->dimension = rhs.dimension;
	this->radius = rhs.radius;
	this->rest = rhs.rest;
	this->points = rhs.points;
	this->edges = rhs.edges;
	this->normals = rhs.normals;
	return (*this);
}

/* compute points, edges and normal, for collision computing and solving */
void	Object::compute_points(void)
{
	vec3	tmp;
	vec3	rotation[3];
	vec3	axis;
	float	theta;
	float	cos_th;
	float	sin_th;

	/* compute rotation matrix only once for optimization */
	theta = vec3_length(this->angular_position);
	if (theta != 0.0f)
	{
		axis = this->angular_position * (1 / theta);
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
	}
	else
	{
		rotation[0] = {1, 0, 0};
		rotation[1] = {0, 1, 0};
		rotation[2] = {0, 0, 1};
	}
	

	/* Clockwise, top then bottom, all positive (far right) first */
	this->points[0] = {0.5f, 0.5f, 0.5f};
	this->points[1] = {0.5f, 0.5f, -0.5f};
	this->points[2] = {-0.5f, 0.5f, -0.5f};
	this->points[3] = {-0.5f, 0.5f, 0.5f};
	this->points[4] = {0.5f, -0.5f, 0.5f};
	this->points[5] = {0.5f, -0.5f, -0.5f};
	this->points[6] = {-0.5f, -0.5f, -0.5f};
	this->points[7] = {-0.5f, -0.5f, 0.5f};
	this->normals[0] = {1.0f, 0.0f, 0.0f};	/* X aligned normal (right face) */
	this->normals[1] = {0.0f, 1.0f, 0.0f};	/* Y aligned normal (top face) */
	this->normals[2] = {0.0f, 0.0f, 1.0f};	/* Z aligned normal (back face) */
	for (vec3 &vec : this->points)	/* rotate then translate points */
	{
		tmp = vec;
		tmp[0] *= this->dimension[0];
		tmp[1] *= this->dimension[1];
		tmp[2] *= this->dimension[2];
		vec[0] = rotation[0] * tmp;
		vec[1] = rotation[1] * tmp;
		vec[2] = rotation[2] * tmp;
		vec += this->position;
	}
	for (vec3 &vec : this->normals)	/* rotate normals */
	{
		tmp = vec;
		vec[0] = rotation[0] * tmp;
		vec[1] = rotation[1] * tmp;
		vec[2] = rotation[2] * tmp;
	}

	/*
		This is depressing..
		3--------0.
		|`.      | `.
		|  `2----+---1
		|   |    |   |
		7---+----4.  |
		 `. |      `.|
		   `6--------5
	*/

	/* X aligned */
	this->edges[0] = Line(this->points[3], this->points[0]);
	this->edges[1] = Line(this->points[2], this->points[1]);
	this->edges[2] = Line(this->points[6], this->points[5]);
	this->edges[3] = Line(this->points[7], this->points[4]);
	/* Y aligned */
	this->edges[4] = Line(this->points[7], this->points[3]);
	this->edges[5] = Line(this->points[4], this->points[0]);
	this->edges[6] = Line(this->points[5], this->points[1]);
	this->edges[7] = Line(this->points[6], this->points[2]);
	/* Z aligned */
	this->edges[8] = Line(this->points[2], this->points[3]);
	this->edges[9] = Line(this->points[1], this->points[0]);
	this->edges[10] = Line(this->points[5], this->points[4]);
	this->edges[11] = Line(this->points[6], this->points[7]);

	/* Edges directions cannot have null (makes collision detection bug) */
	for (Line &edge : this->edges)
	{
		if (edge.dir[0] == 0.0f)
			edge.dir[0] = 1e-30f;
		if (edge.dir[1] == 0.0f)
			edge.dir[1] = 1e-30f;
		if (edge.dir[2] == 0.0f)
			edge.dir[2] = 1e-30f;
	}
}
