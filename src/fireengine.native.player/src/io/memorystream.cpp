#include "memorystream.h"

namespace FireEngine
{
	MemoryStream::MemoryStream(const ByteBuffer& buffer) :buffer(buffer)
	{
		length = buffer.Size();
	}

	uint32_t MemoryStream::Read(void* buffer, uint32_t size)
	{
		uint32_t pos = position;
		uint32_t read = Stream::Read(buffer, size);

		if (read > 0 && buffer != nullptr)
			::memcpy(buffer, &this->buffer[pos], read);

		return read;
	}

	uint32_t MemoryStream::Write(void* buffer, uint32_t size)
	{
		uint32_t pos = position;
		uint32_t write = Stream::Write(buffer, size);

		if (write > 0 && buffer != nullptr)
			::memcpy(&this->buffer[pos], buffer, write);

		return write;
	}

	std::string MemoryStream::ReadString(uint32_t size)
	{
		ByteBuffer buffer(size);
		Read(buffer.Bytes(), size);

		return std::string((const char*)buffer.Bytes(), buffer.Size());
	}


}

