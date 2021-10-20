/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Object.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:13:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/19 10:43:12 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_HPP
# define OBJECT_HPP

# include "General.hpp"
# include <array>

enum	e_type
{
	Cube,
	Sphere
};

class Object
{
	public :
		Object(void);
		Object(const Object &src);
		virtual ~Object(void);

		Object	&operator=(const Object &rhs);	/* REMEMBER TO UPDATE */

		enum e_type	type;
		vec3		position;
		vec3		velocity;
		float		mass;

		vec3		angular_position;	/* for cubes */
		vec3		angular_velocity;	/* for cubes */
		vec3		dimension;			/* for cubes */
		float		radius;				/* for all */

		/* for physical engine */
		bool				rest;		/* is rested on ground */
		std::array<vec3, 8>	points;		/* points of cube */
		std::array<vec3, 3>	normals;	/* normals of all cube faces */

		void	compute_points(void);
};

#endif
