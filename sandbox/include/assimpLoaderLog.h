#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <map>

#include "systems/log.h"

namespace Engine {
	namespace Loader
	{
		static void ASSIMPProcessMeshLog(aiMesh *mesh, const aiScene *scene)
		{
			std::multimap<uint32_t, std::pair<uint32_t, float>> vertexBoneWeights;
			//Log::info("BONES");
			// Iterate through bones
			/*for (uint32_t i = 0; i < mesh->mNumBones; i++)
			{
				aiBone* bone = mesh->mBones[i];

				aiMatrix4x4 transform = bone->mOffsetMatrix;
				Log::info("RELATIVE TRANSFORM FOR BONE: {0} Name: {1}", i, bone->mName.C_Str());
				Log::info("{0} {1} {2} {3}", transform.a1, transform.a2, transform.a3, transform.a4);
				Log::info("{0} {1} {2} {3}", transform.b1, transform.b2, transform.b3, transform.b4);
				Log::info("{0} {1} {2} {3}", transform.c1, transform.c2, transform.c3, transform.c4);
				Log::info("{0} {1} {2} {3}", transform.d1, transform.d2, transform.d3, transform.d4);

				Log::info("WEIGHTS");
				for (uint32_t j = 0; j < bone->mNumWeights; j++)
				{
					Log::info("Bone idx: {0} VertexID: {1} Weight: {2}", i, bone->mWeights[j].mVertexId, bone->mWeights[j].mWeight);
					vertexBoneWeights.insert(std::pair<uint32_t, std::pair<uint32_t, float>>
						(bone->mWeights[j].mVertexId, std::pair<uint32_t, float>(i, bone->mWeights[j].mWeight)));
				}
			}*/

			// Find vertex properties
			bool hasPositions = mesh->HasPositions();
			bool hasNormals = mesh->HasNormals();
			bool hasTangents = mesh->HasTangentsAndBitangents();
			uint32_t numColourChannels = mesh->GetNumColorChannels();
			uint32_t numUVChannels = mesh->GetNumUVChannels();

			Log::info("VERTICES");
			// Iterate through vertices
			for (uint32_t i = 0; i < mesh->mNumVertices; i++)
			{
				// Get vertex data
				glm::vec3 position, normal, tangent, biTangent;
				std::vector<glm::vec4> colours(numColourChannels);
				std::vector<glm::vec2> texCoords(numUVChannels);

				if (hasPositions) position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				if (hasNormals) normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				if (hasTangents)
				{
					tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
					biTangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
				}

				for (uint32_t j = 0; j < numColourChannels; j++)
				{
					colours[j].r = mesh->mColors[j][i].r;
					colours[j].g = mesh->mColors[j][i].g;
					colours[j].b = mesh->mColors[j][i].b;
					colours[j].a = mesh->mColors[j][i].a;
				}

				for (uint32_t j = 0; j < numUVChannels; j++)
				{
					texCoords[j] = glm::vec2(mesh->mTextureCoords[j][i].x, mesh->mTextureCoords[j][i].y);
				}
				/*
				uint32_t boneIds[4] = { 0, 0, 0, 0 };
				float boneWeights[4] = { 0.f, 0.f, 0.f, 0.f };

				auto boneData = vertexBoneWeights.equal_range(i); // i is the vertexID
				int j = 0;
				for (auto it = boneData.first; it != boneData.second; ++it)
				{
					if (j > 4) Log::error("More than four bones influence this vertex");
					auto pair = it->second;
					boneIds[j] = pair.first;
					boneWeights[j] = pair.second;
					j++;
				}
				
				// Log part - assume postion, normal and UV coords
				Log::info("VERTEX DATA");
				if(texCoords.size()>0)Log::info("P x:{0}, y:{1}, z:{2}, N x:{3}, y:{4}, z:{5}, T u:{6}, v{7}", position.x, position.y, position.z, normal.x, normal.y, normal.z, texCoords[0].x, texCoords[0].y);
				Log::info("P x:{0}, y:{1}, z:{2}, N x:{3}, y:{4}, z:{5}, NO TEXTURE", position.x, position.y, position.z, normal.x, normal.y, normal.z );
				Log::info("Bone IDs: {0}, {1}, {2}, {3} Weights: {4}, {5}, {6}, {7}", boneIds[0], boneIds[1], boneIds[2], boneIds[3], boneWeights[0], boneWeights[1], boneWeights[2], boneWeights[3]);
			*/
			}

			Log::info("INDICES");
			uint32_t elementCount = 0;
			for (uint32_t i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				// retrieve all indices of the face and store them in the indices vector
				for (uint32_t j = 0; j < face.mNumIndices; j++)
				{
					uint32_t index = face.mIndices[j]; elementCount++;
				}

				// Log part - assume all faces are trinalge and therefore ahve 3 indices
				//Log::info("Face {0}: {1} {2} {3}", i, face.mIndices[0], face.mIndices[1], face.mIndices[2]);
			}
			Log::info("Num vertices {0} Num indices {1}",mesh->mNumVertices, elementCount);



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
					Log::info("Texture type:{0} filepath:{1}", type, str.C_Str());
				}

			}


			aiString stringValue;
			int intValue;
			float floatValue;
			aiColor3D colorValue;
			if (AI_SUCCESS == material->Get(AI_MATKEY_NAME, stringValue)) Log::info("Material name: {0}", stringValue.C_Str());
			/*
			if (AI_SUCCESS == material->Get(AI_MATKEY_SHADING_MODEL, intValue))
			{
				if (intValue == aiShadingMode_Flat) Log::info("Material shading model: Flat shading");
				if (intValue == aiShadingMode_Gouraud) Log::info("Material shading model: Gouraud");
				if (intValue == aiShadingMode_Phong) Log::info("Material shading model: Phong");
				if (intValue == aiShadingMode_Blinn) Log::info("Material shading model: Blinn");
				if (intValue == aiShadingMode_Toon) Log::info("Material shading model: Toon");
				if (intValue == aiShadingMode_OrenNayar) Log::info("Material shading model: Oren Nayar");
				if (intValue == aiShadingMode_Minnaert) Log::info("Material shading model: Minnaert");
				if (intValue == aiShadingMode_CookTorrance) Log::info("Material shading model: Cook Torrance");
				if (intValue == aiShadingMode_Fresnel) Log::info("Material shading model: Fresnel");
				if (intValue == aiShadingMode_NoShading) Log::info("Material shading model: No shading");
			}*/
			/*
			if (AI_SUCCESS == material->Get(AI_MATKEY_ENABLE_WIREFRAME, intValue))
			{
				if (intValue == 0) Log::info("Material wireframe: false");
				else if (intValue == 1) Log::info("Material wireframe: true");
				else Log::info("Material wireframe: unknown");
			}*/

			/*
			if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, floatValue)) Log::info("Material shininess: {0}", floatValue);
			if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS_STRENGTH, floatValue)) Log::info("Material shininess strength: {0}", floatValue);
			if (AI_SUCCESS == material->Get(AI_MATKEY_REFLECTIVITY, floatValue)) Log::info("Material reflectivity: {0}", floatValue);
			if (AI_SUCCESS == material->Get(AI_MATKEY_REFRACTI, floatValue)) Log::info("Material refraction index: {0}", floatValue);
			if (AI_SUCCESS == material->Get(AI_MATKEY_TRANSPARENCYFACTOR, floatValue)) Log::info("Material transprancy factor: {0}", floatValue);
			if (AI_SUCCESS == material->Get(AI_MATKEY_OPACITY, floatValue)) Log::info("Material opacity: {0}", floatValue);

			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, colorValue)) Log::info("Material ambient colour: {0}, {1}, {2}", colorValue.r, colorValue.g, colorValue.b);
			*/
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, colorValue)) Log::info("Material diffuse colour: {0}, {1}, {2}", colorValue.r, colorValue.g, colorValue.b);
			/*if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, colorValue)) Log::info("Material specular colour: {0}, {1}, {2}", colorValue.r, colorValue.g, colorValue.b);
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_EMISSIVE, colorValue)) Log::info("Material emissive colour: {0}, {1}, {2}", colorValue.r, colorValue.g, colorValue.b);
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_REFLECTIVE, colorValue)) Log::info("Material reflective colour: {0}, {1}, {2}", colorValue.r, colorValue.g, colorValue.b);
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_TRANSPARENT, colorValue)) Log::info("Material tranparent colour: {0}, {1}, {2}", colorValue.r, colorValue.g, colorValue.b);
			*/
			/* Omitted material keys:
							AI_MATKEY_BLEND_FUNC
							AI_MATKEY_OPACITY
							AI_MATKEY_BUMPSCALING
							AI_MATKEY_GLOBAL_BACKGROUND_IMAGE
							AI_MATKEY_GLOBAL_SHADERLANG
							AI_MATKEY_SHADER_VERTEX
							AI_MATKEY_SHADER_FRAGMENT
							AI_MATKEY_SHADER_GEO
							AI_MATKEY_SHADER_TESSELATION
							AI_MATKEY_SHADER_PRIMITIVE
							AI_MATKEY_SHADER_COMPUTE
*/
		}

		static void ASSIMPProcessNodeLog(aiNode *node, const aiScene *scene)
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
				ASSIMPProcessMeshLog(mesh, scene);
			}

			//  Process child nodes
			for (uint32_t i = 0; i < node->mNumChildren; i++)
			{
				ASSIMPProcessNodeLog(node->mChildren[i], scene);
			}
		}

		static void ASSIMPLog(const std::string& filepath, uint32_t flags)
		{
			Assimp::Importer importer;
			const aiScene *scene = importer.ReadFile(filepath, flags);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				Log::error("Cannot load: {0}, ASSIMP Error {1}", filepath, importer.GetErrorString());
				return;
			}
			ASSIMPProcessNodeLog(scene->mRootNode, scene);
			/*
			Log::info("ANIMATIONS");

			for (int i = 0; i < scene->mNumAnimations; i++)
			{
				aiAnimation* anim = scene->mAnimations[i];

				Log::info("TicksPerSecond: {0} Duration: {1}", anim->mTicksPerSecond, anim->mDuration);

				for (int j = 0; j < anim->mNumChannels; j++)
				{
					aiNodeAnim* nodeAnim = anim->mChannels[j];
					Log::info("Bone: {0}", nodeAnim->mNodeName.C_Str());

					Log::info("Position keyframes:");
					for (int k = 0; k < nodeAnim->mNumPositionKeys; k++)
					{
						aiVectorKey key = nodeAnim->mPositionKeys[k];
						Log::info("Time: {0} Position: {1}, {2}, {3}", key.mTime, key.mValue.x, key.mValue.y, key.mValue.z);
					}

					Log::info("Rotation keyframes:");
					for (int k = 0; k < nodeAnim->mNumRotationKeys; k++)
					{
						aiQuatKey key = nodeAnim->mRotationKeys[k];
						Log::info("Time: {0} Rotation (Quat): {1}, {2}, {3}, {4}", key.mTime, key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z);
					}

					Log::info("Scaling keyframes:");
					for (int k = 0; k < nodeAnim->mNumScalingKeys; k++)
					{
						aiVectorKey key = nodeAnim->mScalingKeys[k];
						Log::info("Time: {0} Scaling: {1}, {2}, {3}", key.mTime, key.mValue.x, key.mValue.y, key.mValue.z);
					}

					// Could do the same for mesh and morph mesh animations
				}
				
			}
			*/

		}
	}
}