#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__

#include "buffer.h"

namespace FireEngine
{
	enum class IndexType
	{
		U16,
		U32
	};

	class IndexBuffer : public Buffer
	{
		IndexBuffer() = default;
	public:
		static std::shared_ptr<IndexBuffer> Create(int size, bool dynamic = false);

	};
}

#endif // __INDEX_BUFFER_H__