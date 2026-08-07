#version 330 core
// Fragment shader
out vec4 FragColor;

// texture coordinates passed from the vertex shader
in vec2 TexCoord;

// value to mix the two textures
uniform float mixValue;

// sampler2D is a type that represents a texture. It is used to sample colors from a texture image.
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	// The mix function takes three parameters: the first two are the values to be mixed, and the third is the mixing factor.
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);
}

// If the third value is 0.0 it returns the first input; 
// if it's 1.0 it returns the second input value. 
// A value of 0.2 will return 80% of the first input color and 20% of the second input color, 
// resulting in a mixture of both textures. 
