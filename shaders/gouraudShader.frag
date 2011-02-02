// Goraud shading with color map and glow map

varying vec4 vertexColor;

uniform sampler2D colorMap;
uniform sampler2D glowMap;
uniform int hasTexture [3];

void main() {
	
	vec4 diffuse_base;
	if (hasTexture[0] == 1)
		diffuse_base = texture2D(colorMap, gl_TexCoord[0].st);
	else
		diffuse_base = gl_FrontMaterial.diffuse;
		
	vec4 ambient_base;
	if (hasTexture[2] == 1)
		ambient_base = texture2D(glowMap, gl_TexCoord[0].st);
	else
		ambient_base = gl_FrontMaterial.ambient;
	
	gl_FragColor = vertexColor*diffuse_base;
	gl_FragColor.x += (1.0-gl_FragColor.x)*ambient_base.x;
	gl_FragColor.y += (1.0-gl_FragColor.y)*ambient_base.y;
	gl_FragColor.z += (1.0-gl_FragColor.z)*ambient_base.z;
}
