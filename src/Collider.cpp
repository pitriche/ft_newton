/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Collider.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:14:54 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/20 15:18:49 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Collider.hpp"
#include "Utils.hpp"	/* square */
#include <cmath>		/* abs */

#include <iostream>
/* collision computation, NOT solving. Normal must be normalized */
static void	_compute_collision(Object &obj1, Object &obj2, const vec3 &normal)
{
	vec3	Dp;			/* positive delta momentum, obj1's momentum change */
	float	Dp_scale;	/* delta p's scale */
	float	v1;			/* obj1's velocity projected to impact normal */
	float	v2;			/* obj2's velocity projected to impact normal */


	Dp = normal;
	v1 = obj1.velocity * normal;
	v2 = obj2.velocity * normal;
	Dp_scale = 2.0f * obj1.mass * obj2.mass * (v2 - v1);
	Dp_scale /= obj1.mass + obj2.mass;
	if (v1 - v2 > ELASTIC_SPEED)
		Dp_scale /= 2 - COEFF_OF_RESTITUTION;
	Dp *= Dp_scale;

	// std::cout << "Delta p = " << Dp << std::endl;
	obj1.velocity += Dp / obj1.mass;	/* precomputing 1 / m1 */
	obj2.velocity -= Dp / obj2.mass;	/* might save 2 divisions */
	obj1.rest = false;
	obj2.rest = false;
}

/* ########################################################################## */
/* #########################	Floor collisions	######################### */
/* ########################################################################## */

static void	_sphere_floor_collision(Object &obj)
{
	/* inelastic collision and ground friction */
	if (abs(obj.velocity[1]) <= INELASTIC_SPEED)
	{
		obj.rest = true;
		obj.position[1] = obj.radius;
		obj.velocity[1] = 0.0f;
		obj.velocity *= 1 - COEFF_OF_FRICTION;
		return ;
	}

	/* partially elastic collision */
	obj.velocity[1] *= -1;
	obj.position[1] = obj.radius;
	obj.velocity *= COEFF_OF_RESTITUTION;
}

static void	_cube_floor_collision(Object &obj)
{
	float	above_ground;

	/* secondary collison check for cube */
	above_ground = INFINITY;
	for (vec3 &vec : obj.points)
		if (vec[1] < above_ground)
			above_ground = vec[1];
	if (above_ground > 0.001f)	/* to avoid rounding errors */
		return ;

	/* inelastic collision and ground friction */
	if (abs(obj.velocity[1]) <= INELASTIC_SPEED)
	{
		if (vec3_length(obj.angular_velocity) == 0.0f)
			obj.rest = true;
		obj.velocity[1] = 0.0f;
		obj.velocity *= 1 - COEFF_OF_FRICTION;
	}

	/* partially elastic collision */
	obj.position[1] -= above_ground;
	obj.velocity[1] *= -COEFF_OF_RESTITUTION;
}

/* ########################################################################## */
/* #########################	Cube - Cube			######################### */
/* ########################################################################## */


















/*
	 __ 	 __ 	 __ 	 __ 	 __
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
   \\##//  \\##//  \\##//  \\##//  \\##//
	\\//	\\//	\\//	\\//	\\//
	 \/ 	 \/ 	 \/ 	 \/ 	 \/ 
*/

/*
	2 types :
	-type 0: obj2's corner is inside obj1 
	-type 1: obj1's corner is inside obj2
*/
static void	_cube_cube_collision(Object &cube, Object &cube2)
{
	// _compute_collision(obj1, obj2, obj2.position - obj1.position);
}
/*
	 /\		 /\		 /\		 /\		 /\	
	//\\	//\\	//\\	//\\	//\\
   //##\\  //##\\  //##\\  //##\\  //##\\
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	|##|	|##|	|##|	|##|	|##|
	 -- 	 -- 	 -- 	 -- 	 -- 
*/


































/* ########################################################################## */
/* #########################	Sphere - Cube		######################### */
/* ########################################################################## */

static void	_cube_sphere_type0(Object &cube, Object &sphere, vec3 &relative_pos,
	unsigned axis)
{
	vec3	normal;		/* type 0 impact normal */
	float	solution;

	normal = cube.normals[axis] * ((relative_pos[axis] > 0) ? 1.0f : -1.0f);
	solution = cube.dimension[axis] / 2.0f -
	(abs(relative_pos[axis]) - sphere.radius);

	/* solve collision by moving sphere away. Cause mild ground sandwich */
	sphere.position += normal * solution;

	/* compute collision */
	_compute_collision(cube, sphere, normal);
}

static void	_cube_sphere_type12(Object &cube, Object &sphere,
	vec3 &relative_pos, vec3 &relative_pos_abs, bool sure_axis[3])
{
	vec3	normal;		/* type 1 / 2 normal, move this up */
	float	solution;	/* for type 0 solving */
	float	corner_dist_sq;
	
	corner_dist_sq = 0;
	normal = {0, 0, 0};
	for (unsigned axis = 0; axis < 3; ++axis)
		if (!sure_axis[axis])
		{
			corner_dist_sq += Utils::square(cube.dimension[axis] / 2.0f -
				relative_pos_abs[axis]);
			normal[axis] = cube.dimension[axis] / 2.0f - relative_pos_abs[axis];
			normal[axis] *= (relative_pos[axis] > 0) ? 1.0f : -1.0f;
		}
	if (corner_dist_sq < Utils::square(sphere.radius))
	{
		vec3_normalize(normal);
		vec3_rotate(normal, cube.angular_position);

		/* solve, then compute collision */
		solution = sphere.radius - std::sqrt(corner_dist_sq);
		sphere.position -= normal * solution;
		_compute_collision(cube, sphere, normal);
	}
}

/*
	3 types:
	- type 0 sphere clip face => face normal (potentially stable)
	- type 1 edge inside sphere => corner-sphere normal (unstable)
	- type 2 corner inside sphere => corner-sphere normal (unstable)
*/
static void	_cube_sphere_collision(Object &cube, Object &sphere)
{
	bool	sure_axis[3];
	vec3	relative_pos;
	vec3	relative_pos_abs;

	relative_pos = sphere.position - cube.position;
	vec3_rotate_inverse(relative_pos, cube.angular_position);
	relative_pos_abs = relative_pos;

	for (float &f : relative_pos_abs)	/* checks done with absolute values */
		f = std::abs(f);

	/* axis separation theorem check */
	if ((relative_pos_abs[0] - sphere.radius) * 2 > cube.dimension[0])
		return ;
	if ((relative_pos_abs[1] - sphere.radius) * 2 > cube.dimension[1])
		return ;
	if ((relative_pos_abs[2] - sphere.radius) * 2 > cube.dimension[2])
		return ;

	sure_axis[0] = relative_pos_abs[0] * 2 <= cube.dimension[0];
	sure_axis[1] = relative_pos_abs[1] * 2 <= cube.dimension[1];
	sure_axis[2] = relative_pos_abs[2] * 2 <= cube.dimension[2];

	/* type 0 hit */
	if ((int)sure_axis[0] + (int)sure_axis[1] + (int)sure_axis[2] >= 2)
	{
		if (!sure_axis[0])
			_cube_sphere_type0(cube, sphere, relative_pos, 0);
		else if (!sure_axis[2])
			_cube_sphere_type0(cube, sphere, relative_pos, 2);
		else	/* default axis is +Y */
			_cube_sphere_type0(cube, sphere, relative_pos, 1);
		return ;
	}

	/* type 1 and 2 hits */
	_cube_sphere_type12(cube, sphere, relative_pos, relative_pos_abs, sure_axis);
}

/* ########################################################################## */
/* #########################	Sphere - Sphere		######################### */
/* ########################################################################## */

static void	_sphere_sphere_collision(Object &obj1, Object &obj2, vec3 solution)
{
	vec3	normal;

	/* collision solving */
	obj2.position = obj1.position + solution;

	normal = solution;
	vec3_normalize(normal);
	_compute_collision(obj1, obj2, normal);
}

/* ########################################################################## */
/* #########################	Main wrapper		######################### */
/* ########################################################################## */
/* remember to move up on Z until all objects are above ground */

namespace Collider
{
	void	collide_floor(Object &obj)
	{
		/* preliminary check */
		if (obj.position[1] - obj.radius <= 0)
		{
			switch (obj.type)
			{
				case (Sphere) : _sphere_floor_collision(obj); break;
				case (Cube) : _cube_floor_collision(obj); break;
			}
		}
	}

	void	collide_object(Object &obj1, Object &obj2)
	{
		vec3	obj1_obj2;
		float	intersect;
		float	dist;
		float	dist_radii;

		obj1_obj2 = obj2.position - obj1.position;
		dist = vec3_length(obj1_obj2);
		dist_radii = obj1.radius + obj2.radius;
		intersect = dist - dist_radii;
		if (intersect < 0)	/* Circumscribed sphere collision check */
		{
			/* secondary collision, collision solving, trajectory change */
			if (obj1.type != obj2.type)		/* cube - sphere */
			{
				if (obj1.type == Cube)
					_cube_sphere_collision(obj1, obj2);
				else
					_cube_sphere_collision(obj2, obj1);
			}
			else if (obj1.type == Cube)		/* cube - cube */
				_cube_cube_collision(obj1, obj2);
			else if (obj1.type == Sphere)	/* sphere - sphere */
				_sphere_sphere_collision(obj1, obj2, obj1_obj2 *
					(dist_radii / dist));
		}
	}
}
