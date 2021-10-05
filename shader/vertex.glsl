#version 410 core

/* uniforms */
uniform mat4	matrix_model;
uniform mat4	matrix_view;
uniform	float	fov_mult;
uniform int		object_type;	/* 0:Cube, 1:Sphere, 2:Floor */

/* INPUT | from VBO */
layout (location = 0) in vec3	position_vertex;

/* OUTPUT | to geometry */
out vec3		sphere_center;
out float		sphere_radius;

void	main(void)
{
	gl_Position = matrix_view * matrix_model * vec4(position_vertex, 1);
	if (object_type == 1)
	{
		sphere_center = (matrix_view * matrix_model * vec4(0, 0, 0, 1)).xyz;
		sphere_radius = sqrt(dot(gl_Position.xyz - sphere_center,
				gl_Position.xyz - sphere_center));
	}
}