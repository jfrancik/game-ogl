// FRAGMENT SHADER
#version 400

in vec4 color;
in vec4 position;
in vec2 texCoord0;

out vec4 outColor;

// Texture
uniform sampler2D texture0;

// Color Key
uniform vec3 colorKey = vec3(-1, -1, -1);
uniform float colorKeyMul = 1;

// Solid color
uniform vec3 colorSolid;

// Subroutines
subroutine vec4 RenderType();
subroutine uniform RenderType currentRenderMethod;

subroutine(RenderType) vec4 renderTexture() 
{
    return texture(texture0, texCoord0);
}

subroutine(RenderType) vec4 renderTextureColorKey() 
{
    vec4 color = texture(texture0, texCoord0);
	// color key calculation
	vec3 dist = color.rgb - colorKey;
	return vec4(color.rgb, colorKeyMul * dot(dist, dist));
}

subroutine(RenderType) vec4 renderColorSolid() 
{
    return vec4(colorSolid, 1.0);
}


void main(void) 
{
	outColor = currentRenderMethod();
}
