#version 330 core
// Fragment shader
out vec4 FragColor;

// texture coordinates passed from the vertex shader
in vec2 TexCoords;

// sampler2D is a type that represents a texture. It is used to sample colors from a texture image.
uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
}

