#include "buffer.h"

namespace FireEngine
{

	void Buffer::_Create(BufferType type, bool dynamic)
	{
		this->dynamic = dynamic;
	}

	const std::shared_ptr<ByteBuffer>& Buffer::GetLocalBuffer()
	{
		if (!local_buffer)
			local_buffer = std::make_shared<ByteBuffer>(size);

		return local_buffer;
	}

	void Buffer::Fill(void* param, FillFunc fill)
	{
		if (dynamic)
		{
			auto& buffer = *GetLocalBuffer().get();
			fill(param, buffer);
		}
		else
		{
			ByteBuffer buffer(size);
			fill(param, buffer);
		}
	}
}