#ifndef __VERTEX_ATTRIBUTE_H__
#define __VERTEX_ATTRIBUTE_H__

#include "color.h"
#include <glm/glm.hpp>

namespace FireEngine
{
	enum class VertexAttributeType
	{
		None = -1,

		Vertex = 0,
		Color,
		Texcoord,
		Texcoord2,
		Normal,
		Tangent,
		BlendWeight,
		BlendIndices,

		Count
	};

	struct Vertex
	{
		glm::vec3 vertex;
		Color color;
		glm::vec2 uv;
		glm::vec2 uv2;
		glm::vec3 normal;
		glm::vec4 tangent;
		glm::vec4 bone_weight;
		glm::vec4 bone_indices;
	};

	extern const char* VERTEX_ATTR_TYPES[(int)VertexAttributeType::Count];
	extern const int VERTEX_ATTR_SIZES[(int)VertexAttributeType::Count];
	extern const int VERTEX_ATTR_OFFSETS[(int)VertexAttributeType::Count];
	extern const uint32_t VERTEX_STRIDE;
}

#endif // __KGE_GRAPHICS_VERTEX_ATTRIBUTE_H__
