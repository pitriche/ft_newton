/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   General.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 13:52:16 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/08 13:14:39 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GENERAL_H
# define GENERAL_H

# include <array>

# define OPENGL_VERSION_MAJOR	4
# define OPENGL_VERSION_MINOR	1

# define WIN_TITLE		"The apple doesn't fall far from the tree"
# define WIN_SIZEX		1920
# define WIN_SIZEY		1080
# define WIN_POSX		100
# define WIN_POSY		50

/* ########################################################################## */

# define BASE_FPS		60				/* 0 for unlimited */

# define MOUSE_SENSITIVITY_X	200	/* 1 = 100000 pixels/radian locked camera */
# define MOUSE_SENSITIVITY_Y	200
# define SCROLL_SENSITIVITY		300		/* 1 = 1000 scroll unit/meter */

# define LOOK_CAP_PITCH			1.55f	/* in radian */
# define LOOK_CAP_PITCH_LOCK	0.02f
# define POS_CAP_Y				0.1f	/* in m */
# define CAMERA_SPEED			5.0f	/* in m/s */

/* ########################################################################## */

# define MAX_DELTA				0.03f	/* maximum allowed frame duration */
# define INELASTIC_SPEED		0.5f	/* inelastic collision threshold */
# define COEFF_OF_RESTITUTION	0.2f
# define COEFF_OF_FRICTION		0.8f	/* speed decrease over a second */


/* ########################################################################## */

/* vec3 pseudoclass */
typedef std::array<float, 3>	vec3;

std::ostream	&operator<<(std::ostream &lhs, const vec3 &rhs);
vec3			&operator+=(vec3 &lhs, const vec3 &rhs);
vec3			&operator*=(vec3 &lhs, float rhs);
float			operator*(const vec3 &lhs, const vec3 &rhs);

vec3			operator*(const vec3 &lhs, float rhs);
vec3			operator/(const vec3 &lhs, float rhs);
vec3			operator-(const vec3 &lhs, const vec3 &rhs);
vec3			operator+(const vec3 &lhs, const vec3 &rhs);

float			vec3_length(const vec3 &vec);

#endif
