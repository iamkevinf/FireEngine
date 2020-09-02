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
		auto offset = myMesh->vertices.size();
		auto verticesNum = mesh->mNumVertices;

		for (int i = 0; i < mesh->mNumVertices; ++i)
		{
			auto index = i;
			if(mesh->HasPositions())
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
				myMesh->normals.push_back({ 0,0,0});

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
				for (auto j = 0; j < mesh->mBones[i]->mNumWeights; ++j)
				{
					if (j <= 3)
					{
						auto tmp1x = mesh->mBones[i]->mWeights[0].mWeight;
						auto tmp1y = mesh->mBones[i]->mWeights[1].mWeight;
						auto tmp1z = mesh->mBones[i]->mWeights[2].mWeight;
						auto tmp1w = mesh->mBones[i]->mWeights[3].mWeight;

						auto tmp2x = mesh->mBones[i]->mWeights[0].mVertexId;
						auto tmp2y = mesh->mBones[i]->mWeights[1].mVertexId;
						auto tmp2z = mesh->mBones[i]->mWeights[2].mVertexId;
						auto tmp2w = mesh->mBones[i]->mWeights[3].mVertexId;


						myMesh->bone_weights.push_back({tmp1x, tmp1y, tmp1z, tmp1w });
						myMesh->bone_indices.push_back({ tmp2x, tmp2y, tmp2z, tmp2w });
					}
				}
			}
			else
			{
				myMesh->bone_weights.push_back({ 0,0,0,0 });
				myMesh->bone_indices.push_back({ 0,0,0,0 });
			}

		}
		Mesh::SubMesh subMesh;
		subMesh.start = myMesh->triangles.size();
		for (auto i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				myMesh->triangles.push_back(offset + face.mIndices[j]);
		}
		subMesh.count = myMesh->triangles.size() - subMesh.start;
		myMesh->submeshes.push_back(subMesh);
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
		_processNode(scene->mRootNode, scene, 0, mesh);

		mesh->Tick();

		const bgfx::Memory* vertexData = bgfx::makeRef
		(
			mesh->vertex_buffer->GetLocalBuffer()->Bytes(),
			mesh->vertex_buffer->GetSize()
		);

		if (mesh->dynamic)
		{
			mesh->dynamic_vertex_buffer_handle = bgfx::createDynamicVertexBuffer
			(
				vertexData,
				mesh->layout
			);
		}
		else
		{

			mesh->vertex_buffer_handle = bgfx::createVertexBuffer
			(
				vertexData,
				mesh->layout
			);
		}

		for (int i = 0; i < mesh->submeshes.size(); ++i)
		{
			Mesh::SubMesh subMesh = mesh->submeshes[i];

			const bgfx::Memory* indexData = bgfx::makeRef
			(
				mesh->index_buffer->GetLocalBuffer()->Bytes()
				+ subMesh.start * sizeof(uint16_t),
				subMesh.count * sizeof(uint16_t)
			);

			if (mesh->dynamic)
			{
				mesh->dynamic_index_buffer_handle.push_back
				(
					bgfx::createDynamicIndexBuffer(indexData)
				);
			}
			else
			{
				mesh->index_buffer_handle.push_back
				(
					bgfx::createIndexBuffer(indexData)
				);
			}
		}

		if (mesh->submeshes.size() == 1)
			mesh->submeshes.resize(0);
	}

} // end of namespace FireEngine
