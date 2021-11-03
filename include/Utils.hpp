/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:13:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/11/02 16:39:29 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>	/* std::string */
# include <array>		/* std::array */
# include "Matrix.hpp"	/* Matrix */
# include "Object.hpp"	/* Object */
# include "Line.hpp"	/* Line */
# include <vector>		/* vector */

namespace Utils
{
	void		error_quit(const std::string &str);
	std::string	read_file(const char *filename);
	float		round(float f, int decimal);
	float		square(float f);
	float		max3(const std::array<float, 3>	&vec);
	unsigned	max3_id(const std::array<float, 3>	&vec);
	void		float_cap(float &f, float min, float max);
	void		unsigned_cap(unsigned &n, unsigned min, unsigned max);
	void		add_sphere(std::vector<Object> &list, vec3 position, float
		diameter);
	void		debug_draw_line(std::vector<Object> &list, const Line &line);
}

#endif
