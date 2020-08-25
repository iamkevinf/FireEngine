#ifndef __MESSAGEHANDLER_H__
#define __MESSAGEHANDLER_H__

#include <map>
#include <string>

namespace FireEngine
{
	typedef void(*MessageCBK)();
	class MessageHandler
	{
	private:
		std::map<std::string, MessageCBK> pool;

	};
}

#endif // __MESSAGEHANDLER_H__