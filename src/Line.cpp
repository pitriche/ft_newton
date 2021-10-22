/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Line.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:14:54 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/22 12:03:21 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Line.hpp"

Line::Line(void) : origin({0, 0, 0}), dir({0, 0, 0}), length(0) { }

Line::Line(const vec3 &p1, const vec3 &p2)
{
	this->origin = p1;
	this->dir = p2 - p1;
	this->length = vec3_length(this->dir);
	this->dir *= 1.0f / this->length;
}

Line::Line(const Line &src) { *this = src; }
Line::~Line(void) { }

Line	&Line::operator=(const Line &rhs)
{
	this->origin = rhs.origin;
	this->dir = rhs.dir;
	this->length = rhs.length;
	return (*this);
}

std::ostream	&operator<<(std::ostream &lhs, const Line &rhs)
{
	lhs << "Line from " << rhs.origin << " in direction " << rhs.dir <<
	" length " << rhs.length << "m";
	return (lhs);
}