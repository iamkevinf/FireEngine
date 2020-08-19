#pragma once

typedef void (*IMAssertHacker)(const char16_t* _expr, const char16_t* filename, int line);
extern IMAssertHacker onImAssert;

#define WIDESTR(x) u##x
#define WIDEEXPR(x) u#x
#define WIDEFILE(x) WIDESTR(x)

#define IM_ASSERT(_EXPR)  do {if(!(_EXPR)){ onImAssert(WIDEEXPR(_EXPR),WIDEFILE(__FILE__),__LINE__); }}while(0)     // Disable asserts

