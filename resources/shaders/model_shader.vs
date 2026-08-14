#version 330 core
// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

// Output data ; will be interpolated for each fragment.
out vec2 TexCoords;

// MVP matrix
uniform mat4 mvp;

void main()
{
    // Output the texture coordinates
    TexCoords = aTexCoords;
    // Output position of the vertex, in clip space : MVP * position    
    gl_Position = mvp * vec4(aPos, 1.0);
}