#include "app_utils.h"
#include "app.h"

namespace App
{
    int App_MainLoop(const char* title, void(*OnInit)(), void(*OnGUI)(), void(*OnExit)())
    {
        LPCSTR lp = (LPCTSTR)title;
        return WindowMain(lp, OnInit, OnGUI, OnExit);
    }

    bool App_SetWindowTitle(const char* title)
    {
        LPCSTR lp = (LPCTSTR)title;
        return SetWindowTitle(lp);
    }
}


