/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   General.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 13:52:16 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/05 13:34:58 by pitriche         ###   ########.fr       */
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

# define BASE_FPS		60	/* 0 for unlimited */

# define CAMERA_FOV		75.0f
# define CAMERA_NEAR	1.0f
# define CAMERA_FAR		50.0f

/* 1 is 100000 pixels/radian while locked camera */
# define MOUSE_SENSITIVITY_X	200
# define MOUSE_SENSITIVITY_Y	200
/* 1 is 1000 scroll unit/meter */
# define SCROLL_SENSITIVITY		300

# define LOOK_CAP_PITCH			1.55f
# define LOOK_CAP_PITCH_LOCK	0.1f
# define POS_CAP_Y				0.5f

# define CAMERA_SPEED			8.0f	/* in m/s */

# define MAX_DELTA				0.1f

typedef std::array<float, 3>	vec3;

#endif
