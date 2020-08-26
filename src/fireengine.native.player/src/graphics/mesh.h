#ifndef __MESH_H__
#define __MESH_H__

#include "core/object.h"

#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "color.h"

#include <glm/glm.hpp>

namespace FireEngine
{
	class Mesh : public IObject
	{
	public:
		static std::shared_ptr<Mesh> Create(bool dynamic = false);

		void Tick();

		const std::shared_ptr<VertexBuffer>& GetVertexBuffer()const { return vertex_buffer; }
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer()const { return index_buffer; }

		void GetIndexRange(uint32_t submesh_index, uint32_t& start, uint32_t& count);

		bool IsDynamic()const { return dynamic; }

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uv;
		std::vector<Color> colors;
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

		bool dynamic;

	};
}

#endif // __MESH_H__