#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float totDestroyStatus;
uniform float destroyStatus;
uniform sampler2D destroyTex;

uniform bool RedstoneRetint;
uniform int PowerLevel; //(0-15)

uniform bool selected;
uniform float wireWidth;
uniform int wireMode;

uniform int masked = 0;
uniform int layer = 0;
uniform int mask[256];

void main()
{
	vec4 texColor;
	if (masked > 0) {
		int mx = int(floor(TexCoord[0] * 16.0));
		int mz = int(floor(TexCoord[1] * 16.0));
		if (mx >= 16) mx = 15;
		if (mz >= 16) mz = 15;
		int curVoxel = 0;
		switch (masked) {
			case 1: //top
				curVoxel = mask[layer * 16 + mx] >> mz & 1;
				break;
			case 2: //bot
				curVoxel = mask[(15 - layer) * 16 + mx] >> (15 - mz) & 1;
				break;
			case 3: //neg z
				curVoxel = mask[(15 - mz) * 16 + mx] >> layer & 1;
				break;
			case 4: //pos z
				curVoxel = mask[mz * 16 + mx] >> (15 - layer) & 1;
				break;
			case 5: //pos x
				curVoxel = mask[(15 - mx) * 16 + layer] >> mz & 1;
				break;
			case 6: //neg x
				curVoxel = mask[mx * 16 + (15 - layer)] >> mz & 1;
				break;
		}
		switch (curVoxel) {
			case 0:
				//texColor = vec4(float(mx * 16 + mz) / 256.0,float(mx * 16 + mz) / 256.0,float(mx * 16 + mz) / 256.0,1.0);
				discard;
			case 1:
				switch (masked) {
					case 2:
					case 3:
						texColor = texture(texture1, vec2(1.0 - TexCoord[0],1.0 - TexCoord[1]));
						break;
					case 5:
						texColor = texture(texture1, vec2(TexCoord[1],1.0 - TexCoord[0]));
						break;
					case 6:
						texColor = texture(texture1, vec2(1.0 - TexCoord[1],TexCoord[0]));
						break;
					default:
						texColor = texture(texture1, TexCoord);
						break;
				}
				break;
		}
	} else texColor = texture(texture1, TexCoord);
    if (selected && (TexCoord[0] <= wireWidth || TexCoord[0] >= 1.0 - wireWidth || TexCoord[1] <= wireWidth || TexCoord[1] >= 1.0 - wireWidth)){
		if (wireMode == 0) FragColor = vec4(0.0,0.0,0.0,1.0);
		else FragColor = vec4(1.0,1.0,1.0,1.0);
	}
	else {
		//Redstone re-tint
		if (RedstoneRetint && PowerLevel >= 0 && PowerLevel <= 15) {
			texColor = vec4(texColor.r * (PowerLevel + 1.0) / 16.0,0.0,0.0,texColor.a);
		}
		if (texColor.a < 0.9) discard;
		if (selected) {
			if (texture(destroyTex, TexCoord).a >= 0.8)
				FragColor = mix(texColor,texture(destroyTex, TexCoord),0.85);
			else {
				FragColor = texColor;
			}
		} else FragColor = texColor;
	}
}
