#include "app_wrapper.h"
#include "app_utils.h"

FW_API int igApp_MainLoop(const char* title, void(*OnInit)(), void(*OnGUI)(), void(*OnExit)())
{
	return App::App_MainLoop(title, OnInit, OnGUI, OnExit);
}

FW_API bool igSetWindowTitle(const char* title)
{
	return App::App_SetWindowTitle(title);
}
