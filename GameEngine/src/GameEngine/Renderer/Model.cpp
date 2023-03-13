#include "pch.h"
#include "Model.h"
#include "Renderer/Texture.h"

namespace GE
{
	Model::Model()
	{

	}

	Model::Model(const std::string& filePath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			throw std::runtime_error(importer.GetErrorString());
			return;
		}

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::AddMesh(const Ref<class Mesh>& mesh)
	{
		m_Meshes.emplace_back(mesh);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));

			if (mesh->mMaterialIndex >= 0)
			{
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				std::vector<Ref<Texture2D>> diffuse = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
				m_Textures.insert(m_Textures.begin(), diffuse.begin(), diffuse.end());
			}
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}


	}

	std::shared_ptr<class Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Texture2D> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.m_Position = { mesh->mVertices[i].x,mesh->mVertices[i].y ,mesh->mVertices[i].z };

			if (mesh->mTextureCoords[0])
			{
				vertex.m_TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			}
			else
			{
				vertex.m_TexCoord = glm::vec2(0.0f, 0.0f);
			}

			if (mesh->mColors[0])
			{
				vertex.m_Color = { mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b };
			}
			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back((uint32_t)face.mIndices[j]);
			}
		}

		return MakeRef<Mesh>(vertices, indices);
	}

	std::vector<Ref<class Texture2D>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<Ref<Texture2D>> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			if (std::find_if(m_TexturesLoaded.begin(), m_TexturesLoaded.end(), [str](Ref<Texture2D> text) {
				return text->GetPath() == str.C_Str();
				}) != m_TexturesLoaded.end())
			{
				textures.push_back(m_TexturesLoaded[i]);
			}
			else
			{
				Ref<Texture2D> texture = Texture2D::Create(str.C_Str());
				m_TexturesLoaded.emplace_back(texture);
			}

		}

		return textures;
	}
}