/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Display.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 21:03:42 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/05 17:42:48 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <OpenGL/gl3.h>

#include "Display.hpp"
#include "General.hpp"	/* OPENGL defines */


Display::Display(void) { }
Display::Display(const Display &) { }
Display::~Display(void) { }

void	Display::init(void)
{
	this->res_x = 1920;
	this->res_y = 1080;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		exit(0);

	/* This is out of place, i have no way around */
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
			SDL_GL_CONTEXT_PROFILE_CORE) < 0 ||
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
			OPENGL_VERSION_MAJOR) < 0 ||
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
			OPENGL_VERSION_MINOR) < 0 ||
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8) < 0)
		exit(0);

	/* create window */
	if (!(this->window = SDL_CreateWindow(WIN_TITLE, WIN_POSX, WIN_POSY,
				WIN_SIZEX, WIN_SIZEY, SDL_WINDOW_OPENGL)))
		exit(0);
}

/* ########################################################################## */

#include "Utils.hpp"
#include "All.hpp"

static void	_draw_cube(const Matrix &model)
{
	glUniform1i(all.gl.uniform.object_type, 0);
	glUniformMatrix4fv(all.gl.uniform.matrix_model, 1, true, model.data());
	glUniform1i(all.gl.uniform.color, 0x8080ff);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

static void	_draw_sphere(const Matrix &model)
{
	glUniform1i(all.gl.uniform.object_type, 1);
	glUniformMatrix4fv(all.gl.uniform.matrix_model, 1, true, model.data());
	glUniform1i(all.gl.uniform.color, 0x80ff80);
	glDrawArrays(GL_TRIANGLES, 36, 60);
}

static void	_draw_floor(void)
{
	Matrix model;

	glUniform1i(all.gl.uniform.object_type, 2);
	model.scale(10000, 0, 10000).translate(0, -0.5, 0);	/* top of cube */
	glUniformMatrix4fv(all.gl.uniform.matrix_model, 1, true, model.data());
	glUniform1i(all.gl.uniform.color, 0xf5f5f5);
	glDrawArrays(GL_TRIANGLES, 30, 6);	/* top */
}

/* ########################################################################## */

void		Display::update(const Game &game)
{
	Matrix	view;
	vec3	inv_pos;

	/* camera matrix and look is inverted */
	inv_pos = {-game.pos[0], -game.pos[1], -game.pos[2]};
	if (game.camera_lock == Unlocked)
		view = view.translate(inv_pos).rotate(-game.look_pitch, -game.look_yaw,
			0);
	else if (game.camera_lock == Locked)
		view = view.rotate(-game.look_pitch, -game.look_yaw, 0).translate(0, 0,
			game.pos_locked);
	glUniformMatrix4fv(all.gl.uniform.matrix_view, 1, true, view.data());

	/* draw objects */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (const Object &obj : all.game.obj)
	{
		switch(obj.type)
		{
			case Cube :
				_draw_cube(Matrix().scale(obj.dimension)
					.rotate(obj.angular_position)
					.translate(obj.position));
				break ;

			case Sphere :
				_draw_sphere(Matrix().scale({obj.diameter, obj.diameter, obj.diameter})
					.translate(obj.position));
				break ;
		}
	}
	_draw_floor();

	glFinish();
	SDL_GL_SwapWindow(this->window);

	// std::cout << all.time.fps_average() << std::endl;
}
