// FRAGMENT SHADER
#version 330

in vec4 color;
in vec4 position;
in vec2 texCoord0;

out vec4 outColor;

// Texture
uniform sampler2D texture0;

// Color Key
uniform vec3 colorKey = vec3(-1, -1, -1);
uniform float colorKeyMul = 1;

void main(void) 
{
	outColor = texture(texture0, texCoord0);

	// color key calculation
	vec3 dist = outColor.rgb - colorKey;
	outColor.a = colorKeyMul * dot(dist, dist);
}
