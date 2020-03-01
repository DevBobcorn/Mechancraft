#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D cursor;
uniform float cursorHorz;
uniform float cursorVert;

void main()
{
	if (TexCoords[0] >= 0.5f - cursorHorz && TexCoords[0] <= 0.5f + cursorHorz && TexCoords[1] >= 0.5f - cursorVert && TexCoords[1] <= 0.5f + cursorVert) {
		float xcrd = (TexCoords[0] + cursorHorz - 0.5f) / (2.0 * cursorHorz);
		float ycrd = (TexCoords[1] + cursorVert - 0.5f) / (2.0 * cursorVert);
		vec2 CurCoords = vec2(xcrd,ycrd);
		if (texture(cursor, CurCoords).rgb == vec3(0.0,0.0,0.0)) {
			FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
		} else {
			vec3 col = texture(screenTexture, TexCoords).rgb;
			FragColor = vec4(col, 1.0);
		}
	} else {
		vec3 col = texture(screenTexture, TexCoords).rgb;
		FragColor = vec4(col, 1.0);
	}
} 