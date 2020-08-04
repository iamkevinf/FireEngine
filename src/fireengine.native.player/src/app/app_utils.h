#ifndef __APP_UTILS_H__
#define __APP_UTILS_H__

#define EXPORT_API extern "C" __declspec(dllexport)

namespace App
{
	EXPORT_API int App_MainLoop(const char16_t* title, void(*OnInit)(), void(*OnGUI)(), void(*OnExit)());
	EXPORT_API bool App_SetWindowTitle(const char16_t* title);
}

#endif // __APP_UTILS_H__