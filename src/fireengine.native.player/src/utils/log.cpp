#include "log.h"

#include <cstdarg>
#include <string>
#include <iostream>

#include "string_tool.h"
#include "exportapi.h"

LogHacker OnLog = nullptr;

namespace FireEngine
{
	namespace LogPrefix
	{
		const std::u16string Trace = u"Trace: ";
		const std::u16string Debug = u"DEBUG: ";
		const std::u16string Info = u"INFO : ";
		const std::u16string Warn = u"WARN : ";
		const std::u16string Error = u"ERROR: ";
		const std::u16string Fatal = u"FATAL: ";

		const std::u16string & GetLogPrefix(int lvl)
		{
			if (lvl >= FIREENGINE_LOG_LVL_FATAL) return Fatal;
			if (lvl >= FIREENGINE_LOG_LVL_ERROR) return Error;
			if (lvl >= FIREENGINE_LOG_LVL_WARN) return Warn;
			if (lvl >= FIREENGINE_LOG_LVL_INFO) return Info;
			if (lvl >= FIREENGINE_LOG_LVL_DEBUG) return Debug;
			return Trace;
		}
	}

	void _log(int lvl, const char16_t* format, ...)
	{
		std::u16string msg;

		va_list va;
		va_start(va, format);

		U16StringFromVAList(msg, format, va);

		va_end(va);

		const std::u16string & prefix = LogPrefix::GetLogPrefix(lvl);
		msg.insert(0, prefix);

		int level = 0;
		if (lvl <= FIREENGINE_LOG_LVL_FATAL)
			level = 2;
		else if (lvl <= FIREENGINE_LOG_LVL_WARN)
			level = 1;
		else
			level = 0;
		LOG_AFFECT_CS(msg.c_str(), level);

#if _DEBUG
		std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cv;
		std::cout << cv.to_bytes(msg) << std::endl;
#endif

		if (lvl >= FIREENGINE_LOG_LVL_FATAL)
		{
			abort();
		}
	}

	EXPORT_API void feSetOnLogHacker(LogHacker callback)
	{
		OnLog = callback;
	}


} // end of namespace TVR