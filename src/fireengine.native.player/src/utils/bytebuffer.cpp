#include "bytebuffer.h"

namespace FireEngine
{
	ByteBuffer::ByteBuffer(int size /* = 0 */) :size(size)
		, bytes(nullptr)
		, weak_ref(false)
	{
		if (size > 0)
		{
			ref_count = std::make_shared<bool>(true);
			bytes = (uint8_t*)malloc(size);
		}
		else
		{
			size = 0;
		}
	}

	ByteBuffer::ByteBuffer(const ByteBuffer& buffer)
	{
		size = buffer.size;
		bytes = buffer.bytes;
		ref_count = buffer.ref_count;
		weak_ref = buffer.weak_ref;
	}

	ByteBuffer::ByteBuffer(uint8_t* bytes, int size) :size(size)
		, bytes(bytes)
		, weak_ref(true)
	{

	}

	ByteBuffer::~ByteBuffer()
	{
		Free();
	}

	ByteBuffer& ByteBuffer::operator =(const ByteBuffer& buffer)
	{
		Free();

		size = buffer.size;
		bytes = buffer.bytes;
		ref_count = buffer.ref_count;
		weak_ref = buffer.weak_ref;;

		return *this;
	}

	void ByteBuffer::Free()
	{
		if (!weak_ref)
		{
			if (ref_count && ref_count.use_count() == 1)
			{
				if (bytes != nullptr)
					free(bytes);
			}
		}
	}

	uint8_t* ByteBuffer::Bytes() const
	{
		return bytes;
	}

	int ByteBuffer::Size() const
	{
		return size;
	}

	uint8_t& ByteBuffer::operator [](int index)
	{
		return bytes[index];
	}

	const uint8_t& ByteBuffer::operator [](int index) const
	{
		return bytes[index];
	}

}