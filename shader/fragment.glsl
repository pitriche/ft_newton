#version 410 core

/* INPUT | from geometry */
in vec3		color_geometry;

/* OUTPUT | to raster */
out vec4	outColor;

void	main()
{
	outColor = vec4(color_geometry, 1.0);
}
