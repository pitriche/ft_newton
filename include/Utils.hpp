/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:13:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/06 09:47:15 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>	/* std::string */
# include <array>		/* std::array */
# include "Matrix.hpp"	/* Matrix */

namespace Utils
{
	void		error_quit(const std::string &str);
	std::string	read_file(const char *filename);
	float		round(float f, int decimal);
	float		max3(const std::array<float, 3>	&vec);
}

#endif
