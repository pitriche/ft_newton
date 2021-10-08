/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Object.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:13:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/08 10:57:18 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_HPP
# define OBJECT_HPP

# include "General.hpp"

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

		bool		rest;				/* is rested */
};

#endif
