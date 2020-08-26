#include "vertexbuffer.h"

namespace FireEngine
{
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(int size, bool dynamic)
	{
		std::shared_ptr<VertexBuffer> buffer(new VertexBuffer());

		buffer->size = size;
		buffer->_Create(BufferType::Vertex, dynamic);

		return buffer;
	}
}