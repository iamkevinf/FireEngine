
#pragma once

#include "FW.h"

FW_API int igApp_MainLoop(const char* title, void(*OnInit)(), void(*OnGUI)(), void(*OnExit)());
FW_API bool igSetWindowTitle(const char* title);

