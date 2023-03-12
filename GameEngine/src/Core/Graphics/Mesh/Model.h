#pragma once

#include <vulkan/vulkan.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Mesh.h"

class Model
{
public:

	Model(class Device* device, const std::string& filePath);

	void Draw(VkCommandBuffer commandBuffer);

private:

	std::vector<std::shared_ptr<class Mesh>> m_Meshes;
	std::vector<std::shared_ptr<class Texture2D>> m_Textures;
	std::vector<std::shared_ptr<class Texture2D>> m_TexturesLoaded;

	void ProcessNode(aiNode* node, const aiScene* scene);
	std::shared_ptr<class Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<class Texture2D*> LoadMaterialTextures( aiMaterial* mat, aiTextureType type, std::string typeName);

	class Device* m_Device = nullptr;
};