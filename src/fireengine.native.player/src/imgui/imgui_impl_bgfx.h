#pragma once
#include <imgui.h>
#include "bx/allocator.h"

#define IMGUI_FLAGS_NONE        UINT8_C(0x00)
#define IMGUI_FLAGS_ALPHA_BLEND UINT8_C(0x01)

void     ImGui_Implbgfx_Init();
void     ImGui_Implbgfx_Shutdown();
void     ImGui_Implbgfx_NewFrame();
void     ImGui_Implbgfx_RenderDrawData(ImDrawData* draw_data);

bx::AllocatorI* GetAlloc();
//
//// Use if you want to reset your rendering device without losing Dear ImGui state.
// bool     ImGui_ImplDX9_CreateDeviceObjects();
//void     ImGui_ImplDX9_InvalidateDeviceObjects();


