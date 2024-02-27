#ifndef SKELETAL_MODEL_H
#define SKELETAL_MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <map>

#include "SkeletalMesh.h"

struct BoneInfo
{
    /*id is index in finalBoneMatrices*/
    int id;

    /*offset matrix transforms vertex from model space to bone space*/
    glm::mat4 offset;

};

class SkeletalModel
{
public:
    SkeletalModel(const char* path)
    {
        loadModel(path);
    }
    void Draw(Shader& shader, Camera& camera);

    void setTranslation(glm::vec3 translation);
    void setRotation(glm::quat rotation);
    void setScale(glm::vec3 scale);

    auto& GetBoneInfoMap() { return m_BoneInfoMap; }
    int& GetBoneCount() { return m_BoneCounter; }

private:
    // model data
    std::vector<Texture> textures_loaded; // can be main textures vector but not used rn.
    std::vector<SkeletalMesh> meshes;
    std::string directory;

    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    std::map<std::string, BoneInfo> m_BoneInfoMap;
    int m_BoneCounter = 0;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene, aiMatrix4x4t<float> transformation);
    SkeletalMesh processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4t<float> transformation);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName, int slotInc);
    glm::mat4 aiMat4toGLM(aiMatrix4x4t<float> matrix);

    void ExtractBoneWeightForVertices(std::vector<SkeletalVertex>& vertices, aiMesh* mesh, const aiScene* scene);
};
#endif