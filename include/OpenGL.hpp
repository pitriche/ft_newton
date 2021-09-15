/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpenGL.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:13:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/09/13 15:58:48 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENGL_HPP
# define OPENGL_HPP

#ifdef __MACH__
 #include <OpenGL/gl3.h>
#endif
#ifndef __MACH__
 #include <GL/gl.h>
 #define GL_GLEXT_PROTOTYPES
 #include <GL/glext.h>
#endif


# include "SDL.h"
# include "Matrix.hpp"

struct	_Uniform
{
	GLint	matrix_proj;
	GLint	matrix_view;
	GLint	matrix_model;

	GLint	color;
};

struct	_Shader
{
	GLuint	vertex;
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
		};

	public :
		GLuint		vao;
		GLuint		vbo;
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

		Matrix			matrix_proj;
		Matrix			matrix_view;
		Matrix			matrix_model;

		void	init(SDL_Window *window);

	private:
		void	_init_vao_terrain(void);
		void	_init_shader(void);
		void	_init_uniform(void);
		void	_init_matrix(void);
};

#endif
