#include "app.h"

namespace ImGui
{
    int App_MainLoop(const char* title, void(*OnInit)(), void(*OnGUI)(), void(*OnExit)())
    {
        LPCSTR lp = (LPCTSTR)title;
        return WindowMain(lp, OnInit, OnGUI, OnExit);
    }
}


