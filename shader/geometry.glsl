#version 410 core

/* uniforms */
uniform int		color;
uniform int		object_type;	/* 0:Cube, 1:Sphere, 2:Floor */
uniform	float	fov_mult;

/* INPUT | from vertex */
layout(triangles) in;
in vec3		sphere_center[];
in float	sphere_radius[];
in vec2		world_pos_vertex[];


/* OUTPUT | to fragment */
layout(triangle_strip, max_vertices = 48) out;
out vec3	color_geometry;
out vec2	pos_geometry;		/* for checkerboard pattern */

#define ICS_RADIAL	0.15

/* ########################################################################## */

vec3	normal(vec3 p1, vec3 p2, vec3 p3)
{
	vec3	v1;
	vec3	v2;

	v1 = p2 - p1;
	v2 = p3 - p1;
	return (normalize(cross(v2, v1)));
}

/* ########################################################################## */

vec4	clip_to_camera(vec3 clip_pos)
{
	clip_pos.x /= (16.0 / 9.0);	/* screen ratio */
	return(vec4(clip_pos.xy * fov_mult, -0.01, clip_pos.z));
}

vec3	color_from_normal(vec3 normal_vec, vec3 color_rgb)
{
	float	mult;

	mult = dot(normal_vec, vec3(0, 0, -1));
	return (color_rgb * ((mult + 0.2) / 1.2));
}

void	draw_triangle(vec3 p1, vec3 p2, vec3 p3, vec3 color_rgb)
{
	float	mult;
	vec3	normal_vec[3];
	vec2	world_pos[3];

	/* default case : no checkerboarding (1.1 for round error) */
	world_pos[0] = vec2(1.1, 0);
	world_pos[1] = vec2(1.1, 0);
	world_pos[2] = vec2(1.1, 0);
	if (object_type == 0)
	{
		normal_vec[0] = normal(p1, p2, p3);
		normal_vec[1] = normal_vec[0];
		normal_vec[2] = normal_vec[0];
	}
	else if (object_type == 1)
	{
		normal_vec[0] = normalize(p1 - sphere_center[0]);
		normal_vec[1] = normalize(p2 - sphere_center[0]);
		normal_vec[2] = normalize(p3 - sphere_center[0]);
	}
	else	/* ground, checkerboarding */
	{
		world_pos[0] = world_pos_vertex[0];
		world_pos[1] = world_pos_vertex[1];
		world_pos[2] = world_pos_vertex[2];
		normal_vec[0] = vec3(0, 0, -1);
		normal_vec[1] = vec3(0, 0, -1);
		normal_vec[2] = vec3(0, 0, -1);
	}
	color_geometry = color_from_normal(normal_vec[0], color_rgb);
	pos_geometry = world_pos[0];
	gl_Position = clip_to_camera(p1);
	EmitVertex();
	color_geometry = color_from_normal(normal_vec[1], color_rgb);
	pos_geometry = world_pos[1];
	gl_Position = clip_to_camera(p2);
	EmitVertex();
	color_geometry = color_from_normal(normal_vec[2], color_rgb);
	pos_geometry = world_pos[2];
	gl_Position = clip_to_camera(p3);
	EmitVertex();
	EndPrimitive();
}

/* ########################################################################## */

void	main(void)
{
	vec3	color_rgb;

	color_rgb = vec3(((color >> 16) & 0xff) / 255.0, ((color >> 8) & 0xff) /
		255.0, (color & 0xff) / 255.0);
	draw_triangle(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz,
		gl_in[2].gl_Position.xyz, color_rgb);
}
