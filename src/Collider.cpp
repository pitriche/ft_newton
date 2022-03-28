/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Collider.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:14:54 by pitriche          #+#    #+#             */
/*   Updated: 2021/11/15 14:39:27 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Collider.hpp"
#include "Utils.hpp"	/* square */
#include "Line.hpp"		/* Line */
#include <cmath>		/* abs */
#include "All.hpp"		/* to get delta time for arbitrary operations */

#ifdef USE_ROTATIONS
	/* compute angular velocity change, return the remaining amount of impulse */
	static float	_compute_rotation(Object &cube, const vec3 &normal, const vec3
		&impact, float impulse)
	{
		vec3	axis;		/* rotation axis of the collision */
		float	dist;		/* distance of the moment of force's arm */
		float	angular_impulse;
		float	moment_of_inertia;
		float	d_omega;	/* angular velocity change */
		float	conversion;	/* ratio of impulse converted to rotation energy */

		axis = vec3_cross_product(normal, cube.position - impact);
		dist = vec3_length(axis); /* can be negative i think */ /* no it can't be negative you absolute buffoon */
		conversion = dist / Utils::max3(cube.dimension);

		if (all.event.key.debug_impact)
		{
			Utils::add_sphere(all.game.debug, impact, 0.7f);
			Utils::debug_draw_line(all.game.debug, Line(cube.position, cube.position + axis * 10));;
		}
		// std::cout << "Impact at " << impact << " axis " << vec3_normalize(axis) << std::endl;
		// std::cout << "Impact normal " << normal << std::endl;
		// std::cout << "Conversion " << (1.0f - conversion) * 100.0f << '%' << std::endl;
		if (dist == 0)	/* if distance is 0, there is no rotation */
			return (1.0f);
		angular_impulse = impulse * dist;
		moment_of_inertia = cube.mass * Utils::square(cube.radius * 2) / 6.0f;	/* arbitrary formula */
		d_omega = angular_impulse / moment_of_inertia;
		cube.angular_velocity += vec3_normalize(axis) * d_omega;				/* apply */

		/* omega damping */
		// std::cout << "Impact impulse euler vector" << vec3_normalize(axis) * d_omega << std::endl;
		// std::cout << "Impact delta omega " << d_omega << std::endl;
		cube.angular_velocity *= 1.0f;
		// return (1.0f - conversion);
		return (1.0f);
	}
#endif

/* collision linear velocity change, then angular */
static void		_compute_collision(Object &obj1, Object &obj2, const vec3
	&normal, const vec3 &impact)
{
	vec3	Dp;			/* positive delta momentum, obj1's momentum change */
	float	Dp_scale;	/* delta p's scale */
	float	v1;			/* obj1's velocity projected to impact normal */
	float	v2;			/* obj2's velocity projected to impact normal */
	float	conversion1;	/* obj1's linear / rotational impulse ratio */
	float	conversion2;	/* obj2's linear / rotational impulse ratio */

	Dp = normal;
	v1 = obj1.velocity * normal;
	v2 = obj2.velocity * normal;
	Dp_scale = 2.0f * obj1.mass * obj2.mass * (v2 - v1);
	Dp_scale /= obj1.mass + obj2.mass;
	if (v1 - v2 > ELASTIC_SPEED)
		Dp_scale /= 2 - COEFF_OF_RESTITUTION;
	Dp *= Dp_scale;

	obj1.rest = false;
	obj2.rest = false;
	conversion1 = 1.0f;
	conversion2 = 1.0f;

	#ifdef USE_ROTATIONS
		if (obj1.type == Cube)
			conversion1 = _compute_rotation(obj1, normal, impact, Dp_scale / 2.0f);
		if (obj2.type == Cube)
			conversion2 = _compute_rotation(obj2, normal * -1.0f, impact, Dp_scale
				/ 2.0f);
	#endif
	#ifndef USE_ROTATIONS
		(void)impact;
	#endif

	obj1.velocity += Dp * conversion1 / obj1.mass;
	obj2.velocity -= Dp * conversion2 / obj2.mass;
}

/* self explanatory, solve the collision by displacing the lightest object*/
static void		_solve_collision(Object &obj1, Object &obj2, const vec3 &solution)
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

static void	_sphere_floor_collision(Object &obj, bool wind)
{
	/* inelastic collision and ground friction, if no wind */
	if (abs(obj.velocity[1]) <= INELASTIC_SPEED && !wind)
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

static void	_cube_floor_collision(Object &obj, bool wind)
{
	float	above_ground;
	vec3	impact;

	/* secondary collison check for cube */
	above_ground = INFINITY;
	for (vec3 &vec : obj.points)
		if (vec[1] < above_ground)
		{
			above_ground = vec[1];
			impact = vec;
		}
	if (above_ground > 0.001f)	/* to avoid rounding errors */
		return ;

	#ifdef USE_ROTATIONS
		_compute_rotation(obj, {0, 1, 0}, impact, -obj.velocity[1] * obj.mass * 1);
	#endif

	/* inelastic collision and ground friction, if no wind */
	if (abs(obj.velocity[1]) <= INELASTIC_SPEED && !wind)
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

static bool	_cube_line_collision(Object &cube, Line line)
{
	float	tmin;
	float	tmax;
	float	tmpmin;
	float	tmpmax;
	vec3	inv_dir;
	bool	axis_positive[3];

	/* axis align the cube, with lower, closest left cornet at origin */
	line.origin -= cube.position;
	vec3_rotate_inverse(line.origin, cube.angular_position);
	line.origin += cube.dimension * 0.5f;
	vec3_rotate_inverse(line.dir, cube.angular_position);

	for (unsigned axis = 0; axis < 3; ++axis)
	{
		inv_dir[axis] = 1.0f / line.dir[axis];
		axis_positive[axis] = line.dir[axis] > 0;
	}

	/* X axis */
	tmin = ((axis_positive[0] ? 0.0f : cube.dimension[0]) - line.origin[0]) * inv_dir[0];
	tmax = ((axis_positive[0] ? cube.dimension[0] : 0.0f) - line.origin[0]) * inv_dir[0];

	/* Y axis */
	tmpmin = ((axis_positive[1] ? 0.0f : cube.dimension[1]) - line.origin[1]) * inv_dir[1];
	tmpmax = ((axis_positive[1] ? cube.dimension[1] : 0.0f) - line.origin[1]) * inv_dir[1];
	if ((tmin > tmpmax) || (tmpmin > tmax))
		return (false);
	if (tmpmin > tmin)
		tmin = tmpmin;
	if (tmpmax < tmax)
		tmax = tmpmax;

	/* X axis */
	tmpmin = ((axis_positive[2] ? 0.0f : cube.dimension[2]) - line.origin[2]) * inv_dir[2];
	tmpmax = ((axis_positive[2] ? cube.dimension[2] : 0.0f) - line.origin[2]) * inv_dir[2];
	if ((tmin > tmpmax) || (tmpmin > tmax))
		return (false);
	if (tmpmin > tmin)
		tmin = tmpmin;
	if (tmpmax < tmax)
		tmax = tmpmax;

	if (tmax <= 0.0f)
		return (false);
	if (tmin > line.length)
		return (false);
	// tmin > 0.0f ? tmin : tmax
	return (true);
}

/* #########################		Main			######################### */

/*
	find if the centers are closer together than the smallest dimention of both
	cubes. If it is the case, spread them apart widely (f*ck finding the correct
	amount)
*/
static void	_solve_cube_cube_secondary(Object &cube, Object &cube2)
{
	vec3	cube_cube2;
	float	min_size;
	float	min_size_cube2;
	float	dist_radii;

	cube_cube2 = cube2.position - cube.position;
	min_size = -Utils::max3(cube.dimension * -1.0f);
	min_size_cube2 = -Utils::max3(cube2.dimension * -1.0f);
	if (min_size_cube2 < min_size)
		min_size = min_size_cube2;
	dist_radii = vec3_length(cube_cube2);
	if (dist_radii < min_size)
	{
		
		vec3_normalize(cube_cube2);
		cube_cube2 *= 0.5f;	/*equally move each objects */
		cube_cube2 *= (cube.radius + cube2.radius) - dist_radii;
		cube2.position += cube_cube2;
		cube.position -= cube_cube2;
	}
}

static bool	_cube_cube_collision_type0(Object &cube, Object &cube2)
{
	unsigned	axis;
	bool		axis_positive[3];
	vec3		relative_pos_abs;
	vec3		intersect;
	vec3		normal;
	bool		collision;

	collision = false;
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
		if (intersect[0] < 0.0f && intersect[1] < 0.0f && intersect[2] < 0.0f)
		{
			axis = Utils::max3_id(intersect);
			normal = cube.normals[axis] * (axis_positive[axis] ? 1.0f : -1.0f);
			_solve_collision(cube, cube2, normal * -intersect[axis]);
			_compute_collision(cube, cube2, normal, {0, 0, 0});
			cube2.compute_points();	/* update points after each collision */
			collision = true;
		}
	}
	_solve_cube_cube_secondary(cube, cube2);
	return (collision);
}

/*
cube rotations when impact points are negative seem to bug 
*/

static bool	_cube_cube_collision_type1(Object &cube, Object &cube2)	
{
	float	solution;
	vec3	normal;

	/* cube's edges in cube2 */
	for (unsigned i = 0; i < 12; ++i)
	{
		if (_cube_line_collision(cube2, cube.edges[i]))
		{
			// Utils::debug_draw_line(all.game.debug, cube.edges[i]);
			normal = cube.edges[i].origin + cube.edges[i].dir *
			(cube.edges[i].length / 2.0f);
			normal -= cube.position;
			vec3_normalize(normal);

			/* this is disgusting beyound words */
			solution = vec3_length(cube.velocity - cube2.velocity) *
			(all.time.delta / 1000000000.0f);

			_solve_collision(cube, cube2, normal * solution);
			_compute_collision(cube, cube2, normal, {0, 0, 0}); ///////////////////////////////////////////////// impact
			return (true);
		}
	}
	return (false);
}

/*
	2 types :
	-type 0: a cube's corner is inside the other
	-type 1: a cube's edge is inside the other
*/
static void	_cube_cube_collision(Object &cube, Object &cube2)
{
	/* check type 0, type 0 with object swapped, type 1, then type 1 swapped */
	if (!_cube_cube_collision_type0(cube, cube2) &&
		!_cube_cube_collision_type0(cube2, cube))
	{
		if (cube.radius > cube2.radius)
		{
			if (!_cube_cube_collision_type1(cube, cube2))
				_cube_cube_collision_type1(cube2, cube);
		}
		else
			if (!_cube_cube_collision_type1(cube2, cube))
				_cube_cube_collision_type1(cube, cube2);
	}
}

/* ########################################################################## */
/* #########################	Sphere - Cube		######################### */
/* ########################################################################## */

static void	_cube_sphere_type0(Object &cube, Object &sphere, vec3 &relative_pos,
	unsigned axis)
{
	vec3	normal;		/* type 0 impact normal */
	vec3	impact;
	float	solution;

	normal = cube.normals[axis] * ((relative_pos[axis] > 0) ? 1.0f : -1.0f);
	solution = cube.dimension[axis] / 2.0f -
	(abs(relative_pos[axis]) - sphere.radius);

	/* solve, then compute collision */
	_solve_collision(sphere, cube, normal * -solution);
	impact = sphere.position + normal * sphere.radius;
	_compute_collision(cube, sphere, normal, impact);
}

static void	_cube_sphere_type12(Object &cube, Object &sphere,
	vec3 &relative_pos, vec3 &relative_pos_abs, bool sure_axis[3])
{
	vec3	normal;		/* type 1 / 2 normal, move this up */
	vec3	impact;		/* impact point */
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
		vec3_rotate_euler(normal, cube.angular_position);
		solution = sphere.radius - std::sqrt(corner_dist_sq);

		/* solve, then compute collision */
		_solve_collision(cube, sphere, normal * -solution);
		impact = sphere.position + normal * sphere.radius;
		_compute_collision(cube, sphere, normal, impact);
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
	vec3_rotate_euler(relative_pos, cube.angular_position * -1);

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
	_compute_collision(obj1, obj2, normal, {0, 0, 0});
}

/* ########################################################################## */
/* #########################	Main wrapper		######################### */
/* ########################################################################## */
/* remember to move up on Z until all objects are above ground */

namespace Collider
{
	void	collide_floor(Object &obj, bool wind)
	{
		/* preliminary check */
		if (obj.position[1] - obj.radius <= 0)
		{
			switch (obj.type)
			{
				case (Sphere) : _sphere_floor_collision(obj, wind); break;
				case (Cube) : _cube_floor_collision(obj, wind); break;
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
