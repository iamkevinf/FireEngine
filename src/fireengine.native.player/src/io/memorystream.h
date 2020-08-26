#ifndef __MEMORYSTREAM_H__
#define __MEMORYSTREAM_H__

#include "stream.h"
#include "utils/bytebuffer.h"
#include <string>

namespace FireEngine
{
	class MemoryStream : public Stream
	{
	public:
		MemoryStream(const ByteBuffer& buffer);

		virtual uint32_t Read(void* buffer, uint32_t size)override;
		virtual uint32_t Write(void* buffer, uint32_t size)override;

		template<class T>
		T Read();
		template<class T>
		void Write(const T& t);

		// ÓÐIO
		std::string ReadString(uint32_t size);

	private:
		ByteBuffer buffer;
	};

	template<class T>
	T MemoryStream::Read()
	{
		T t;
		Read((void*)&t, sizeof(T));
		return t;
	}

	template<class T>
	void MemoryStream::Write(const T& t)
	{
		Write((void*)&t, sizeof(T));
	}
}

#endif // __MEMORYSTREAM_H__
