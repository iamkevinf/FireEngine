#ifndef __APP_UTILS_H__
#define __APP_UTILS_H__

#include "imgui.h"

namespace ImGui
{
	IMGUI_API int App_MainLoop(const char* title, void(*OnInit)(), void(*OnGUI)(), void(*OnExit)());

}

#endif // __APP_UTILS_H__