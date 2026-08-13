#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>

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
        // constructor
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<ModelTexture> textures);

        // render the mesh
        void Draw();

    private:
        // mesh Data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<ModelTexture> textures;
        
        unsigned int VAO;
        
        // render data 
        unsigned int VBO, EBO;

        // initializes all the buffer objects/arrays
        void setupMesh();
};

#endif
