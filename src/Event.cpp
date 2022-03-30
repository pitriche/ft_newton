/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 21:03:42 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/19 10:21:42 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Event.hpp"
#include "All.hpp"
#include "Utils.hpp"	/* cap */
#include <cmath>		/* floor */

Event::Event(void)
{
	this->key.object_number = DEFAULT_OBJ_NB;
	this->key.object_mass = DEFAULT_OBJ_MASS;
	this->key.object_speed = DEFAULT_OBJ_SPEED;
	this->key.object_size = DEFAULT_OBJ_SIZE;
	this->key.wind_speed = DEFAULT_WIND_SPEED;
	this->key.max_distance = INFINITY;
	this->key.gravity = DEFAULT_GRAVITY;
	this->key.time_speed = DEFAULT_TIME;
}

Event::Event(const Event &) { }
Event::~Event(void) { }

void	Event::_mousechange(Uint8 button, bool button_state)
{
	switch (button)
	{
		case (SDL_BUTTON_LEFT) :
			this->key.object_thrown = button_state;
			this->key.object_type = 1;	/* Sphere */
			break;
		case (SDL_BUTTON_MIDDLE) :
			if (button_state)
				this->key.mouse_middle ^= 1;
			break;
		case (SDL_BUTTON_RIGHT) :
			this->key.mouse_right = button_state;
			/* throw cube */
			// this->key.object_thrown = button_state;
			// this->key.object_type = 0;	/* Cube */
			break;
	}
}

void	Event::_keychange(SDL_Keycode kc, bool key_state)
{
	switch (kc)		/* key press and release */
	{
		case (SDLK_ESCAPE) : exit(0); break;
		case (SDLK_w) : this->key.w = key_state; break;				/* camera horizontal displacement */
		case (SDLK_a) : this->key.a = key_state; break;				/* | */
		case (SDLK_s) : this->key.s = key_state; break;				/* | */
		case (SDLK_d) : this->key.d = key_state; break;				/* | */
		case (SDLK_LSHIFT) : this->key.lshift = key_state; break;	/* camera vertical displacement */
		case (SDLK_SPACE) : this->key.space = key_state; break;		/* | */
	}

	if (!key_state)	/* only on key press */
		return ;
	switch (kc)
	{
		case (SDLK_r) : this->key.object_speed *= 1.1f; break;
		case (SDLK_f) : this->key.object_speed *= 0.9f; break;
		case (SDLK_v) : this->key.object_speed = DEFAULT_OBJ_SPEED; break;
		case (SDLK_t) : this->key.object_mass *= 1.5f; break;
		case (SDLK_g) : this->key.object_mass -= 1.0f; break;
		case (SDLK_b) : this->key.object_mass = DEFAULT_OBJ_MASS; break;
		case (SDLK_y) : this->key.object_number += 3; break;
		case (SDLK_h) : this->key.object_number -= 1; break;
		case (SDLK_n) : this->key.object_number = DEFAULT_OBJ_NB; break;
		case (SDLK_u) : this->key.object_size *= 1.2f; break;
		case (SDLK_j) : this->key.object_size *= 0.9f; break;
		case (SDLK_m) : this->key.object_size = DEFAULT_OBJ_SIZE; break;
		case (SDLK_EQUALS) : this->key.max_distance += 50.0f; break;		/* object despawn distance */
		case (SDLK_MINUS) : this->key.max_distance -= 50.0f; break;			/* | */
		case (SDLK_BACKSPACE) : this->key.max_distance = INFINITY; break;	/* | */
		case (SDLK_RIGHTBRACKET) : this->key.gravity *= 1.1f; break;		/* gravity control */
		case (SDLK_LEFTBRACKET) : this->key.gravity *= 0.9f; break;			/* | */
		case (SDLK_BACKSLASH) : this->key.gravity = DEFAULT_GRAVITY; break;	/* | */
		case (SDLK_x) : this->key.wind_speed *= 1.2f; break;				/* wind control */
		case (SDLK_z) : this->key.wind_speed *= 0.8f; break;				/* | */
		case (SDLK_c) : this->key.wind_speed = DEFAULT_WIND_SPEED; break;	/* | */
		case (SDLK_PERIOD) : this->key.time_speed *= 1.5f; break;			/* accelerate time */
		case (SDLK_COMMA) : this->key.time_speed *= 0.5f; break;			/* slow down time */
		case (SDLK_SLASH) : this->key.time_speed = DEFAULT_TIME; break;		/* normal time */
		case (SDLK_RSHIFT) : this->key.time_speed = 1e-20f; break;			/* stop time (the world) */
		case (SDLK_q) : this->key.object_auto ^= 1; break;					/* throw object each frame */
		case (SDLK_e) : this->key.debug_display ^= 1; break;				/* wireframe, useless */
		case (SDLK_o) : this->key.debug_impact ^= 1; break;					/* display hits and angular impulses */
		case (SDLK_1) : this->key.init_pile ^= 1; break;					/* add a pile of spheres */
		case (SDLK_2) : this->key.init_pool ^= 1; break;					/* add a nice square */
		case (SDLK_3) : this->key.init_tower ^= 1; break;					/* add 2 piles of cubes */
		case (SDLK_0) : this->key.reset_all ^= 1; break;					/* remove all objects */
	}
	if ((kc == SDLK_EQUALS || kc == SDLK_MINUS) &&
		isinf(this->key.max_distance))
		this->key.max_distance = DEFAULT_MAX_DIST;
	Utils::float_cap(this->key.object_speed, 0.0f, 1e20f);
	Utils::float_cap(this->key.object_mass, 1.0f, 1e20f);
	Utils::float_cap(this->key.object_size, 1e-20f, 1e20f);
	Utils::unsigned_cap(this->key.object_number, 1, 1000000);
	Utils::float_cap(this->key.max_distance, 50.0f, INFINITY);
	Utils::float_cap(this->key.time_speed, 1e-20f, INFINITY);
}

void	Event::update(void)
{
	SDL_Event event;

	this->key.mouse_scroll = false;
	this->key.mouse_x = false;
	this->key.mouse_y = false;
	if (!this->key.object_auto)
		this->key.object_thrown = false;

	this->key.init_pile = false;
	this->key.init_pool = false;
	this->key.init_tower = false;
	this->key.reset_all = false;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			exit(0);

		if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) &&
			!event.key.repeat)
			this->_keychange(event.key.keysym.sym, event.type == SDL_KEYDOWN);

		if (event.type == SDL_MOUSEBUTTONDOWN ||
			event.type == SDL_MOUSEBUTTONUP)
			this->_mousechange(event.button.button, event.type ==
				SDL_MOUSEBUTTONDOWN);

		if (event.type == SDL_MOUSEWHEEL)
			this->key.mouse_scroll = event.wheel.y;

		if (event.type == SDL_MOUSEMOTION &&
			(this->key.mouse_right || !this->key.mouse_middle))
		{
			this->key.mouse_x = event.motion.xrel;
			this->key.mouse_y = event.motion.yrel;
		}
	}

	std::cout << "Object: " << this->key.object_size << "m, " <<
	this->key.object_mass << "kg, " <<
	this->key.object_speed << "m/s, " <<
	this->key.object_number << " spawned" << std::endl;

	std::cout << "Game status: " <<
	this->key.time_speed * 100.0f << "% time speed, gravity: " <<
	Utils::round(this->key.gravity, 2) << "m/s^2 (" <<
	Utils::round(this->key.gravity / 9.80665f, 2) << "g), " <<
	this->key.max_distance << "m maximum object distance" << std::endl;
	std::cout << "Wind speed: " << this->key.wind_speed << "m/s" << std::endl << std::endl;
}
