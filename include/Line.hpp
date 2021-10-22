/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Line.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:13:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/22 12:09:04 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_HPP
# define LINE_HPP

# include "General.hpp"	/* vec3 */
# include <iostream>	/* ostream */

class Line
{
	public :
		Line(void);
		Line(const vec3 &p1, const vec3 &p2);
		Line(const Line &src);
		virtual ~Line(void);

		vec3	origin;
		vec3	dir;
		float	length;

		Line	&operator=(const Line &rhs);

	protected:
	private:
};

std::ostream	&operator<<(std::ostream &lhs, const Line &rhs);

#endif
