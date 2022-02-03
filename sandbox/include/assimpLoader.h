#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>
#include <glm/glm.hpp>
#include <map>

#include "systems/log.h"
#include <filesystem>

namespace Engine {
	namespace Loader
	{
		struct TempMesh
		{
			std::vector<Renderer3DVertex> vertices;
			std::vector<uint32_t> indicies;
			std::shared_ptr<TextureRend> diffusTex = nullptr;
			glm::vec3 defuseTint = { 1.f, 1.f, 1.f };
		};
		static TempMesh output;
		//static std::vector<std::shared_ptr<Material>> s_material;
		//static std::vector<std::shared_ptr<VertexArray>> s_VAO;
		static std::string s_workingDir = "";
		/*std::shared_ptr<VertexArray> cubeVAO;
		std::shared_ptr<VertexBuffer> cubeVBO;
		std::shared_ptr<IndexBuffer> cubeIBO;*/

		//auto& resources = Application::getInstance().getResourceManager();
		static void ASSIMPProcessMesh(aiMesh* mesh, const aiScene* scene)
		{
			output.vertices.clear();
			//std::multimap<uint32_t, std::pair<uint32_t, float>> vertexBoneWeights;

			// Find vertex properties
			bool hasPositions = mesh->HasPositions();
			bool hasNormals = mesh->HasNormals();
			uint32_t numUVChannels = mesh->GetNumUVChannels();

			// Iterate through vertices
			for (uint32_t i = 0; i < mesh->mNumVertices; i++)
			{
				// Get vertex data
				glm::vec3 position, normal;
				std::vector<glm::vec2> texCoords(numUVChannels);

				if (hasPositions) position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				if (hasNormals) normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

				for (uint32_t j = 0; j < numUVChannels; j++)
				{
					texCoords[j] = glm::vec2(mesh->mTextureCoords[j][i].x, mesh->mTextureCoords[j][i].y);
				}

				Renderer3DVertex vert;
				vert.position = position;
				vert.normal = normal;
				vert.uvCoords = texCoords[0];
				output.vertices.push_back(vert);


				// Log part - assume postion, normal and UV coords
				//Log::info("VERTEX DATA");
				//if(texCoords.size()>0)Log::info("P x:{0}, y:{1}, z:{2}, N x:{3}, y:{4}, z:{5}, T u:{6}, v{7}", position.x, position.y, position.z, normal.x, normal.y, normal.z, texCoords[0].x, texCoords[0].y);
				//Log::info("P x:{0}, y:{1}, z:{2}, N x:{3}, y:{4}, z:{5}, NO TEXTURE", position.x, position.y, position.z, normal.x, normal.y, normal.z );
			}


			Log::info("INDICES");
			//uint32_t elementCount = 0;
			for (uint32_t i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				// retrieve all indices of the face and store them in the indices vector
				for (uint32_t j = 0; j < face.mNumIndices; j++)
				{
					//uint32_t index = face.mIndices[j]; elementCount++;
					output.indicies.push_back(face.mIndices[j]);
				}

				// Log part - assume all faces are trinalge and therefore ahve 3 indices
				//Log::info("Face {0}: {1} {2} {3}", i, face.mIndices[0], face.mIndices[1], face.mIndices[2]);
			}
			//Log::info("Num vertices {0} Num indices {1}", mesh->mNumVertices, elementCount);



			std::vector<aiTextureType> types = {
				aiTextureType_NONE,
				aiTextureType_DIFFUSE,
				aiTextureType_SPECULAR,
				aiTextureType_AMBIENT,
				aiTextureType_EMISSIVE,
				aiTextureType_HEIGHT,
				aiTextureType_NORMALS,
				aiTextureType_SHININESS,
				aiTextureType_OPACITY,
				aiTextureType_DISPLACEMENT,
				aiTextureType_LIGHTMAP,
				aiTextureType_REFLECTION,
				aiTextureType_BASE_COLOR,
				aiTextureType_NORMAL_CAMERA,
				aiTextureType_EMISSION_COLOR,
				aiTextureType_METALNESS,
				aiTextureType_DIFFUSE_ROUGHNESS,
				aiTextureType_AMBIENT_OCCLUSION,
				aiTextureType_UNKNOWN
			};


			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];


			for (auto type : types)
			{
				for (uint32_t i = 0; i < material->GetTextureCount(type); i++)
				{
					aiString str;
					material->GetTexture(type, i, &str);
					if (type == aiTextureType_DIFFUSE)
					{
						std::string pwd = std::filesystem::current_path().u8string();
						std::string fn(str.C_Str());

						auto extenLoc = fn.find(".DDS");
						if (extenLoc != std::string::npos)
						{
							fn.replace(extenLoc, extenLoc + 4, ".bmp");
						}
						//std::string texturepath(pwd + "\\assets\\models\\" + fn);
						std::string texturepath(pwd + s_workingDir + fn);
						Log::info("Texture type:{0} filepath:{1}", type, texturepath.c_str());
						output.diffusTex.reset(TextureRend::create(texturepath.c_str()));

					}

				}

			}


			aiString stringValue;
			int intValue;
			float floatValue;
			aiColor3D colorValue;

			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, colorValue))
			{
				Log::info("Material name: {0}", stringValue.C_Str());
				output.defuseTint = { (float)colorValue.r,(float)colorValue.g, (float)colorValue.b };
			}

			


			/*std::shared_ptr<Material> mat;
			if (output.diffusTex)
				mat.reset(new Material(shader, output.diffusTex));
			else
				mat.reset(new Material(shader, glm::vec4(output.defuseTint,1.0f)));

			s_VAO.push_back(cubeVAO);
			s_material.push_back(mat);*/
		}

		static void ASSIMPProcessNode(aiNode *node, const aiScene *scene)
		{
			std::string parentName = "Null";
			if (node->mParent != nullptr) parentName = node->mParent->mName.C_Str();
			if (node->mNumMeshes > 0) Log::info("MESHED NODE: {0} PARENT: {1}", node->mName.C_Str(), parentName);
			if (node->mNumMeshes == 0) Log::info("UNMESHED NODE: {0} PARENT: {1}", node->mName.C_Str(), parentName);

			aiMatrix4x4 *transform = &node->mTransformation;

			Log::info("TRANSFORM");
			Log::info("{0} {1} {2} {3}", transform->a1, transform->a2, transform->a3, transform->a4);
			Log::info("{0} {1} {2} {3}", transform->b1, transform->b2, transform->b3, transform->b4);
			Log::info("{0} {1} {2} {3}", transform->c1, transform->c2, transform->c3, transform->c4);
			Log::info("{0} {1} {2} {3}", transform->d1, transform->d2, transform->d3, transform->d4);

			// process all the node's meshes
			for (uint32_t i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
				ASSIMPProcessMesh(mesh, scene);
			}

			//  Process child nodes
			for (uint32_t i = 0; i < node->mNumChildren; i++)
			{
				ASSIMPProcessNode(node->mChildren[i], scene);
			}
		}

		static void ASSIMPLoad(const std::string& filepath, uint32_t flags)
		{
			auto lastSlashLocation = filepath.find_last_of("\\/");
			//find slash
			if (lastSlashLocation != std::string::npos)
			{
				s_workingDir = filepath.substr(0,lastSlashLocation + 1);
			}
			Assimp::Importer importer;
			const aiScene *scene = importer.ReadFile(filepath, flags);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				Log::error("Cannot load: {0}, ASSIMP Error {1}", filepath, importer.GetErrorString());
				return;
			}
			Log::info("Loaded: {0} ", filepath);
			ASSIMPProcessNode(scene->mRootNode, scene);

		}
	}
}