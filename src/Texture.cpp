/*
    COMP371 2261 CX - Assignment 2 
    Team members:
    - Nerina An 40310293 
    - Noemie Corneillier 40284815 
    - Ryan Anthony Khireddine 40315218

	This part was made with the help of the following tutorial:
	https://learnopengl.com/Getting-started/Textures
    https://github.com/nothings/stb/blob/master/stb_image.h
*/

#include "Texture.h"
#include <GL/glew.h>
#include <stb_image.h>
#include <iostream>

Texture::Texture(const char* path) {
    glGenTextures(1, &id);
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, id);

	// Set texture filtering parameters
	// Use linear filtering for minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // for smoother pixels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width;
    int height;
    int channels;
    
    // load image
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return;
    }

    GLenum format;
    // Check for proper channel ex: PNG has alpha so need RGBA
    if (channels == 1) {
        format = GL_RED;
    }
    else if (channels == 3) {
        format = GL_RGB;
    }
    else if (channels == 4) {
        format = GL_RGBA;
    }
    else {
        std::cerr << "Unsupported channel count for texture: " << path << std::endl;
        stbi_image_free(data);
        return;
    }

    // Create texture and generate mipmap
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // Free the image memory after creating the texture
    stbi_image_free(data);
    // Flag for debugging
    valid = true;
}

// Destructor
Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::bind(unsigned int textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, id);
}

bool Texture::isValid() const {
    return valid;
}