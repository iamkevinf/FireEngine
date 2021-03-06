#ifndef __MESH_H__
#define __MESH_H__

#include "core/object.h"

#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "color.h"

#include <glm/glm.hpp>
#include <bgfx/bgfx.h>

struct aiMesh;
struct aiScene;
class Material;

namespace FireEngine
{
	class Mesh : public IObject
	{
	public:
		static std::shared_ptr<Mesh> Create(bool dynamic = false);
		static std::shared_ptr<Mesh> LoadFromMem(ByteBuffer data, bool dynamic = false);

		virtual ~Mesh();
		void Tick();

		const std::shared_ptr<VertexBuffer>& GetVertexBuffer()const { return vertex_buffer; }
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer()const { return index_buffer; }
		bgfx::VertexBufferHandle GetVertexBufferHandle()const { return vertex_buffer_handle; }
		bgfx::DynamicVertexBufferHandle GetDynamicVertexBufferHandle()const { return dynamic_vertex_buffer_handle; }
		bgfx::IndexBufferHandle GetIndexBufferHandle(int index)const { return index_buffer_handle[index]; }
		bgfx::DynamicIndexBufferHandle GetDynamicIndexBufferHandle(int index)const { return dynamic_index_buffer_handle[index]; }

		void GetIndexRange(uint32_t submesh_index, uint32_t& start, uint32_t& count);

		bool IsDynamic()const { return dynamic; }

		bgfx::VertexBufferHandle& GetVertexBufferHandleRef(){return vertex_buffer_handle;}
		bgfx::DynamicVertexBufferHandle& GetDynamicVertexBufferHandleRef() { return dynamic_vertex_buffer_handle; }
		std::vector<bgfx::IndexBufferHandle>& GetIndexBufferHandleRef() { return index_buffer_handle; }
		std::vector<bgfx::DynamicIndexBufferHandle>& GetDynamicIndexBufferHandleRef() { return dynamic_index_buffer_handle; }
		bgfx::VertexLayout GetLayout()const { return layout; }


		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uv;
		std::vector<uint32_t> colors;
		std::vector<glm::vec2> uv2;			//Lightmap
		std::vector<glm::vec3> normals;		//Light
		std::vector<glm::vec4> tangents;		//NormalMap
		std::vector<glm::vec4> bone_weights;
		std::vector<glm::vec4> bone_indices;	//Skinned

		struct SubMesh
		{
			uint32_t start;
			uint32_t count;
		};

		std::vector<uint16_t> triangles;
		std::vector<SubMesh> submeshes;
		std::vector<glm::mat4> bind_poses;

	private:
		static void FillVertexBuffer(void* param, const ByteBuffer& buffer);
		static void FillIndexBuffer(void* param, const ByteBuffer& buffer);

		Mesh();
		void UpdateVertexBuffer();
		void UpdateIndexBuffer();

		uint32_t VertexBufferSize()const;
		uint32_t IndexBufferSize()const;

	private:
		std::shared_ptr<VertexBuffer> vertex_buffer;
		std::shared_ptr<IndexBuffer> index_buffer;

		bgfx::VertexBufferHandle vertex_buffer_handle = {bgfx::kInvalidHandle};
		bgfx::DynamicVertexBufferHandle dynamic_vertex_buffer_handle = { bgfx::kInvalidHandle };
		std::vector<bgfx::IndexBufferHandle> index_buffer_handle;
		std::vector<bgfx::DynamicIndexBufferHandle> dynamic_index_buffer_handle;
		bgfx::VertexLayout layout;

		bool dynamic;
	};
}

#endif // __MESH_H__