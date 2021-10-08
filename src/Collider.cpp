/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Collider.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:14:54 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/08 16:30:23 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Collider.hpp"
#include <cmath>

static void	_sphere_floor_collision(Object &obj, float delta)
{
	/* inelastic collision */
	if (abs(obj.velocity[1]) <= INELASTIC_SPEED)
	{
		obj.rest = true;
		obj.velocity[1] = 0.0f;
		obj.velocity *= 1 - (COEFF_OF_FRICTION * delta);
	}

	/* partially elastic collision */
	obj.position[1] = obj.radius;
	obj.velocity[1] *= -COEFF_OF_RESTITUTION;
}

static void	_cube_floor_collision(Object &obj, float delta)
{
	/* inelastic collision */
	if (abs(obj.velocity[1]) <= INELASTIC_SPEED)
	{
		obj.rest = true;
		obj.velocity[1] = 0.0f;
		obj.velocity *= 1 - (COEFF_OF_FRICTION * delta);
	}

	/* partially elastic collision */
	obj.position[1] = obj.radius;
	obj.velocity[1] *= -COEFF_OF_RESTITUTION;
}

namespace Collider
{
	void	collide_floor(Object &obj, float delta)
	{
		/* preliminary check */
		if (obj.position[1] - obj.radius < 0)
		{
			switch (obj.type)
			{
				case (Sphere) : _sphere_floor_collision(obj, delta); break;
				case (Cube) : _cube_floor_collision(obj, delta); break;
			}
		}
	}

	void	collide_object(Object &obj1, Object &obj2)
	{
		vec3	obj1_obj2;
		float	intersect;
		float	dist;
		float	dist_radii;
		vec3	momentum;

		obj1_obj2 = obj2.position - obj1.position;
		dist = vec3_length(obj1_obj2);
		dist_radii = obj1.radius + obj2.radius;
		intersect = dist - dist_radii;
		if (intersect < 0)
		{
			momentum = obj1.velocity * obj1.mass + obj2.velocity * obj2.mass;
			obj2.position = obj1.position + (obj1_obj2 * (dist_radii / dist));

			momentum *= 1 / (obj1.mass + obj2.mass);
			obj1.velocity = momentum;
			obj2.velocity = momentum;
			obj1.rest = false;
			obj2.rest = false;
		}
	}
}
