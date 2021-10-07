/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Display.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 21:03:42 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/07 18:27:39 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <OpenGL/gl3.h>

#include "Display.hpp"
#include "General.hpp"	/* OPENGL defines */
#include "All.hpp"

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

static void	_push_matrix(std::vector<float> &mat_array, const Matrix &mat)
{
	mat_array.insert(mat_array.end(), mat.begin(), mat.end());
}

static void	_draw_objects(const Game &game)
{
	std::vector<float>	spheres_mat;
	std::vector<float>	cubes_mat;

	cubes_mat.reserve(game.obj.size() * 16);
	spheres_mat.reserve(game.obj.size() * 16);
	for (const Object &obj : game.obj)
	{
		switch(obj.type)
		{
			case Cube :
				_push_matrix(cubes_mat, Matrix().scale(obj.dimension)
					.rotate(obj.angular_position)
					.translate(obj.position).transpose());
				break ;
			case Sphere :
				_push_matrix(spheres_mat, Matrix().scale({obj.diameter, obj.diameter, obj.diameter})
					.translate(obj.position).transpose());
				break ;
		}
	}

	/* draw cubes */
	glBindBuffer(GL_ARRAY_BUFFER, all.gl.terrain.vbo_matrix);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(cubes_mat.size() *
		sizeof(float)), cubes_mat.data(), GL_DYNAMIC_DRAW);
	glUniform1i(all.gl.uniform.object_type, 0);
	glUniform1i(all.gl.uniform.color, 0x8080ff);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, (GLsizei)(cubes_mat.size() /
		16));
	
	/* draw spheres */
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(spheres_mat.size() *
		sizeof(float)), spheres_mat.data(), GL_DYNAMIC_DRAW);
	glUniform1i(all.gl.uniform.object_type, 1);
	glUniform1i(all.gl.uniform.color, 0x80ff80);
	glDrawArraysInstanced(GL_TRIANGLES, 36, 60 * 4 * 4,
		(GLsizei)(spheres_mat.size() / 16));
}

static void	_draw_floor(void)
{
	Matrix	model;

	model = Matrix().scale(10000, 0, 10000);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(16 * sizeof(float)),
		model.data(), GL_DYNAMIC_DRAW);
	glUniform1i(all.gl.uniform.object_type, 2);
	glUniform1i(all.gl.uniform.color, 0xe5e5e5);
	glDrawArraysInstanced(GL_TRIANGLES, 30, 6, 1);
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
	_draw_objects(game);
	_draw_floor();

	glFinish();
	SDL_GL_SwapWindow(this->window);

	std::cout << all.time.fps_average() << std::endl;
}
