#ifndef MODEL_H
#define MODEL_H

#include <mesh.h>
#include <assimp/scene.h>
#include <stb_image.h>

#include <string>
#include <vector>

struct ModelTexture {
    unsigned int id;
    std::string path;
};

class Model {
public:
    Model(const std::string& path);
    ~Model();

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    void Draw() const;

private:
    std::vector<ModelTexture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    
    unsigned int loadDiffuseTexture(aiMaterial* material);
};

#endif
