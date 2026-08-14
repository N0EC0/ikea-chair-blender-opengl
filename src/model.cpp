/*
    COMP371 2261 CX - Assignment 3
    Team members:
    - Nerina An 40310293
    - Noemie Corneillier 40284815
    - Ryan Anthony Khireddine 40315218

    This part was made with the help of this tutorial:
    https://learnopengl.com/Model-Loading/Model
*/

#include <GL/glew.h> 
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <model.h>

#include <iostream>

// Function to load a texture from file and return its OpenGL texture ID
unsigned int TextureFromFile(const char* path, const std::string& directory);

// constructor, expects a filepath to a 3D model.
Model::Model(std::string const& path) {
    loadModel(path);
}

// Desctructor
Model::~Model() {
    for (const ModelTexture& texture : textures_loaded) {
        if (texture.id != 0)
            glDeleteTextures(1, &texture.id);
    }
}

// draws the model, and thus all its meshes
void Model::Draw() const {
    for (const Mesh& mesh : meshes)
        mesh.Draw();
}

// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector
void Model::loadModel(const std::string& path) {
    // read file via ASSIMP
    Assimp::Importer importer;
	// flags for post-processing
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) { // if is Not Zero
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

// Processes each individual mesh located at the node and repeats this process on its children nodes (if any)
void Model::processNode(aiNode* node, const aiScene* scene) {
	// process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) { 
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // after processing all of the meshes (if any), recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

// Processes a mesh and returns a Mesh object
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // texture coordinates
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // Going through each of the mesh's faces and retrieve the corresponding vertex indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    unsigned int textureID = 0;
	// check if the mesh has a material assigned to it
    if (mesh->mMaterialIndex < scene->mNumMaterials) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// load diffuse texture
        textureID = loadDiffuseTexture(material);
    }
	// return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textureID);
}

// Loads the diffuse texture from the material and returns its OpenGL texture ID
unsigned int Model::loadDiffuseTexture(aiMaterial* material) {
    if (material->GetTextureCount(aiTextureType_DIFFUSE) == 0)
        return 0;
	// Get the texture path
    aiString path;
	// Check if the texture is successfully retrieved
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) != AI_SUCCESS) {
        return 0;
    }

    // Reuse the texture if it was already loaded by this model.
    for (const ModelTexture& loadedTexture : textures_loaded) {
        if (loadedTexture.path == path.C_Str())
            return loadedTexture.id;
    }
	// If the texture hasn't been loaded yet, load it from file
    const unsigned int textureID = TextureFromFile(path.C_Str(), directory);
	// Store the loaded texture for future reference
    if (textureID != 0) {
        textures_loaded.push_back({
            textureID,
            path.C_Str()
        });
    }
    return textureID;
}

// Loads a texture from file and returns its OpenGL texture ID
unsigned int TextureFromFile(const char* path, const std::string& directory) {
	// Construct the full path to the texture file
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    
    int width = 0;
    int height = 0;
    int componentCount = 0;
	// Load the texture data using stb_image
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &componentCount, 0);
    if (!data) {
        std::cerr << "Texture failed to load: " << filename << " — " << stbi_failure_reason() << '\n';
        return 0;
    }

    GLenum format;
	// Determine the texture format based on the number of components
    switch (componentCount) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            std::cerr << "Unsupported texture format: " << filename << " (" << componentCount << " components)\n";
            stbi_image_free(data);
            return 0;
    }
	// Generate and bind the OpenGL texture
    unsigned int textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Necessary for RGB images whose row size is not divisible by four.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Upload the texture data to OpenGL and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
	// Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Reset the unpack alignment to the default value
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	// Free the loaded image data
    stbi_image_free(data);
    return textureID;
}

