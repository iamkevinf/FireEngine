#include "imgui_utils.h"
#include <imgui_internal.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/gl3w.h>

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

//#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

namespace ImGui
{
    static inline ImVec2 operator*(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x * rhs, lhs.y * rhs); }
    static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
    static inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }

    bool ImageAnimButton(ImTextureID user_texture_id, ImTextureID user_texture_id2, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& uv2, const ImVec2& uv3, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        // Default to using texture ID as ID. User can still push string/integer prefixes.
        // We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
        PushID((void*)user_texture_id);
        const ImGuiID id = window->GetID("#image");
        PopID();

        const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
        const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
        const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
        ItemSize(bb);
        if (!ItemAdd(bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held);

        // Render
        const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
        if (bg_col.w > 0.0f)
            window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));
        if (held)
        {
            window->DrawList->AddImage(user_texture_id2, image_bb.Min, image_bb.Max, uv2, uv3, GetColorU32(tint_col));
        }
        else
        {
            window->DrawList->AddImage(user_texture_id, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));
        }

        return pressed;
    }


    bool ToggleButton(ImTextureID user_texture_id, ImTextureID user_texture_id2, bool* v, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& uv2, const ImVec2& uv3, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        // Default to using texture ID as ID. User can still push string/integer prefixes.
        // We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
        PushID((void*)user_texture_id);
        const ImGuiID id = window->GetID("#image");
        PopID();

        const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
        const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
        const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
        ItemSize(bb);
        if (!ItemAdd(bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held);
        if (ImGui::IsItemClicked())
            *v = !*v;

        // Render
        const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
        if (bg_col.w > 0.0f)
            window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));
        if (*v || held)
        {
            window->DrawList->AddImage(user_texture_id2, image_bb.Min, image_bb.Max, uv2, uv3, GetColorU32(tint_col));
        }
        else
        {
            window->DrawList->AddImage(user_texture_id, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));
        }

        return v;
    }

    void ToggleButton(const char* str_id, bool * v)
    {
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        float height = ImGui::GetFrameHeight();
        float width = height * 1.55f;
        float radius = height * 0.50f;

        ImGui::InvisibleButton(str_id, ImVec2(width, height));
        if (ImGui::IsItemClicked())
            *v = !*v;

        float t = *v ? 1.0f : 0.0f;

        ImGuiContext& g = *GImGui;
        float ANIM_SPEED = 0.08f;
        if (g.LastActiveId == g.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
        {
            float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
            t = *v ? (t_anim) : (1.0f - t_anim);
        }

        ImU32 col_bg;
        if (ImGui::IsItemHovered())
            col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
        else
            col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));

        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
        draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
    }

    bool RadioButton(ImTextureID tid_active, ImTextureID tid_deactivate, const ImVec2& size, bool active)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        // Default to using texture ID as ID. User can still push string/integer prefixes.
        // We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
        PushID((void*)tid_active);
        const ImGuiID id = window->GetID("#image");
        PopID();

        const ImVec2 pos = window->DC.CursorPos;
        const ImRect total_bb(pos, pos + size);
        ItemSize(total_bb, style.FramePadding.y);
        if (!ItemAdd(total_bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
        if (pressed)
            MarkItemEdited(id);

        RenderNavHighlight(total_bb, id);

        if (active)
        {
            window->DrawList->AddImage(tid_active, total_bb.Min, total_bb.Max, ImVec2(0, 0), ImVec2(1, 1));
        }
        else
        {
            window->DrawList->AddImage(tid_deactivate, total_bb.Min, total_bb.Max, ImVec2(0, 0), ImVec2(1, 1));
        }

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
        return pressed;
    }

    // FIXME: This would work nicely if it was a public template, e.g. 'template<T> RadioButton(const char* label, T* v, T v_button)', but I'm not sure how we would expose it..
    bool RadioButton(ImTextureID tid_active, ImTextureID tid_deactivate, const ImVec2& size, int* v, int v_button)
    {
        const bool pressed = RadioButton(tid_active, tid_deactivate, size, *v == v_button);
        if (pressed)
            *v = v_button;
        return pressed;
    }

}