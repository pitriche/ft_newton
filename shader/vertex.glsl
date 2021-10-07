#version 410 core

/* uniforms */
uniform mat4	matrix_view;

/* INPUT | from VBO */
layout (location = 0) in vec3	position_vertex;
layout (location = 1) in mat4	model_matrix_inst;

/* OUTPUT | to geometry */
out vec3		sphere_center;
out float		sphere_radius;
out vec2		world_pos_vertex;

void	main(void)
{
	gl_Position = matrix_view * model_matrix_inst * vec4(position_vertex, 1);
	sphere_center = (matrix_view * model_matrix_inst * vec4(0, 0, 0, 1)).xyz;
	sphere_radius = sqrt(dot(gl_Position.xyz - sphere_center,
			gl_Position.xyz - sphere_center));
	world_pos_vertex = (model_matrix_inst * vec4(position_vertex, 1)).xz;
}