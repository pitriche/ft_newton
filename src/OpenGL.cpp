/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpenGL.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:14:54 by pitriche          #+#    #+#             */
/*   Updated: 2021/-/27 12:54:54 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL.hpp"
#include "General.hpp"
#include "Utils.hpp"

#define	PI2_5		1.25663706144f	/* 2Pi / 5 */
#define COS_ANGLE	0.4472135955f	/* 0.5 * cos(arctan(1/2)) */
#define SIN_ANGLE	0.22360679775f	/* 0.5 * sin(arctan(1/2)) */

OpenGL::OpenGL(void) { }

/* ########################################################################## */

/* All triangles are CCW */
static void	_init_cube(float *buffer)
{
	static const float	cube_data[] =
	{
		1,	-1,	-1,		-1,	1,	-1,		-1,	-1,	-1,	/* front */
		-1,	1,	-1,		1,	-1,	-1,		1,	1,	-1,
		-1,	1,	1,		1,	-1,	1,		-1,	-1,	1,	/* back */
		-1,	1,	1,		1,	1,	1,		1,	-1,	1,
		-1,	1,	1,		-1,	-1,	1,		-1,	-1,	-1,	/* left */
		-1,	1,	1,		-1,	-1,	-1,		-1,	1,	-1,
		1,	1,	1,		1,	-1,	-1,		1,	-1,	1,	/* right */
		1,	1,	1,		1,	1,	-1,		1,	-1,	-1,
		-1,	-1,	-1,		-1,	-1,	1,		1,	-1,	1,	/* bottom */
		-1,	-1,	-1,		1,	-1,	1,		1,	-1,	-1,
		-1,	1,	-1,		1,	1,	1,		-1,	1,	1,	/* top */
		-1,	1,	-1,		1,	1,	-1,		1,	1,	1,
	};

	for (unsigned i = 0; i < 108; ++i)
		buffer[i] = cube_data[i] / 2.0f;	/* bring side length to 1 */
}

static void	_triangle_to_buffer(float *buffer, const vec3 &p1, const vec3 &p2,
	const vec3 &p3)
{
	memcpy(buffer + 0, p1.data(), 3 * sizeof(float));
	memcpy(buffer + 3, p2.data(), 3 * sizeof(float));
	memcpy(buffer + 6, p3.data(), 3 * sizeof(float));
}

std::ostream	&operator<<(std::ostream &lhs, vec3 &rhs)
{
	lhs << "[" << rhs[0] << ", " << rhs[1] << ", " << rhs[2] << "]" << std::endl;
	return (lhs);
}

static void	_init_icosahedron(float *buffer)
{
	vec3	vertex[12];
	float	row1_angle;
	float	row2_angle;

	vertex[0] = {0, 0.5f, 0};	/* top */
	vertex[11] = {0, -0.5f, 0};	/* bottom */
	for (unsigned i = 0; i < 5; ++i)
	{
		row1_angle = i * PI2_5;
		row2_angle = i * PI2_5 + (PI2_5 / 2.0f);
		vertex[i + 1] = {COS_ANGLE * cos(row1_angle), SIN_ANGLE, COS_ANGLE * sin(row1_angle)};
		vertex[i + 6] = {COS_ANGLE * cos(row2_angle), -SIN_ANGLE, COS_ANGLE * sin(row2_angle)};;
	}

	/* top */
	_triangle_to_buffer(buffer + 0 * 9, vertex[0], vertex[1], vertex[2]);
	_triangle_to_buffer(buffer + 1 * 9, vertex[0], vertex[2], vertex[3]);
	_triangle_to_buffer(buffer + 2 * 9, vertex[0], vertex[3], vertex[4]);
	_triangle_to_buffer(buffer + 3 * 9, vertex[0], vertex[4], vertex[5]);
	_triangle_to_buffer(buffer + 4 * 9, vertex[0], vertex[5], vertex[1]);

	/* middle */
	_triangle_to_buffer(buffer + 5 * 9, vertex[1], vertex[6], vertex[2]);
	_triangle_to_buffer(buffer + 6 * 9, vertex[6], vertex[7], vertex[2]);
	_triangle_to_buffer(buffer + 7 * 9, vertex[2], vertex[7], vertex[3]);
	_triangle_to_buffer(buffer + 8 * 9, vertex[7], vertex[8], vertex[3]);
	_triangle_to_buffer(buffer + 9 * 9, vertex[3], vertex[8], vertex[4]);
	_triangle_to_buffer(buffer + 10 * 9, vertex[8], vertex[9], vertex[4]);
	_triangle_to_buffer(buffer + 11 * 9, vertex[4], vertex[9], vertex[5]);
	_triangle_to_buffer(buffer + 12 * 9, vertex[9], vertex[10], vertex[5]);
	_triangle_to_buffer(buffer + 13 * 9, vertex[5], vertex[10], vertex[1]);
	_triangle_to_buffer(buffer + 14 * 9, vertex[10], vertex[6], vertex[1]);

	/* bottom */
	_triangle_to_buffer(buffer + 15 * 9, vertex[11], vertex[7], vertex[6]);
	_triangle_to_buffer(buffer + 16 * 9, vertex[11], vertex[8], vertex[7]);
	_triangle_to_buffer(buffer + 17 * 9, vertex[11], vertex[9], vertex[8]);
	_triangle_to_buffer(buffer + 18 * 9, vertex[11], vertex[10], vertex[9]);
	_triangle_to_buffer(buffer + 19 * 9, vertex[11], vertex[6], vertex[10]);
}

/* ########################################################################## */
/* #####################	Buffers & Attributes		##################### */
/* ########################################################################## */

static GLuint locate_attribute(GLuint program, const char *attribute_name)
{
	GLint	attribute;

	if ((attribute = glGetAttribLocation(program, attribute_name)) < 0)
		Utils::error_quit(std::string("Invalid attribute : \"") +
			attribute_name + "\", return " + std::to_string(attribute));
	return ((GLuint)attribute);
}

void	OpenGL::_init_vao(void)
{
	float	vbo_data[(6 * 6 + 20 * 3) * 3];

	_init_cube(vbo_data);
	_init_icosahedron(vbo_data + 108);

	/* init and select vao */
	glGenVertexArrays(1, &this->terrain.vao);
	glBindVertexArray(this->terrain.vao);
	std::cout << "VAO:[" << this->terrain.vao << "/1]\t";

	/* init and fill vbo with cube data */
	glGenBuffers(1, &this->terrain.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->terrain.vbo);
	std::cout << "VBO:[" << this->terrain.vbo << "/1]" << std::endl;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data), vbo_data, GL_STATIC_DRAW);

	/* init attributes */
	this->terrain.att.position_vertex = locate_attribute(this->shader.program,
		"position_vertex");
	glVertexAttribPointer(this->terrain.att.position_vertex, 3, GL_FLOAT,
		GL_FALSE, sizeof(float) * 3, (void *)(0));
	glEnableVertexAttribArray(this->terrain.att.position_vertex);
}

/* ########################################################################## */
/* #####################			Shaders				##################### */
/* ########################################################################## */

void			openGL_error_log(unsigned int shader, int err_code,
	const char *filename)
{
	char	buffer[512];

	std::cerr << "[" << filename << "] shader compilation failed: [" <<
	err_code << "/1]" << std::endl;
	glGetShaderInfoLog(shader, 512, NULL, buffer);
	std::cerr << "Compilation log:" << std::endl << buffer;
	exit(0);
}

static GLuint	compile_shader(const char *filename, GLenum type)
{
	GLuint		shader;
	std::string	scode;
	const char	*code;
	GLint		status;

	/* fetch code in stack */
	scode = Utils::read_file(filename);
	code = scode.c_str();
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &code, 0);

	/* compile and check for errors */
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
		openGL_error_log(shader, status, filename);
	return (shader);
}

void			OpenGL::_init_shader(void)
{
	this->shader.vertex = compile_shader("shader/vertex.glsl",
		GL_VERTEX_SHADER);
	this->shader.geometry = compile_shader("shader/geometry.glsl",
		GL_GEOMETRY_SHADER);
	this->shader.fragment = compile_shader("shader/fragment.glsl",
		GL_FRAGMENT_SHADER);

	/* create shader program and attach shaders */
	this->shader.program = glCreateProgram();
	std::cout << "Program: [" << this->shader.program << "/3]" << std::endl;
	glAttachShader(this->shader.program, this->shader.vertex);
	glAttachShader(this->shader.program, this->shader.geometry);
	glAttachShader(this->shader.program, this->shader.fragment);
	glBindFragDataLocation(this->shader.program, 0, "outColor");

	/* link and use shader program */
	glLinkProgram(this->shader.program);
	glUseProgram(this->shader.program);
}

/* ########################################################################## */
/* #####################		Uniforms & Data			##################### */
/* ########################################################################## */

static GLint locate_uniform(GLuint program, const char *uniform_name)
{
	GLint	uniform;

	if ((uniform = glGetUniformLocation(program, uniform_name)) < 0)
		Utils::error_quit(std::string("Invalid uniform : \"") + uniform_name +
			"\", return " + std::to_string(uniform));
	return (uniform);
}

void	OpenGL::_init_uniform(void)
{
	this->uniform.matrix_view = locate_uniform(this->shader.program,
		"matrix_view");
	this->uniform.matrix_model = locate_uniform(this->shader.program,
		"matrix_model");

	this->uniform.color = locate_uniform(this->shader.program,
		"color");
	this->uniform.fov_mult = locate_uniform(this->shader.program,
		"fov_mult");

	this->uniform.object_type = locate_uniform(this->shader.program,
		"object_type");
}

void	OpenGL::_init_matrix(void)
{
	this->matrix_model = Matrix();
	this->matrix_view = Matrix();

	glUniformMatrix4fv(this->uniform.matrix_model, 1, true,
		this->matrix_model.data());
	glUniformMatrix4fv(this->uniform.matrix_view, 1, true,
		this->matrix_view.data());
}

/* ########################################################################## */
/* #####################			Main init			##################### */
/* ########################################################################## */

void	OpenGL::init(SDL_Window *window)
{
	/* create OpenGL context */
	this->glcontext = SDL_GL_CreateContext(window);
	std::cout << "Supported OpenGL version: " << glGetString(GL_VERSION) <<
		std::endl << "Using OpenGL " << OPENGL_VERSION_MAJOR << '.' <<
		OPENGL_VERSION_MINOR << std::endl << std::endl;

	this->_init_shader();
	this->_init_uniform();
	this->_init_vao();
	this->_init_matrix();

	glUniform1f(this->uniform.fov_mult, 1.4f); /* set default FOV */
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.243f, 0.243f, 0.241f, 1.0f);
}
