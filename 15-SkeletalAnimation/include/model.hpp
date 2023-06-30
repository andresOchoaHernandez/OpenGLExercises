#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>

#include <mesh.hpp>

struct Bone
{
    glm::mat4 finalTransformation = glm::mat4(0.0f);
    glm::mat4 meshToBone = glm::mat4(0.0f);
};

class Model
{

    std::vector<Mesh> meshes;
    std::vector<Texture> texturesLoaded;
    std::string directory;

    /* ANIMATION DATA */
    unsigned int numberOfBones;
    std::vector<Bone> bones;
    std::map<std::string,unsigned int> boneMapping;
    
    glm::mat4 globalTransformation;
    glm::mat4 globalInverseTransformation;


    public:
        Model(const char *path);
        void Draw(Shader &shader);

        std::vector<Mesh> getMeshes();

    private:

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
        unsigned int TextureFromFile(const char *path, const std::string &directory);
};