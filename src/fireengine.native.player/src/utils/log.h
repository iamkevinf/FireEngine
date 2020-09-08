#ifndef __LOG_HPP__
#define __LOG_HPP__

typedef void (*LogHacker)(const char16_t* _expr, const char16_t* filename, int line, int level);
extern LogHacker OnLog;

#ifndef WIDESTR
#define WIDESTR(x) u##x
#endif
#ifndef WIDEFILE
#define WIDEFILE(x) WIDESTR(x)
#endif

#define LOG_AFFECT_CS(_EXPR, level)  do {{ OnLog(_EXPR,WIDEFILE(__FILE__),__LINE__,level); }}while(0)     // Disable asserts

#define FIREENGINE_LOG_LVL_TRACE 1
#define FIREENGINE_LOG_LVL_DEBUG 10
#define FIREENGINE_LOG_LVL_INFO  20
#define FIREENGINE_LOG_LVL_WARN  30
#define FIREENGINE_LOG_LVL_ERROR 40
#define FIREENGINE_LOG_LVL_FATAL 50

#ifndef FIREENGINE_LOG_LVL
#   ifdef _RETAIL
#       define FIREENGINE_LOG_LVL KGE_LOG_WARN
#   else
#       define FIREENGINE_LOG_LVL FIREENGINE_LOG_LVL_TRACE
#   endif
#endif

namespace FireEngine
{
	void _log(int lvl, const char16_t* format, ...);
}

#if FIREENGINE_LOG_LVL_TRACE >= FIREENGINE_LOG_LVL
#   define FIREENGINE_LOG_TRACE(...) FireEngine::_log(FIREENGINE_LOG_LVL_TRACE, __VA_ARGS__)
#else
#   define FIREENGINE_LOG_TRACE(...) (void)
#endif

#if FIREENGINE_LOG_LVL_DEBUG >= FIREENGINE_LOG_LVL
#   define FIREENGINE_LOG_DEBUG(...) FireEngine::_log(FIREENGINE_LOG_LVL_DEBUG, __VA_ARGS__)
#else
#   define FIREENGINE_LOG_DEBUG(...) (void)
#endif

#if FIREENGINE_LOG_LVL_INFO >= FIREENGINE_LOG_LVL
#   define FIREENGINE_LOG_INFO(...) FireEngine::_log(FIREENGINE_LOG_LVL_INFO, __VA_ARGS__)
#else
#   define FIREENGINE_LOG_INFO(...) (void)
#endif

#if FIREENGINE_LOG_LVL_WARN >= FIREENGINE_LOG_LVL
#   define FIREENGINE_LOG_WARN(...) FireEngine::_log(FIREENGINE_LOG_LVL_WARN, __VA_ARGS__)
#else
#   define FIREENGINE_LOG_WARN(...) (void)
#endif

#if FIREENGINE_LOG_LVL_ERROR >= FIREENGINE_LOG_LVL
#   define FIREENGINE_LOG_ERROR(...) FireEngine::_log(FIREENGINE_LOG_LVL_ERROR, __VA_ARGS__)
#else
#   define FIREENGINE_LOG_ERROR(...) (void)
#endif

#define FIREENGINE_LOG_FATAL(...) FireEngine::_log(FIREENGINE_LOG_LVL_FATAL, __VA_ARGS__)

#define FIREENGINE_STACK_TRACE FIREENGINE_LOG_TRACE("%s() LINE:%d FILE:%s ", __FUNCTION__, __LINE__, __FILE__)

#endif // __LOG_HPP__