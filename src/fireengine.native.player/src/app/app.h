#pragma once

#include <windows.h>
#include <tchar.h>

bool    ImGui_ImplWin32_Init(void* hwnd);

void ImGui_ImplWin32_Shutdown();


// Helper function to enable DPI awareness without setting up a manifest
void ImGui_ImplWin32_EnableDpiAwareness();

float ImGui_ImplWin32_GetDpiScaleForHwnd(void* hwnd);
int WindowMain(const char16_t* title, void(*OnInit)(), void(*OnGUI)(), void(*OnTick)(), void(*OnExit)());
bool SetWindowTitle(const char16_t* title);

