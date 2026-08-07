#version 330 core
// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

// Output data ; will be interpolated for each fragment.
out vec2 TexCoord;

// Values that stay constant for the whole mesh.
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	// Output position of the vertex, in clip space : MVP * position
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	// Output the texture coordinates
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}