/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Display.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 21:03:42 by pitriche          #+#    #+#             */
/*   Updated: 2021/11/10 16:11:16 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <OpenGL/gl3.h>

#include "Display.hpp"
#include "General.hpp"	/* OPENGL defines */
#include "Utils.hpp"	/* error_quit */
#include "All.hpp"
#include <vector>

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

static void	_draw_catapult(const Game &game)
{
	std::vector<float>	cubes_mat;
	Matrix				tmp;

	for (const Object &obj : game.sling)
	{
		tmp = Matrix().scale(obj.dimension)
			.rotate_euler(obj.angular_position)
			.translate(obj.position)
			.rotate(game.look_pitch, game.look_yaw, 0)
			.translate(game.pos).transpose();
		_push_matrix(cubes_mat, tmp);
	}
	glBindBuffer(GL_ARRAY_BUFFER, all.gl.terrain.vbo_matrix);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(cubes_mat.size() *
		sizeof(float)), cubes_mat.data(), GL_DYNAMIC_DRAW);
	glUniform1i(all.gl.uniform.object_type, 0);
	glUniform1i(all.gl.uniform.color, 0x8b4513);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, (GLsizei)(cubes_mat.size() /
			16));
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
					.rotate_euler(obj.angular_position)
					.translate(obj.position).transpose());
				break ;
			case Sphere :
				_push_matrix(spheres_mat, (Matrix() * (obj.radius * 2.0f))
					.translate(obj.position).transpose());
				break ;
			default:
				Utils::error_quit("Undefined Object Type");
				break;
		}
	}

	if (all.event.key.debug_display) /* enable wireframe if needed */
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable	(GL_CULL_FACE);
	}

	/* draw cubes */
	glBindBuffer(GL_ARRAY_BUFFER, all.gl.terrain.vbo_matrix);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(cubes_mat.size() *
		sizeof(float)), cubes_mat.data(), GL_DYNAMIC_DRAW);
	glUniform1i(all.gl.uniform.object_type, 0);
	glUniform1i(all.gl.uniform.color, 0x8080ff); // all sides
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, (GLsizei)(cubes_mat.size() /
			16));

	/* draw spheres */
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(spheres_mat.size() *
		sizeof(float)), spheres_mat.data(), GL_DYNAMIC_DRAW);
	glUniform1i(all.gl.uniform.object_type, 1);
	glUniform1i(all.gl.uniform.color, 0x80ff80);
	glDrawArraysInstanced(GL_TRIANGLES, 36, 60 * 4 * 4,
		(GLsizei)(spheres_mat.size() / 16));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); /* disable wireframe */
	glEnable(GL_CULL_FACE);

	/* draw debug spheres */
	spheres_mat.clear();
	spheres_mat.reserve(game.debug.size() * 16);
	for (const Object &sphere : game.debug)
		_push_matrix(spheres_mat, (Matrix() * (sphere.radius * 2.0f))
			.translate(sphere.position).transpose());
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(spheres_mat.size() *
		sizeof(float)), spheres_mat.data(), GL_DYNAMIC_DRAW);
	glUniform1i(all.gl.uniform.object_type, 1);
	glUniform1i(all.gl.uniform.color, 0xff8080);
	glDrawArraysInstanced(GL_TRIANGLES, 36, 60 * 4 * 4,
		(GLsizei)(spheres_mat.size() / 16));
	
	if (all.event.key.mouse_middle)	/* draw catapult */
		_draw_catapult(game);
}

static void	_draw_floor(void)
{
	Matrix	model;

	model = Matrix().scale(2000, 0, 3000);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(16 * sizeof(float)),
		model.data(), GL_DYNAMIC_DRAW);
	glUniform1i(all.gl.uniform.object_type, 2);
	glUniform1i(all.gl.uniform.color, 0xe5e5e5);
	glDrawArraysInstanced(GL_TRIANGLES, 30, 6, 1);	/* top of cube */
}

/* ########################################################################## */

void		Display::update(const Game &game)
{
	Matrix	view;
	Matrix	view_rotate;
	vec3	inv_pos;

	/* camera position is inverted */
	inv_pos = {-game.pos[0], -game.pos[1], -game.pos[2]};

	/* invert roll-pitch-yaw order because camera direction is inverted */
	view_rotate.rotate(0, -game.look_yaw, 0);
	view_rotate.rotate(-game.look_pitch, 0, 0);
	if (game.camera_lock == Unlocked)
		view = view_rotate * view.translate(inv_pos);
	else if (game.camera_lock == Locked)
		view = (view_rotate * view).translate(0, 0, game.pos_locked);
	glUniformMatrix4fv(all.gl.uniform.matrix_view, 1, true, view.data());

	/* draw objects */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_draw_objects(game);
	_draw_floor();

	glFinish();
	SDL_GL_SwapWindow(this->window);

	std::cout << all.time.fps_average() << ", Objects:" << game.obj.size() << std::endl;
}
