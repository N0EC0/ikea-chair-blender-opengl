#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>

#include <vector>

struct Vertex {
    // position
    glm::vec3 Position;
    // texCoords
    glm::vec2 TexCoords;
};

class Mesh {
    public:
        // constructor
        Mesh(
            const std::vector<Vertex>& vertices,
            const std::vector<unsigned int>& indices,
            unsigned int texture
        );

        ~Mesh();

        // OpenGL handles must not be copied
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        // Required because Model stores meshes in a vector
        Mesh(Mesh&& other) noexcept;
        Mesh& operator=(Mesh&& other) noexcept;

        // render the mesh
        void Draw() const;

    private:      
        unsigned int VAO = 0;
        unsigned int VBO = 0;
        unsigned int EBO = 0;
        
        unsigned textureID = 0;
        unsigned int indexCount = 0;

        void setupMesh(
            const std::vector<Vertex>& vertices,
            const std::vector<unsigned int>& indices
        );

        void release();
};

#endif
