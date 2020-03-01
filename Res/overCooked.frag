#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

#define r 100

float rand(float x, float y){
	return fract(cos(x * (12.9898) + y * (4.1414)) * 43758.5453);
}

float noise(float x, float y)
{
	int base_x = int(floor(x / r));
	int base_y = int(floor(y / r));
	x = fract(x / r);
	y = fract(y / r);
	float f_x = smoothstep(0, 1, x);
	float f_y = smoothstep(0, 1, y);
	return mix(mix(rand(float(base_x), float(base_y)), rand(float(base_x + 1), float(base_y)), f_x), mix(rand(float(base_x), float(base_y + 1)), rand(float(base_x + 1), float(base_y + 1)), f_x) ,f_y);
}

void main()
{
	float grey = noise(TexCoord.x * 500,TexCoord.y * 500) * 0.3 + 0.7;
	vec4 colorVec = vec4(TexCoord.y * grey,TexCoord.x * grey,TexCoord.x * grey,1.0);
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2) * colorVec;
}