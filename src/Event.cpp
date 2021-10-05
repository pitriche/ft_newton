/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 21:03:42 by pitriche          #+#    #+#             */
/*   Updated: 2021/09/22 15:04:26 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Event.hpp"
#include "All.hpp"

Event::Event(void) { }
Event::Event(const Event &) { }
Event::~Event(void) { }

void	Event::_keychange(SDL_Keycode kc, unsigned key_state)
{
	switch (kc)
	{
		case (SDLK_ESCAPE) : exit(0); break;

		case (SDLK_w) : this->key.w = key_state; break;
		case (SDLK_a) : this->key.a = key_state; break;
		case (SDLK_s) : this->key.s = key_state; break;
		case (SDLK_d) : this->key.d = key_state; break;
		case (SDLK_LSHIFT) : this->key.lshift = key_state; break;
		case (SDLK_SPACE) : this->key.space = key_state; break;
	}
}

void	Event::update(void)
{
	SDL_Event event;

	this->key.mouse_scroll = 0;
	this->key.mouse_x = 0;
	this->key.mouse_y = 0;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			exit(0);

		if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) &&
			!event.key.repeat)
			this->_keychange(event.key.keysym.sym, event.type == SDL_KEYDOWN);

		if (event.type == SDL_MOUSEBUTTONDOWN ||
			event.type == SDL_MOUSEBUTTONUP)
		{
			if (event.button.button == SDL_BUTTON_RIGHT)
				this->key.mouse_right = (event.type == SDL_MOUSEBUTTONDOWN);
			if (event.button.button == SDL_BUTTON_MIDDLE &&
				event.type == SDL_MOUSEBUTTONDOWN)
				this->key.mouse_middle ^= 1;
		}

		if (event.type == SDL_MOUSEWHEEL)
			this->key.mouse_scroll = event.wheel.y;

		if (event.type == SDL_MOUSEMOTION &&
			(this->key.mouse_right || !this->key.mouse_middle))
		{
			this->key.mouse_x = event.motion.xrel;
			this->key.mouse_y = event.motion.yrel;
		}
	}
}
