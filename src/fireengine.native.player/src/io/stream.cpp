#include "stream.h"

namespace FireEngine
{

	Stream::Stream() :position(0)
		, closed(0)
		, length(0)
	{

	}

	Stream::~Stream()
	{

	}

	void Stream::Close()
	{
		closed = true;
	}

	uint32_t Stream::Read(void* buffer, uint32_t size)
	{
		uint32_t read = (position + size <= length) ? size : length - position;

		position += read;

		return read;
	}

	uint32_t Stream::Write(void* buffer, uint32_t size)
	{
		uint32_t write = (position + size <= length) ? size : length - position;

		position += write;

		return write;
	}
}
