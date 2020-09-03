#ifndef __BYTE_BUFFER_H__
#define __BYTE_BUFFER_H__

#include <cstdint>
#include <memory>

namespace FireEngine
{
	class ByteBuffer
	{
	public:
		ByteBuffer(int size = 0);
		ByteBuffer(const ByteBuffer& buffer);
		ByteBuffer(uint8_t* bytes, int size);
		~ByteBuffer();

		uint8_t* Bytes() const;
		int Size() const;

		ByteBuffer& operator =(const ByteBuffer& buffer);
		uint8_t& operator[](int index);
		const uint8_t& operator[](int index)const;

		void ReSet(int size, uint8_t* bytes) { this->size = size; this->bytes = bytes; }
	private:
		void Free();

		int size;
		uint8_t* bytes;
		std::shared_ptr<bool> ref_count;
		bool weak_ref;
	};
}

#endif // __KGE_UTIL_BYTE_BUFFER_H__