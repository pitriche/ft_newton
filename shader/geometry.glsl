#version 410 core

/* uniforms */
uniform int		color;
uniform int		object_type;	/* 0:Cube, 1:Sphere, 2:Floor */
uniform	float	fov_mult;

/* INPUT | from vertex */
layout(triangles) in;
in vec3		sphere_center[];
in float	sphere_radius[];

/* OUTPUT | to fragment */
layout(triangle_strip, max_vertices = 48) out;
out vec3	color_geometry;

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

vec4	clip_to_camera(vec3 clip_pos)
{
	clip_pos.x /= (16.0 / 9.0);	/* screen ratio */
	clip_pos *= 10;				/* to fix annoying near clip */
	return(vec4(clip_pos.xy * fov_mult, -1, clip_pos.z));
}

/* ########################################################################## */

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
	else
	{
		normal_vec[0] = vec3(0, 0, -1);
		normal_vec[1] = vec3(0, 0, -1);
		normal_vec[2] = vec3(0, 0, -1);
	}
	color_geometry = color_from_normal(normal_vec[0], color_rgb);
	gl_Position = clip_to_camera(p1);
	EmitVertex();
	color_geometry = color_from_normal(normal_vec[1], color_rgb);
	gl_Position = clip_to_camera(p2);
	EmitVertex();
	color_geometry = color_from_normal(normal_vec[2], color_rgb);
	gl_Position = clip_to_camera(p3);
	EmitVertex();
	EndPrimitive();
}

vec3[6]	icosphere_face_divide(vec3 p1, vec3 p2, vec3 p3)
{
	vec3	vertex[6];
	float	scale_radius;

	vertex[0] = p1;
	vertex[3] = p2;
	vertex[5] = p3;

	vertex[1] = p1 + ((p2 - p1) / 2);
	vertex[2] = p1 + ((p3 - p1) / 2);
	vertex[4] = p2 + ((p3 - p2) / 2);

	/* expand middle point towards radius (each have different ratio) */
	scale_radius = sphere_radius[0] / sqrt(dot((vertex[1] - sphere_center[0]),
			(vertex[1] - sphere_center[0])));
	vertex[1] += (vertex[1] - sphere_center[0]) * (scale_radius - 1);

	scale_radius = sphere_radius[0] / sqrt(dot((vertex[2] - sphere_center[0]),
			(vertex[2] - sphere_center[0])));
	vertex[2] += (vertex[2] - sphere_center[0]) * (scale_radius - 1);

	scale_radius = sphere_radius[0] / sqrt(dot((vertex[4] - sphere_center[0]),
			(vertex[4] - sphere_center[0])));
	vertex[4] += (vertex[4] - sphere_center[0]) * (scale_radius - 1);
	return (vertex);
}

void	icosphere_face_1(vec3 p1, vec3 p2, vec3 p3, vec3 color_rgb)
{
	vec3	vertex[6];

	vertex = icosphere_face_divide(p1, p2, p3);
	draw_triangle(vertex[0], vertex[1], vertex[2], color_rgb);
	draw_triangle(vertex[1], vertex[3], vertex[4], color_rgb);
	draw_triangle(vertex[1], vertex[4], vertex[2], color_rgb);
	draw_triangle(vertex[2], vertex[4], vertex[5], color_rgb);
}

void	icosphere_face_2(vec3 p1, vec3 p2, vec3 p3, vec3 color_rgb)
{
	vec3	vertex[6];

	vertex = icosphere_face_divide(p1, p2, p3);
	icosphere_face_1(vertex[0], vertex[1], vertex[2], color_rgb);
	icosphere_face_1(vertex[1], vertex[3], vertex[4], color_rgb);
	icosphere_face_1(vertex[1], vertex[4], vertex[2], color_rgb);
	icosphere_face_1(vertex[2], vertex[4], vertex[5], color_rgb);
}

/* ########################################################################## */

void	main(void)
{
	float	mult;
	vec3	normal_vec;
	vec3	color_rgb;

	color_rgb = vec3(((color >> 16) & 0xff) / 255.0, ((color >> 8) & 0xff) /
		255.0, (color & 0xff) / 255.0);

	normal_vec = normal(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz,
		gl_in[2].gl_Position.xyz);

	if (object_type == 0 || object_type == 2)
	{
		draw_triangle(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz,
			gl_in[2].gl_Position.xyz, color_rgb);
	}
	else if (object_type == 1)
	{
		icosphere_face_2(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz,
			gl_in[2].gl_Position.xyz, color_rgb);
	}

}
