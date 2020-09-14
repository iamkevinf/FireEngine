#include "loader.h"

#include <bimg/decode.h>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "filesystem/FileSystem.h"

#include "graphics/mesh.h"
#include "graphics/texture2d.h"
#include "graphics/material.h"

#include "math/math.h"


namespace FireEngine
{
	static void imageReleaseCb(void* _ptr, void* _userData)
	{
		BX_UNUSED(_ptr);
		bimg::ImageContainer* imageContainer = (bimg::ImageContainer*)_userData;
		bimg::imageFree(imageContainer);
	}

	bgfx::TextureHandle _loadTexture(bx::FileReaderI* _reader, const char* _filePath, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation, ByteBuffer* pOutMem)
	{
		BX_UNUSED(_skip);
		bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

		uint32_t size;
		void* data = FileSystem::getInstance()->load(_reader, getAllocator(), _filePath, &size);
		if (NULL != data)
		{
			bimg::ImageContainer* imageContainer = bimg::imageParse(getAllocator(), data, size);

			if (NULL != imageContainer)
			{
				if (NULL != _orientation)
				{
					*_orientation = imageContainer->m_orientation;
				}

				const bgfx::Memory* mem = bgfx::makeRef(
					imageContainer->m_data
					, imageContainer->m_size
					, imageReleaseCb
					, imageContainer
				);

				if (pOutMem != NULL)
					pOutMem->ReSet(mem->size, mem->data);

				FileSystem::getInstance()->unload(data);

				if (imageContainer->m_cubeMap)
				{
					handle = bgfx::createTextureCube(
						uint16_t(imageContainer->m_width)
						, 1 < imageContainer->m_numMips
						, imageContainer->m_numLayers
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
						, _flags
						, mem
					);
				}
				else if (1 < imageContainer->m_depth)
				{
					handle = bgfx::createTexture3D(
						uint16_t(imageContainer->m_width)
						, uint16_t(imageContainer->m_height)
						, uint16_t(imageContainer->m_depth)
						, 1 < imageContainer->m_numMips
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
						, _flags
						, mem
					);
				}
				else if (bgfx::isTextureValid(0, false, imageContainer->m_numLayers, bgfx::TextureFormat::Enum(imageContainer->m_format), _flags))
				{
					handle = bgfx::createTexture2D(
						uint16_t(imageContainer->m_width)
						, uint16_t(imageContainer->m_height)
						, 1 < imageContainer->m_numMips
						, imageContainer->m_numLayers
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
						, _flags
						, mem
					);
				}

				if (bgfx::isValid(handle))
				{
					bgfx::setName(handle, _filePath);
				}

				if (NULL != _info)
				{
					bgfx::calcTextureSize(
						*_info
						, uint16_t(imageContainer->m_width)
						, uint16_t(imageContainer->m_height)
						, uint16_t(imageContainer->m_depth)
						, imageContainer->m_cubeMap
						, 1 < imageContainer->m_numMips
						, imageContainer->m_numLayers
						, bgfx::TextureFormat::Enum(imageContainer->m_format)
					);
				}
			}
		}

		return handle;
	}


	bgfx::TextureHandle loadTexture(const char* _name, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation, ByteBuffer* mem)
	{
		return _loadTexture(getFileReader(), _name, _flags, _skip, _info, _orientation, mem);
	}

	static const bgfx::Memory* loadMem(bx::FileReaderI* _reader, const char* _filePath)
	{
		if (bx::open(_reader, _filePath))
		{
			uint32_t size = (uint32_t)bx::getSize(_reader);
			const bgfx::Memory* mem = bgfx::alloc(size + 1);
			bx::read(_reader, mem->data, size);
			bx::close(_reader);
			mem->data[mem->size - 1] = '\0';
			return mem;
		}

		DBG("Failed to load %s.", _filePath);
		return NULL;
	}


	bgfx::ShaderHandle _loadShader(bx::FileReaderI* _reader, const char* prefix, const char* _name)
	{
		char filePath[512];

		const char* shaderPath = "???";

		switch (bgfx::getRendererType())
		{
		case bgfx::RendererType::Noop:
		case bgfx::RendererType::Direct3D9:  shaderPath = "shaders/dx9/";   break;
		case bgfx::RendererType::Direct3D11:
		case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
		case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
		case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
		case bgfx::RendererType::Nvn:        shaderPath = "shaders/nvn/";   break;
		case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
		case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
		case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;

		case bgfx::RendererType::Count:
			BX_ASSERT(false, "You should not be here!");
			break;
		}

		bx::strCopy(filePath, BX_COUNTOF(filePath), prefix);
		bx::strCat(filePath, BX_COUNTOF(filePath), shaderPath);
		bx::strCat(filePath, BX_COUNTOF(filePath), _name);
		bx::strCat(filePath, BX_COUNTOF(filePath), ".bin");

		bgfx::ShaderHandle handle = bgfx::createShader(loadMem(_reader, filePath));
		bgfx::setName(handle, _name);

		return handle;
	}

	bgfx::ShaderHandle loadShader(const char* name, const char* prefix)
	{
		return _loadShader(getFileReader(), prefix, name);
	}

	std::vector<std::shared_ptr<Texture2D>> loadMaterialTextures(aiString directory, aiMaterial* mat, aiTextureType type)
	{
		std::vector<std::shared_ptr<Texture2D>> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			aiString path;
			path.Append(directory.C_Str());
			path.Append("/");
			path.Append(str.C_Str());
			{
				// if texture hasn't been loaded already, load it
				std::shared_ptr<Texture2D> texture = Texture2D::LoadFromFile(path.C_Str());
				if (texture)
					textures.push_back(texture);
			}
		}
		return textures;
	}

	void _processMesh(aiString directory, aiMesh* mesh, const aiScene* scene, uint32_t subMeshCount, std::shared_ptr<Mesh> myMesh, std::shared_ptr<Material> myMat)
	{
		auto offset = myMesh->vertices.size();
		auto verticesNum = mesh->mNumVertices;

		for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
		{
			if (mesh->HasPositions())
			{
				auto tmp = mesh->mVertices[i];
				myMesh->vertices.push_back({ tmp.x, tmp.y, tmp.z });
			}

			if (mesh->HasTextureCoords(0))
			{
				auto tmp = mesh->mTextureCoords[0][i];
				myMesh->uv.push_back({ tmp.x, tmp.y });
			}
			else
			{
				myMesh->uv.push_back({ 0,0 });
			}

			if (mesh->HasVertexColors(0))
			{
				Color color(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a);
				myMesh->colors.push_back(color.GetHex());
			}
			else
			{
				myMesh->colors.push_back(0xffffffff);
			}

			if (mesh->HasTextureCoords(1))
			{
				auto tmp = mesh->mTextureCoords[1][i];
				myMesh->uv2.push_back({ tmp.x, tmp.y });
			}
			else
			{
				myMesh->uv2.push_back({ 0,0 });
			}

			if (mesh->HasNormals())
			{
				auto tmp = mesh->mNormals[i];
				myMesh->normals.push_back({ tmp.x, tmp.y, tmp.z });
			}
			else
			{
				myMesh->normals.push_back({ 0,0,0 });

			}

			if (mesh->HasTangentsAndBitangents())
			{
				auto tmp = mesh->mTangents[i];
				myMesh->tangents.push_back({ tmp.x, tmp.y, tmp.z, 0 });
			}
			else
			{
				myMesh->tangents.push_back({ 0,0,0,0 });
			}

			if (mesh->HasBones())
			{
				glm::vec4 wights{};
				glm::vec4 indices{};

				int currentbone = 0;
				for (uint32_t boneindex = 0; boneindex < mesh->mNumBones; boneindex++)
				{
					for (uint32_t weightindex = 0; weightindex < mesh->mBones[boneindex]->mNumWeights; weightindex++)
					{
						if (mesh->mBones[boneindex]->mWeights[weightindex].mVertexId == i)
						{
							float weight = mesh->mBones[boneindex]->mWeights[weightindex].mWeight;
							if (currentbone == 4)
							{
								break;
							}

							wights[currentbone] = weight;
							indices[currentbone] = (float)boneindex;
							currentbone++;
						}
					}
				}

				myMesh->bone_weights.push_back(wights);
				myMesh->bone_indices.push_back(indices);
			}
			else
			{
				myMesh->bone_weights.push_back({ 0,0,0,0 });
				myMesh->bone_indices.push_back({ 0,0,0,0 });
			}

		}

		Mesh::SubMesh subMesh;
		subMesh.start = (uint32_t)myMesh->triangles.size();
		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				myMesh->triangles.push_back((uint16_t)offset + face.mIndices[j]);
		}
		subMesh.count = (uint32_t)myMesh->triangles.size() - subMesh.start;
		myMesh->submeshes.push_back(subMesh);


		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// 1. diffuse maps
		auto diffuseMaps = loadMaterialTextures(directory, material, aiTextureType_DIFFUSE);
		auto uniformsize = myMat->GetShader()->pass.uniforms.size();

		auto texture_index = 0;
		for (auto uniform_index = 0; uniform_index < uniformsize; ++uniform_index)
		{
			auto uniform = myMat->GetShader()->pass.uniforms[uniform_index];
			if (uniform.type == bgfx::UniformType::Sampler)
			{
				if (texture_index == diffuseMaps.size())
					break;
				myMat->SetTexture(uniform.name, diffuseMaps[texture_index]);
				texture_index++;
			}

		}
	}

	void _processNode(aiString directory, aiNode* node, const aiScene* scene, uint32_t childrenIdx, std::shared_ptr<Mesh> myMesh, std::vector<std::shared_ptr<Material>>& myMats)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			auto myMat = Material::Create("Assets/default.mat");
			_processMesh(directory, mesh, scene, childrenIdx + i, myMesh, myMat);
			myMats.push_back(myMat);
		}

		for (uint32_t i = 0; i < node->mNumChildren; ++i)
		{
			_processNode(directory, node->mChildren[i], scene, childrenIdx + i, myMesh, myMats);
		}
	}

	void loadMesh(const char* path, std::shared_ptr<Mesh> mesh, std::vector<std::shared_ptr<Material>>& mats)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path,
			aiProcess_JoinIdenticalVertices |		// join identical vertices/ optimize indexing
			aiProcess_ValidateDataStructure |		// perform a full validation of the loader's output
			aiProcess_ImproveCacheLocality |		// improve the cache locality of the output vertices
			aiProcess_RemoveRedundantMaterials |	// remove redundant materials
			aiProcess_GenUVCoords |					// convert spherical, cylindrical, box and planar mapping to proper UVs
			aiProcess_TransformUVCoords |			// pre-process UV transformations (scaling, translation ...)
			//aiProcess_FindInstances |				// search for instanced meshes and remove them by references to one master
			aiProcess_LimitBoneWeights |			// limit bone weights to 4 per vertex
			aiProcess_OptimizeMeshes |				// join small meshes, if possible;
			//aiProcess_PreTransformVertices |
			aiProcess_GenSmoothNormals |			// generate smooth normal vectors if not existing
			aiProcess_SplitLargeMeshes |			// split large, unrenderable meshes into sub-meshes
			aiProcess_Triangulate |					// triangulate polygons with more than 3 edges
			aiProcess_CalcTangentSpace |
			aiProcess_ConvertToLeftHanded |			// convert everything to D3D left handed space
			aiProcess_SortByPType);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			printf(importer.GetErrorString());
			return;
		}

		std::string strPath(path);
		std::string directory = strPath.substr(0, strPath.find_last_of('/'));
		_processNode(aiString(directory.c_str()), scene->mRootNode, scene, 0, mesh, mats);

		mesh->Tick();

		const bgfx::Memory* vertexData = bgfx::makeRef
		(
			mesh->GetVertexBuffer()->GetLocalBuffer()->Bytes(),
			mesh->GetVertexBuffer()->GetSize()
		);

		if (mesh->IsDynamic())
		{
			mesh->GetDynamicVertexBufferHandleRef() = bgfx::createDynamicVertexBuffer
			(
				vertexData,
				mesh->GetLayout()
			);
		}
		else
		{

			mesh->GetVertexBufferHandleRef() = bgfx::createVertexBuffer
			(
				vertexData,
				mesh->GetLayout()
			);
		}

		for (int i = 0; i < mesh->submeshes.size(); ++i)
		{
			Mesh::SubMesh subMesh = mesh->submeshes[i];

			const bgfx::Memory* indexData = bgfx::makeRef
			(
				mesh->GetIndexBuffer()->GetLocalBuffer()->Bytes()
				+ subMesh.start * sizeof(uint16_t),
				subMesh.count * sizeof(uint16_t)
			);

			if (mesh->IsDynamic())
			{
				mesh->GetDynamicIndexBufferHandleRef().push_back
				(
					bgfx::createDynamicIndexBuffer(indexData)
				);
			}
			else
			{
				mesh->GetIndexBufferHandleRef().push_back
				(
					bgfx::createIndexBuffer(indexData)
				);
			}
		}

		if (mesh->submeshes.size() == 1)
			mesh->submeshes.resize(0);
	}

} // end of namespace FireEngine
