#version 410 core

/* INPUT | from geometry */
in vec3		color_geometry;
in vec2		pos_geometry;		/* for checkerboard pattern */


/* OUTPUT | to raster */
out vec4	outColor;

void	main()
{
	vec3	tmp_color;

	tmp_color = color_geometry;
	if ((int(floor(pos_geometry.x) + floor(pos_geometry.y)) % 2) == 0)
		tmp_color *= 0.8;
	outColor = vec4(tmp_color, 1.0);
}
