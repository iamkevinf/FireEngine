#ifndef __STREAM_H__
#define __STREAM_H__

#include <cstdint>

namespace FireEngine
{
	class Stream
	{
	public:
		Stream();
		virtual ~Stream();
		virtual void Close();
		virtual uint32_t Read(void* buffer, uint32_t size);
		virtual uint32_t Write(void* buffer, uint32_t size);

	protected:
		uint32_t position;
		bool closed;
		uint32_t length;
	};
}

#endif // _STREAM_H__