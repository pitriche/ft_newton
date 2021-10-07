/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpenGL.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:13:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/06 15:39:13 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENGL_HPP
# define OPENGL_HPP

# include <OpenGL/gl3.h>

# include "SDL.h"
# include "Matrix.hpp"

struct	_Uniform
{
	GLint	matrix_view;
	GLint	matrix_model;

	GLint	color;
	GLint	fov_mult;

	GLint	object_type;
};

struct	_Shader
{
	GLuint	vertex;
	GLuint	geometry;
	GLuint	fragment;

	GLuint	program;
};

/* ########################################################################## */

struct	VAO
{
	private :
		struct _Attribute
		{
			GLuint	position_vertex;
			GLuint	model_matrix_inst;
		};

	public :
		GLuint		vao;
		GLuint		vbo;
		GLuint		vbo_matrix;
		_Attribute	att;


};

struct OpenGL
{
	public :
		OpenGL(void);
		SDL_Window		*window;
		SDL_GLContext	glcontext;

		VAO				terrain;
		_Shader			shader;
		_Uniform		uniform;

		Matrix			matrix_view;
		Matrix			matrix_model;

		void	init(SDL_Window *window);

	private:
		void	_init_vao(void);
		void	_init_shader(void);
		void	_init_uniform(void);
		void	_init_matrix(void);
};

#endif
