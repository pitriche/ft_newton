/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:14:54 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/21 12:13:54 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include <iostream>		/* cerr, string */
#include <fstream>		/* ifstream */
#include <sstream>		/* stringstream */
#include <cstdlib>		/* exit */
#include <cmath>		/* round */


namespace Utils
{
	void		error_quit(const std::string &str)
	{
		std::cerr << str << std::endl;
		exit(0);
	}

	std::string	read_file(const char *filename)
	{
		std::ifstream		fs;
		std::stringstream	ss;

		fs.open(filename);
		if (!fs.is_open())
			error_quit("Cannot open source file");
		ss << fs.rdbuf();
		return (ss.str());
	}

	float		round(float f, int decimal)
	{
		float	n;

		n = (float)std::pow(10.0f, decimal);
		return (std::round(f * n) / n);
	}

	float		square(float f)
	{
		return (f * f);
	}

	float		max3(const std::array<float, 3>	&vec)
	{
		if (vec[0] >= vec[1] && vec[0] >= vec[2])
			return (vec[0]);
		if (vec[1] >= vec[0] && vec[1] >= vec[2])
			return (vec[1]);
		return (vec[2]);
	}

	/* return index of maximum value */
	unsigned	max3_id(const std::array<float, 3>	&vec)
	{
		if (vec[0] >= vec[1] && vec[0] >= vec[2])
			return (0);
		if (vec[1] >= vec[0] && vec[1] >= vec[2])
			return (1);
		return (2);
	}

	/* indifferent if min > max */
	void		float_cap(float &f, float min, float max)
	{
		if (f < min)
			f = min;
		if (f > max)
			f = max;
	}

	void		unsigned_cap(unsigned &n, unsigned min, unsigned max)
	{
		if (n < min)
			n = min;
		if (n > max)
			n = max;
	}
}
