#ifndef __IMGUI_UTILS_H__
#define __IMGUI_UTILS_H__

#include "imgui.h"

namespace ImGui
{
	IMGUI_API bool ImageAnimButton(ImTextureID user_texture_id, ImTextureID user_texture_id2, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& uv2, const ImVec2& uv3, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col);
	IMGUI_API bool ToggleButton(ImTextureID user_texture_id1, ImTextureID user_texture_id2, bool* v, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& uv2, const ImVec2& uv3, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col);
	IMGUI_API void ToggleButton(const char* str_id, bool* v);
	IMGUI_API bool RadioButton(ImTextureID tid_active, ImTextureID tid_deactivate, const ImVec2& size, bool active);
	IMGUI_API bool RadioButton(ImTextureID tid_active, ImTextureID tid_deactivate, const ImVec2& size, int* v, int v_button);
}

#endif // __IMGUI_UTILS_H__
