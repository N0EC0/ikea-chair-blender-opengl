#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <shader.h>

#include <string>
#include <vector>

struct Vertex {
    // position
    glm::vec3 Position;
    // texCoords
    glm::vec2 TexCoords;
};

struct ModelTexture {
    unsigned int id;
    std::string path;
};

class Mesh {
    public:
        // mesh Data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<ModelTexture> textures;
        unsigned int VAO;

        // constructor
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<ModelTexture> textures);

        // render the mesh
        void Draw();

    private:
        // render data 
        unsigned int VBO, EBO;

        // initializes all the buffer objects/arrays
        void setupMesh();
};

#endif
