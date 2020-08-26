#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "utils/bytebuffer.h"
#include "buffertype.h"
#include <memory>

namespace FireEngine
{
	class Buffer
	{
	public:
		const std::shared_ptr<ByteBuffer>& GetLocalBuffer();
		typedef void(*FillFunc)(void*, const ByteBuffer&);
		void Fill(void* param, FillFunc fill);
		uint32_t GetSize()const { return size; }

	protected:
		void _Create(BufferType type, bool dynamic = false);

		uint32_t size;

	private:
		std::shared_ptr<ByteBuffer> local_buffer;
		bool dynamic = false;
	};
}

#endif // __BUFFER_H__