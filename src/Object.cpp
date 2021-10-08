/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Object.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:14:54 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/08 10:59:25 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Object.hpp"

Object::Object(void) : rest(false) { }
Object::Object(const Object &src) { *this = src; }
Object::~Object(void) { }

Object	&Object::operator=(const Object &rhs)
{
	this->type = rhs.type;
	this->position = rhs.position;
	this->velocity = rhs.velocity;
	this->mass = rhs.mass;
	this->angular_position = rhs.angular_position;
	this->angular_velocity = rhs.angular_velocity;
	this->dimension = rhs.dimension;
	this->radius = rhs.radius;
	this->rest = rhs.rest;
	return (*this);
}
