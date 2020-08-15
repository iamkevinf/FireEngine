#ifndef __FIREENGINE_H__
#define __FIREENGINE_H__

#include <bx/debug.h>

#define DBG_STRINGIZE(_x) DBG_STRINGIZE_(_x)
#define DBG_STRINGIZE_(_x) #_x
#define DBG_FILE_LINE_LITERAL "" __FILE__ "(" DBG_STRINGIZE(__LINE__) "): "
#define DBG(_format, ...) bx::debugPrintf(DBG_FILE_LINE_LITERAL "" _format "\n", ##__VA_ARGS__)

namespace FireEngine
{
	static const uint16_t kInvalidHandle = UINT16_MAX;
}

#define FIREENGINE_HANDLE(_name)                                                             \
	struct _name { uint16_t idx; };                                                          \
	inline bool isValid(_name _handle) { return FireEngine::kInvalidHandle != _handle.idx; }

#define FIREENGINE_INVALID_HANDLE { FireEngine::kInvalidHandle }


FIREENGINE_HANDLE(ObjectHandle)

#endif // __FIREENGINE_H__
