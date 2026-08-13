
#include <GL/glew.h> // holds all OpenGL type declarations

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

Mesh::Mesh(Mesh&& other) noexcept
    : VAO(other.VAO),
      VBO(other.VBO),
      EBO(other.EBO),
      textureID(other.textureID),
      indexCount(other.indexCount)
{
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
    other.textureID = 0;
    other.indexCount = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
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
    const std::vector<unsigned int>& indices
) {
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

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

void Mesh::release()
{
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