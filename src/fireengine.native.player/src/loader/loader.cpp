#include "loader.h"

#include <bimg/decode.h>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "filesystem/FileSystem.h"

#include "graphics/mesh.h"


namespace FireEngine
{
	static void imageReleaseCb(void* _ptr, void* _userData)
	{
		BX_UNUSED(_ptr);
		bimg::ImageContainer* imageContainer = (bimg::ImageContainer*)_userData;
		bimg::imageFree(imageContainer);
	}

	bgfx::TextureHandle _loadTexture(bx::FileReaderI* _reader, const char* _filePath, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
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


	bgfx::TextureHandle loadTexture(const char* _name, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
	{
		return _loadTexture(getFileReader(), _name, _flags, _skip, _info, _orientation);
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


	bgfx::ShaderHandle _loadShader(bx::FileReaderI* _reader, const char* _name)
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

		bx::strCopy(filePath, BX_COUNTOF(filePath), shaderPath);
		bx::strCat(filePath, BX_COUNTOF(filePath), _name);
		bx::strCat(filePath, BX_COUNTOF(filePath), ".bin");

		bgfx::ShaderHandle handle = bgfx::createShader(loadMem(_reader, filePath));
		bgfx::setName(handle, _name);

		return handle;
	}

	bgfx::ShaderHandle loadShader(const char* name)
	{
		return _loadShader(getFileReader(), name);
	}

	void _processMesh(aiMesh* mesh, const aiScene* scene, uint32_t subMeshCount, std::shared_ptr<Mesh> myMesh)
	{
		auto verticesNum = mesh->mNumVertices;
		uint32_t offset = 0;
		if (subMeshCount > 0)
		{
			Mesh::SubMesh subMesh;
			subMesh.count = subMeshCount;
			subMesh.start = myMesh->vertices.size();
			myMesh->submeshes.push_back(subMesh);
			offset = subMesh.start;
			verticesNum += offset;
		}

		myMesh->vertices.resize(verticesNum);
		myMesh->uv.resize(verticesNum);
		myMesh->colors.resize(verticesNum);
		myMesh->uv2.resize(verticesNum);
		myMesh->normals.resize(verticesNum);
		myMesh->tangents.resize(verticesNum);
		myMesh->bone_weights.resize(verticesNum);
		myMesh->bone_indices.resize(verticesNum);


		for (int i = 0; i < mesh->mNumVertices; ++i)
		{
			auto index = i + offset;
			myMesh->vertices[index].x = mesh->mVertices[i].x;
			myMesh->vertices[index].y = mesh->mVertices[i].y;
			myMesh->vertices[index].z = mesh->mVertices[i].z;

			if (mesh->mTextureCoords[0])
			{
				myMesh->uv[index].x = mesh->mTextureCoords[0][i].x;
				myMesh->uv[index].y = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				myMesh->uv[index] = { 0,0 };
			}

			if (mesh->mColors[0])
			{
				Color color(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a);
				myMesh->colors[index] = color.GetHex();
			}
			else
			{
				myMesh->colors[index] = 0xffffffff;
			}

			if (mesh->mTextureCoords[1])
			{
				myMesh->uv2[index].x = mesh->mTextureCoords[1][i].x;
				myMesh->uv2[index].y = mesh->mTextureCoords[1][i].y;
			}
			else
			{
				myMesh->uv2[index] = { 0,0 };
			}

			if (mesh->HasNormals())
			{
				myMesh->normals[index].x = mesh->mNormals[i].x;
				myMesh->normals[index].y = mesh->mNormals[i].y;
				myMesh->normals[index].z = mesh->mNormals[i].z;
			}
			else
			{
				myMesh->normals[index] = { 0,0,0 };
			}

			if (mesh->mTangents)
			{
				myMesh->tangents[index].x = mesh->mTangents[i].x;
				myMesh->tangents[index].y = mesh->mTangents[i].y;
				myMesh->tangents[index].z = mesh->mTangents[i].z;
				myMesh->tangents[index].w = 0;

			}
			else
			{
				myMesh->tangents[index] = { 0,0,0,0 };
			}

			if (mesh->mBones)
			{
				for (auto j = 0; j < mesh->mBones[i]->mNumWeights; ++j)
				{
					if (j <= 3)
					{
						myMesh->bone_weights[index][j] = mesh->mBones[i]->mWeights[0].mWeight;
						myMesh->bone_weights[index][j] = mesh->mBones[i]->mWeights[0].mVertexId;
					}
				}
			}
			else
			{
				myMesh->bone_weights[index] = { 0,0,0,0 };
				myMesh->bone_indices[index] = { 0,0,0,0 };
			}

		}

		for (auto i = 0; i < mesh->mNumFaces; i++)
		{
			auto index = i + offset;
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				myMesh->triangles.push_back(face.mIndices[j]);
		}

	}

	void _processNode(aiNode* node, const aiScene* scene, uint32_t childrenIdx, std::shared_ptr<Mesh> myMesh)
	{
		for (auto i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			_processMesh(mesh, scene, childrenIdx + i, myMesh);
		}

		for (auto i = 0; i < node->mNumChildren; ++i)
		{
			_processNode(node->mChildren[i], scene, childrenIdx + i, myMesh);
		}
	}

	void loadMesh(const char* path, std::shared_ptr<Mesh> mesh)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			return;
		}

		std::string strPath(path);
		std::string directory = strPath.substr(0, strPath.find_last_of('/'));
		_processNode(scene->mRootNode, scene, 0, mesh);
	}

} // end of namespace FireEngine
