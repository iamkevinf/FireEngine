#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#include "buffer.h"

namespace FireEngine
{
	class VertexBuffer : public Buffer
	{
		VertexBuffer() = default;
	public:
		static std::shared_ptr<VertexBuffer> Create(int size, bool dynamic = false);
	};
}

#endif // __VERTEX_BUFFER_H__