#ifndef __APP_UTILS_H__
#define __APP_UTILS_H__

#include "FW.h"

namespace App
{
	FW_API int App_MainLoop(const char* title, void(*OnInit)(), void(*OnGUI)(), void(*OnExit)());
	FW_API bool App_SetWindowTitle(const char* title);
}

#endif // __APP_UTILS_H__