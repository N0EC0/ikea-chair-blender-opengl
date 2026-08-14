/*
    COMP371 2261 CX - Assignment 3
    Team members:
    - Nerina An 40310293
    - Noemie Corneillier 40284815
    - Ryan Anthony Khireddine 40315218

    This part was made with the help of this tutorial:
    https://learnopengl.com/Model-Loading/Mesh
*/

#include <GL/glew.h>

#include <mesh.h>

#include <cstddef>
#include <utility>

// constructor
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int texture)
    : textureID(texture), indexCount(static_cast<unsigned int>(indices.size())) {
    setupMesh(vertices, indices);
}

// Destructor
Mesh::~Mesh() {
    release();
}

// Move constructor
Mesh::Mesh(Mesh&& other) 
    noexcept: VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), 
    textureID(other.textureID), indexCount(other.indexCount) {
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
    other.textureID = 0;
    other.indexCount = 0;
}

//  Move assignment operator
Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        release();

        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        textureID = other.textureID;
        indexCount = other.indexCount;

        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
        other.textureID = 0;
        other.indexCount = 0;
    }
    return *this;
}

//  render the mesh
void Mesh::Draw() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

// initializes all the buffer objects/arrays
void Mesh::setupMesh(
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices) {
    
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Structs memory layout is sequential for all its items
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	// load data into element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glBindVertexArray(0);
}

//  properly de-allocates all resources once they are no longer needed
void Mesh::release() {
    if (EBO != 0)
        glDeleteBuffers(1, &EBO);
    if (VBO != 0)
        glDeleteBuffers(1, &VBO);
    if (VAO != 0)
        glDeleteVertexArrays(1, &VAO);

    EBO = 0;
    VBO = 0;
    VAO = 0;
}