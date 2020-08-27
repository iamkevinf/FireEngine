#include "mesh.h"

#include "io/memorystream.h"
#include "vertexattribute.h"

namespace FireEngine
{
	const char* VERTEX_ATTR_TYPES[(int)VertexAttributeType::Count] =
	{
		"Vertex",
		"Color",
		"Texcoord",
		"Texcoord2",
		"Normal",
		"Tangent",
		"BlendWeight",
		"BlendIndices"
	};

	const int VERTEX_ATTR_SIZES[(int)VertexAttributeType::Count] = {
		12, 16, 8, 8, 12, 16, 16, 16
	};

	const int VERTEX_ATTR_OFFSETS[(int)VertexAttributeType::Count] =
	{
		0, 12, 28, 36, 44, 56, 72, 88
	};

	const uint32_t VERTEX_STRIDE = 104;

	Mesh::Mesh() :dynamic(false)
	{
		SetName("Mesh");
	}

	std::shared_ptr<Mesh> Mesh::Create(bool dynamic/*=false*/)
	{
		std::shared_ptr<Mesh> mesh(new Mesh);

		mesh->dynamic = dynamic;

		mesh->layout
			.begin()
			.add(bgfx::Attrib::Enum::Position, 3, bgfx::AttribType::Enum::Float, false)
			.add(bgfx::Attrib::Enum::Color0, 4, bgfx::AttribType::Enum::Uint8, true)
			.end();

		return mesh;
	}

	std::shared_ptr<Mesh> Mesh::LoadFromMem(ByteBuffer data, bool dynamic)
	{
		auto mesh = Mesh::Create(dynamic);

		auto ms = MemoryStream(data);
		int vc = ms.Read<int>();
		mesh->vertices.resize(vc);
		mesh->colors.resize(vc);
		for (int i = 0; i < vc; ++i)
		{
			glm::vec3 pos = ms.Read<glm::vec3>();
			mesh->vertices[i] = pos;
			uint32_t color = ms.Read<uint32_t>();
			mesh->colors[i] = color;
		}
		int ic = ms.Read<int>();
		mesh->triangles.resize(ic);
		for (int i = 0; i < ic; ++i)
		{
			uint16_t triangle = ms.Read<uint16_t>();
			mesh->triangles[i] = triangle;
		}

		mesh->Tick();

		return mesh;
	}

	void Mesh::Tick()
	{
		UpdateVertexBuffer();
		UpdateIndexBuffer();
	}

	void Mesh::UpdateVertexBuffer()
	{
		uint32_t buffer_size = VertexBufferSize();
		bool dynamic = IsDynamic();

		if (!vertex_buffer || vertex_buffer->GetSize() < buffer_size)
			vertex_buffer = VertexBuffer::Create(buffer_size, dynamic);

		vertex_buffer->Fill(this, Mesh::FillVertexBuffer);

		if (dynamic)
		{
			if (!bgfx::isValid(dynamic_vertex_buffer_handle))
			{
				dynamic_vertex_buffer_handle = bgfx::createDynamicVertexBuffer(
					bgfx::makeRef(vertex_buffer->GetLocalBuffer()->Bytes(),
						vertex_buffer->GetLocalBuffer()->Size()), layout
				);
			}

			bgfx::update(dynamic_vertex_buffer_handle, 0,
				bgfx::makeRef(vertex_buffer->GetLocalBuffer()->Bytes(),
					vertex_buffer->GetLocalBuffer()->Size()));

		}
		else
		{
			if (!bgfx::isValid(vertex_buffer_handle))
			{
				vertex_buffer_handle = bgfx::createVertexBuffer(
					bgfx::makeRef(vertex_buffer->GetLocalBuffer()->Bytes(),
						vertex_buffer->GetLocalBuffer()->Size()), layout
				);
			}
		}
	}

	void Mesh::UpdateIndexBuffer()
	{
		uint32_t buffer_size = IndexBufferSize();
		bool dynamic = IsDynamic();

		if (!index_buffer || index_buffer->GetSize() < buffer_size)
			index_buffer = IndexBuffer::Create(buffer_size, dynamic);

		index_buffer->Fill(this, Mesh::FillIndexBuffer);

		if (dynamic)
		{
			if (!bgfx::isValid(dynamic_index_buffer_handle))
			{
				dynamic_index_buffer_handle = bgfx::createDynamicIndexBuffer(
					bgfx::makeRef(index_buffer->GetLocalBuffer()->Bytes(),
						index_buffer->GetLocalBuffer()->Size())
				);
			}

			bgfx::update(dynamic_index_buffer_handle, 0,
				bgfx::makeRef(index_buffer->GetLocalBuffer()->Bytes(),
					index_buffer->GetLocalBuffer()->Size()));
		}
		else
		{
			if (!bgfx::isValid(index_buffer_handle))
			{
				index_buffer_handle = bgfx::createIndexBuffer(
					bgfx::makeRef(index_buffer->GetLocalBuffer()->Bytes(),
						index_buffer->GetLocalBuffer()->Size())
				);
			}
		}
	}

	uint32_t Mesh::VertexBufferSize() const
	{
		return VERTEX_STRIDE * (uint32_t)vertices.size();
	}

	uint32_t Mesh::IndexBufferSize() const
	{
		return (uint32_t)triangles.size() * sizeof(uint16_t);
	}

	void Mesh::FillVertexBuffer(void* param, const ByteBuffer& buffer)
	{
		Mesh* mesh = (Mesh*)param;
		MemoryStream ms = MemoryStream(buffer);

		uint32_t count = (uint32_t)mesh->vertices.size();
		for (uint32_t i = 0; i < count; ++i)
		{
			ms.Write<glm::vec3>(mesh->vertices[i]);

			if (mesh->colors.empty())
				ms.Write<uint32_t>(1);
			else
				ms.Write<uint32_t>(mesh->colors[i]);

			//if (mesh->uv.empty())
			//	ms.Write<glm::vec2>(glm::vec2(0, 0));
			//else
			//	ms.Write<glm::vec2>(mesh->uv[i]);

			//if (mesh->uv2.empty())
			//	ms.Write<glm::vec2>(glm::vec2(0, 0));
			//else
			//	ms.Write<glm::vec2>(mesh->uv2[i]);

			//if (mesh->normals.empty())
			//	ms.Write<glm::vec3>(glm::vec3(0, 0, 0));
			//else
			//	ms.Write<glm::vec3>(mesh->normals[i]);

			//if (mesh->tangents.empty())
			//	ms.Write<glm::vec4>(glm::vec4(0, 0, 0, 0));
			//else
			//	ms.Write<glm::vec4>(mesh->tangents[i]);

			//if (mesh->bone_weights.empty())
			//	ms.Write<glm::vec4>(glm::vec4(0, 0, 0, 0));
			//else
			//	ms.Write<glm::vec4>(mesh->bone_weights[i]);

			//if (mesh->bone_indices.empty())
			//	ms.Write<glm::vec4>(glm::vec4(0, 0, 0, 0));
			//else
			//	ms.Write<glm::vec4>(mesh->bone_indices[i]);
		}

		ms.Close();


	}

	void Mesh::FillIndexBuffer(void* param, const ByteBuffer& buffer)
	{
		Mesh* mesh = (Mesh*)param;
		::memcpy(buffer.Bytes(), (void*)&mesh->triangles[0], mesh->IndexBufferSize());
	}

	void Mesh::GetIndexRange(uint32_t submesh_index, uint32_t& start, uint32_t& count)
	{
		if (submeshes.empty())
		{
			start = 0;
			count = (uint32_t)triangles.size();
		}
		else
		{
			SubMesh submesh = submeshes[submesh_index];

			start = submesh.start;
			count = submesh.count;
		}
	}
}