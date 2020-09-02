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
			.add(bgfx::Attrib::Enum::TexCoord0, 2, bgfx::AttribType::Enum::Float, false)
			.add(bgfx::Attrib::Enum::Color0, 4, bgfx::AttribType::Enum::Uint8, true)
			.add(bgfx::Attrib::Enum::TexCoord1, 2, bgfx::AttribType::Enum::Float, true)
			.add(bgfx::Attrib::Enum::Normal, 3, bgfx::AttribType::Enum::Float, false)
			.add(bgfx::Attrib::Enum::Tangent, 4, bgfx::AttribType::Enum::Float, false)
			.add(bgfx::Attrib::Enum::Weight, 4, bgfx::AttribType::Enum::Float, false)
			.add(bgfx::Attrib::Enum::Indices, 4, bgfx::AttribType::Enum::Float, false)
			.end();

		return mesh;
	}

	std::shared_ptr<Mesh> Mesh::LoadFromMem(ByteBuffer data, bool dynamic)
	{
		auto mesh = Mesh::Create(dynamic);

		auto ms = MemoryStream(data);
		int vc = ms.Read<int>();
		mesh->vertices.resize(vc);
		mesh->uv.resize(vc);
		mesh->colors.resize(vc);
		mesh->uv2.resize(vc);
		mesh->normals.resize(vc);
		mesh->tangents.resize(vc);
		mesh->bone_indices.resize(vc);
		mesh->bone_weights.resize(vc);
		for (int i = 0; i < vc; ++i)
		{
			glm::vec3 pos = ms.Read<glm::vec3>();
			mesh->vertices[i] = pos;
			glm::vec2 uv = ms.Read<glm::vec2>();
			mesh->uv[i] = uv;
			uint32_t color = ms.Read<uint32_t>();
			mesh->colors[i] = color;
			glm::vec2 uv2 = ms.Read<glm::vec2>();
			mesh->uv2[i] = uv2;
			glm::vec3 normals = ms.Read<glm::vec3>();
			mesh->normals[i] = normals;
			glm::vec4 tangents = ms.Read<glm::vec4>();
			mesh->tangents[i] = tangents;
			glm::vec4 bone_weights = ms.Read<glm::vec4>();
			mesh->bone_weights[i] = bone_weights;
			glm::vec4 bone_indices = ms.Read<glm::vec4>();
			mesh->bone_indices[i] = bone_indices;
		}
		int ic = ms.Read<int>();
		mesh->triangles.resize(ic);
		for (int i = 0; i < ic; ++i)
		{
			uint32_t triangle = ms.Read<uint16_t>();
			mesh->triangles[i] = triangle;
		}

		mesh->Tick();

		const bgfx::Memory* vertexData = bgfx::makeRef
		(
			mesh->vertex_buffer->GetLocalBuffer()->Bytes(),
			mesh->vertex_buffer->GetSize()
		);

		if (dynamic)
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

		int sc = ms.Read<int>();
		for (int i = 0; i < sc; ++i)
		{
			SubMesh subMesh;
			subMesh.start = ms.Read<uint32_t>();
			subMesh.count = ms.Read<uint32_t>();
			mesh->submeshes.push_back(subMesh);

			const bgfx::Memory* indexData = bgfx::makeRef
			(
				mesh->index_buffer->GetLocalBuffer()->Bytes()
				+ subMesh.start * sizeof(uint16_t),
				subMesh.count * sizeof(uint16_t)
			);

			if (dynamic)
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
			const bgfx::Memory* data = bgfx::makeRef(vertex_buffer->GetLocalBuffer()->Bytes(),
				vertex_buffer->GetLocalBuffer()->Size());
			bgfx::update(dynamic_vertex_buffer_handle, 0, data);
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
			const bgfx::Memory* data = bgfx::makeRef(index_buffer->GetLocalBuffer()->Bytes(),
				index_buffer->GetLocalBuffer()->Size());
			bgfx::update(dynamic_index_buffer_handle[0], 0, data);
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

			if (mesh->uv.empty())
				ms.Write<glm::vec2>(glm::vec2(0, 0));
			else
				ms.Write<glm::vec2>(mesh->uv[i]);

			if (mesh->colors.empty())
				ms.Write<uint32_t>(0xffffffff);
			else
				ms.Write<uint32_t>(mesh->colors[i]);

			if (mesh->uv2.empty())
				ms.Write<glm::vec2>(glm::vec2(0, 0));
			else
				ms.Write<glm::vec2>(mesh->uv2[i]);

			if (mesh->normals.empty())
				ms.Write<glm::vec3>(glm::vec3(0, 0, 0));
			else
				ms.Write<glm::vec3>(mesh->normals[i]);

			if (mesh->tangents.empty())
				ms.Write<glm::vec4>(glm::vec4(0, 0, 0, 0));
			else
				ms.Write<glm::vec4>(mesh->tangents[i]);

			if (mesh->bone_weights.empty())
				ms.Write<glm::vec4>(glm::vec4(0, 0, 0, 0));
			else
				ms.Write<glm::vec4>(mesh->bone_weights[i]);

			if (mesh->bone_indices.empty())
				ms.Write<glm::vec4>(glm::vec4(0, 0, 0, 0));
			else
				ms.Write<glm::vec4>(mesh->bone_indices[i]);
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