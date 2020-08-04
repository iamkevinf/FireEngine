#include "app_utils.h"
#include "app.h"

namespace App
{
    int App_MainLoop(const char16_t* title, void(*OnInit)(), void(*OnGUI)(), void(*OnExit)())
    {
        return WindowMain(title, OnInit, OnGUI, OnExit);
    }

    bool App_SetWindowTitle(const char16_t* title)
    {
        return SetWindowTitle(title);
    }
}


