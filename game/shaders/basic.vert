// VERTEX SHADER
#version 330

// Matrices
uniform mat4 matrixProjection;
uniform mat4 matrixModelView;

in vec2 aVertex;
in vec2 aTexCoord;

out vec4 color;
out vec4 position;
out vec2 texCoord0;

void main(void) 
{
	// calculate position
	position = matrixModelView * vec4(aVertex, 0.0, 1.0);
	gl_Position = matrixProjection * position;

	// calculate texture coordinate
	texCoord0 = aTexCoord;

	// calculate light
	color = vec4(1, 0, 0, 1);
}
