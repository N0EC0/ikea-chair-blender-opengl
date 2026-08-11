#version 330 core
// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// Output data ; will be interpolated for each fragment.
out vec2 TexCoords;

// MVP matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Output the texture coordinates
    TexCoords = aTexCoords;
    // Output position of the vertex, in clip space : MVP * position    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}