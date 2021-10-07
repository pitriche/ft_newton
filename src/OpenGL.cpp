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
#include <vector>

/*
	requires change at:
	Display.cpp: _draw_objects (draw call)
	OpenGL.cpp: init_vao (vbo_data size)
*/
#define FACE_SUBDIVISION 2

#define	PI2_5		1.25663706144f	/* 2Pi / 5 */
#define COS_ANGLE	0.4472135955f	/* 0.5 * cos(arctan(1/2)) */
#define SIN_ANGLE	0.22360679775f	/* 0.5 * sin(arctan(1/2)) */

OpenGL::OpenGL(void) { }

/* ########################################################################## */
/* #####################		VBO Data initialize		##################### */
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

static void	_triangle_to_buffer(std::vector<float> &data, const vec3 &p1, const vec3 &p2,
	const vec3 &p3)
{
	data.insert(data.end(), p1.begin(), p1.end());
	data.insert(data.end(), p2.begin(), p2.end());
	data.insert(data.end(), p3.begin(), p3.end());
}

static void	_icosahedron_face_subdivide(std::vector<float> &data, const vec3 &p1,
	const vec3 &p2, const vec3 &p3, int division)
{
	vec3	vertex[6];
	float	scale_radius;

	if (division == 0)
		_triangle_to_buffer(data, p1, p2, p3);
	else
	{

		vertex[0] = p1;
		vertex[3] = p2;
		vertex[5] = p3;

		vertex[1] = p1 + ((p2 - p1) / 2.0f);
		vertex[2] = p1 + ((p3 - p1) / 2.0f);
		vertex[4] = p2 + ((p3 - p2) / 2.0f);

		/* expand middle point towards radius (each have different ratio) */
		scale_radius = 0.5f / sqrt(vertex[1] * vertex[1]);
		vertex[1] *= scale_radius;

		scale_radius = 0.5f / sqrt(vertex[2] * vertex[2]);
		vertex[2] *= scale_radius;

		scale_radius = 0.5f / sqrt(vertex[4] * vertex[4]);
		vertex[4] *= scale_radius;

		/* recursive calls */
		_icosahedron_face_subdivide(data, vertex[0], vertex[1], vertex[2], division - 1);
		_icosahedron_face_subdivide(data, vertex[1], vertex[3], vertex[4], division - 1);
		_icosahedron_face_subdivide(data, vertex[1], vertex[4], vertex[2], division - 1);
		_icosahedron_face_subdivide(data, vertex[2], vertex[4], vertex[5], division - 1);
	}
}

static void	_init_icosahedron(float *buffer)
{
	vec3				vertex[12];
	float				row1_angle;
	float				row2_angle;
	std::vector<float>	data;

	vertex[0] = {0, 0.5f, 0};	/* top */
	vertex[11] = {0, -0.5f, 0};	/* bottom */
	for (unsigned i = 0; i < 5; ++i)
	{
		row1_angle = i * PI2_5;
		row2_angle = i * PI2_5 + (PI2_5 / 2.0f);
		vertex[i + 1] = {COS_ANGLE * cos(row1_angle), SIN_ANGLE,
			COS_ANGLE * sin(row1_angle)};
		vertex[i + 6] = {COS_ANGLE * cos(row2_angle), -SIN_ANGLE,
			COS_ANGLE * sin(row2_angle)};;
	}

	/* top */
	_icosahedron_face_subdivide(data, vertex[0], vertex[1], vertex[2], FACE_SUBDIVISION); /* top */
	_icosahedron_face_subdivide(data, vertex[0], vertex[2], vertex[3], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[0], vertex[3], vertex[4], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[0], vertex[4], vertex[5], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[0], vertex[5], vertex[1], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[1], vertex[6], vertex[2], FACE_SUBDIVISION); /* middle */
	_icosahedron_face_subdivide(data, vertex[6], vertex[7], vertex[2], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[2], vertex[7], vertex[3], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[7], vertex[8], vertex[3], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[3], vertex[8], vertex[4], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[8], vertex[9], vertex[4], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[4], vertex[9], vertex[5], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[9], vertex[10], vertex[5], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[5], vertex[10], vertex[1], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[10], vertex[6], vertex[1], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[11], vertex[7], vertex[6], FACE_SUBDIVISION); /* bottom */
	_icosahedron_face_subdivide(data, vertex[11], vertex[8], vertex[7], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[11], vertex[9], vertex[8], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[11], vertex[10], vertex[9], FACE_SUBDIVISION);
	_icosahedron_face_subdivide(data, vertex[11], vertex[6], vertex[10], FACE_SUBDIVISION);
	memcpy(buffer, data.data(), data.size() * sizeof(float));

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
	float	vbo_data[((6 * 6) + (20 * 4 * 4 * 3)) * 3];
	GLuint	mm_att;

	_init_cube(vbo_data);
	_init_icosahedron(vbo_data + 108);

	/* init and select vao */
	glGenVertexArrays(1, &this->terrain.vao);
	glBindVertexArray(this->terrain.vao);
	std::cout << "VAO:[" << this->terrain.vao << "/1]\t";

	/* init and fill vbo with objects data */
	glGenBuffers(1, &this->terrain.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->terrain.vbo);
	std::cout << "VBO:[" << this->terrain.vbo << "/1]\t";
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data), vbo_data, GL_STATIC_DRAW);

	/* init vbo for matrix data */
	glGenBuffers(1, &this->terrain.vbo_matrix);
	std::cout << "matrix VBO:[" << this->terrain.vbo_matrix << "/2]" << std::endl;


	/* init attributes */
	this->terrain.att.position_vertex = locate_attribute(this->shader.program,
		"position_vertex");
	glVertexAttribPointer(this->terrain.att.position_vertex, 3, GL_FLOAT,
		GL_FALSE, sizeof(float) * 3, (void *)(0));
	glEnableVertexAttribArray(this->terrain.att.position_vertex);

	glBindBuffer(GL_ARRAY_BUFFER, this->terrain.vbo_matrix);
	mm_att = locate_attribute(this->shader.program, "model_matrix_inst");
	this->terrain.att.model_matrix_inst = 1;
	glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 64, (void*)0);
    glEnableVertexAttribArray(2); 
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 64, (void*)(16));
    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 64, (void*)(32));
    glEnableVertexAttribArray(4); 
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 64, (void*)(48));
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
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
	std::cout << "Program: [" << this->shader.program << "/4]" << std::endl;
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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.243f, 0.243f, 0.241f, 1.0f);
}
