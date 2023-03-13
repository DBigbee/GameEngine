#pragma once

#include <vulkan/vulkan.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Mesh.h"

namespace GE
{
	class Model
	{
	public:
		Model();

		Model(const std::vector<Ref<class Mesh>>& meshes);

		Model(const std::string& filePath);

		const std::vector<Ref<class Mesh>>& GetMeshes() const { return m_Meshes; }

		void AddMesh(const Ref<class Mesh>& mesh);

	private:

		std::vector<Ref<class Mesh>> m_Meshes;
		std::vector<Ref<class Texture2D>> m_Textures;
		std::vector<Ref<class Texture2D>> m_TexturesLoaded;

		void ProcessNode(aiNode* node, const aiScene* scene);
		Ref<class Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<class Texture2D>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}
