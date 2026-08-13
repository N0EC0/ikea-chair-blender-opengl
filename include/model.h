#ifndef MODEL_H
#define MODEL_H

#include <mesh.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <assimp/scene.h>
#include <stb_image.h>

class Model {
public:
    // model data 
    // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once
    std::vector<ModelTexture> textures_loaded;	
    std::vector<Mesh>    meshes;
    std::string directory;

    // constructor, expects a filepath to a 3D model.
    Model(std::string const& path);

    // draws the model, and thus all its meshes
    void Draw();

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<ModelTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type);
};

#endif
