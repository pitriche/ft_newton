/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Collider.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:14:54 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/22 18:03:55 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Collider.hpp"
#include "Utils.hpp"	/* square */
#include "Line.hpp"		/* Line */
#include <cmath>		/* abs */

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

	obj1.velocity += Dp / obj1.mass;
	obj2.velocity -= Dp / obj2.mass;
	obj1.rest = false;
	obj2.rest = false;
}

/* self explanatory, solve the collision by displacing the lightest object*/
static void	_solve_collision(Object &obj1, Object &obj2, const vec3 &solution)
{
	if (obj1.type == Sphere && obj2.type == Sphere)
	{
		if (obj1.mass >= obj2.mass)
			obj2.position = obj1.position + solution;
		else
			obj1.position = obj2.position - solution;
	}
	else
	{
		if (obj1.mass >= obj2.mass)
			obj2.position += solution;
		else
			obj1.position -= solution;
	}
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

/* #########################	Cube - Line			######################### */

// checker avec cube sans dimension et rotation pour tester

bool	_cube_line_collision(Object &cube, Line line) //////////////////////////////// static here
{
	float	tmin;
	float	tmax;
	float	tymin;
	float	tymax;
	float	tzmin;
	float	tzmax;
	vec3	inv_dir;
	vec3	null;
	bool	axis_positive[3];

	/* axis align the cube, with lower, closest left cornet at origin */
	line.origin -= cube.position - cube.dimension * 0.5f;
	vec3_rotate_inverse(line.origin, cube.angular_position);
	vec3_rotate_inverse(line.dir, cube.angular_position);

	null = {0.0f, 0.0f, 0.0f};
	for (unsigned axis = 0; axis < 3; ++axis)
	{
		inv_dir[axis] = 1.0f / line.dir[axis];
		axis_positive[axis] = line.dir[axis] > 0;
	}

	tmin = ((axis_positive[0] ? 0.0f : cube.dimension[0]) - line.origin[0]) * inv_dir[0];
	tmax = ((axis_positive[0] ? cube.dimension[0] : 0.0f) - line.origin[0]) * inv_dir[0];

	tymin = ((axis_positive[1] ? 0.0f : cube.dimension[1]) - line.origin[1]) * inv_dir[1];
	tymax = ((axis_positive[1] ? cube.dimension[1] : 0.0f) - line.origin[1]) * inv_dir[1];

	if ((tmin > tymax) || (tymin > tmax))
		return (false);
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	tzmin = ((axis_positive[2] ? 0.0f : cube.dimension[2]) - line.origin[2]) * inv_dir[2];
	tzmax = ((axis_positive[2] ? cube.dimension[2] : 0.0f) - line.origin[2]) * inv_dir[2];

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	std::cout << "Hit: tmin>" << tmin << " tmax<" << tmax << std::endl;
	return (true);

	// tmin = (bounds[r.sign[0]].x - r.orig.x) * r.invdir.x; 
	// tmax = (bounds[1-r.sign[0]].x - r.orig.x) * r.invdir.x; 
	// tymin = (bounds[r.sign[1]].y - r.orig.y) * r.invdir.y; 
	// tymax = (bounds[1-r.sign[1]].y - r.orig.y) * r.invdir.y; 

	// if ((tmin > tymax) || (tymin > tmax)) 
	// 	return false; 
	// if (tymin > tmin) 
	// 	tmin = tymin; 
	// if (tymax < tmax) 
	// 	tmax = tymax; 
 
	// tzmin = (bounds[r.sign[2]].z - r.orig.z) * r.invdir.z; 
	// tzmax = (bounds[1-r.sign[2]].z - r.orig.z) * r.invdir.z; 
 
	// if ((tmin > tzmax) || (tzmin > tmax)) 
	// 	return false; 
	// if (tzmin > tmin) 
	// 	tmin = tzmin; 
	// if (tzmax < tmax) 
	// 	tmax = tzmax; 
	// return (true);
}














/* #########################		Main			######################### */


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
#include <iostream> /////////////////////////////////////////////////////////////////////////////////////
static bool	_cube_cube_collision_type0(Object &cube, Object &cube2)
{
	bool	axis_positive[3];
	vec3	relative_pos_abs;
	vec3	intersect;
	vec3	normal;

	/* for each of cube2's points */
	for (unsigned i = 0; i < 8; ++i)
	{
		relative_pos_abs = cube2.points[i] - cube.position;
		vec3_rotate_inverse(relative_pos_abs, cube.angular_position);
		for (unsigned axis_vec = 0; axis_vec < 3; ++axis_vec)
			axis_positive[axis_vec] = (relative_pos_abs[axis_vec] >= 0.0f);
		for (float &f : relative_pos_abs)
			f = std::abs(f);

		intersect = relative_pos_abs - (cube.dimension * 0.5f);
		/* axis separation theorem check */
		if (intersect[0] < 0.0f && intersect[1] < 0.0f && intersect[2] < 0.0f)
		{
			std::cout << "HIT\n";
			unsigned axis = Utils::max3_id(intersect);
			normal = cube.normals[axis] * (axis_positive[axis] ? 1.0f : -1.0f);
			_solve_collision(cube, cube2, normal * -intersect[axis]);
			_compute_collision(cube, cube2, normal);
			return (true);
		}
	}
	return (false);
}

/*
	2 types :
	-type 0: obj2's corner is inside obj1 
	-type 1: obj1's corner is inside obj2 (swap cubes for type 0)
*/
static void	_cube_cube_collision(Object &cube, Object &cube2)
{
	/* check type 0 first, and type 1 if no collision */
	if (!_cube_cube_collision_type0(cube, cube2))
	{
		// std::cout << "2nd call\n\n";
		_cube_cube_collision_type0(cube2, cube);
	}
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

	/* solve collision by moving sphere away */
	_solve_collision(sphere, cube, normal * -solution);
	// sphere.position += normal * solution;

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
		// _solve_collision(cube, sphere, normal * solution);
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
	_solve_collision(obj1, obj2, solution);

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
		if (intersect < 0)
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
