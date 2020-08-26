#include "indexbuffer.h"

namespace FireEngine
{
	std::shared_ptr<IndexBuffer> IndexBuffer::Create(int size, bool dynamic)
	{
		std::shared_ptr<IndexBuffer> buffer(new IndexBuffer());

		buffer->size = size;
		buffer->_Create(BufferType::Index, dynamic);

		return buffer;
	}
}