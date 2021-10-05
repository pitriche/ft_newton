/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   All.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 21:03:42 by pitriche          #+#    #+#             */
/*   Updated: 2021/09/22 14:57:32 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "All.hpp"
#include "General.hpp"

All::All(void) { }
All::All(const All &) { }
All::~All(void) { }

void	All::init(void)
{
	this->disp.init();
	this->gl.init(this->disp.window);
	this->time.set_fps(BASE_FPS);
	this->game.init();
}

void	All::operator=(const All &) { }
