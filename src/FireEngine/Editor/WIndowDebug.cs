using FireEditor;
using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Text;

namespace FireEngine.Editor
{
    class WindowDebug : iWindow
    {
        uint debug = (uint)bgfx.Debug.BGFX_DEBUG_NONE;
        private bool debugGame = false;

        public void Init()
        {
        }

        public void OnGUI()
        {
            ImGui.CheckboxFlags("BGFX_DEBUG_WIREFRAME##DebugWindow", ref debug, (uint)bgfx.Debug.BGFX_DEBUG_WIREFRAME);
            ImGui.CheckboxFlags("BGFX_DEBUG_IFH##DebugWindow", ref debug, (uint)bgfx.Debug.BGFX_DEBUG_IFH);
            ImGui.CheckboxFlags("BGFX_DEBUG_STATS##DebugWindow", ref debug, (uint)bgfx.Debug.BGFX_DEBUG_STATS);
            ImGui.CheckboxFlags("BGFX_DEBUG_TEXT##DebugWindow", ref debug, (uint)bgfx.Debug.BGFX_DEBUG_TEXT);
            ImGui.CheckboxFlags("BGFX_DEBUG_PROFILER##DebugWindow", ref debug, (uint)bgfx.Debug.BGFX_DEBUG_PROFILER);
            ImGui.Checkbox("Debug Game?", ref debugGame);

            FireEngineNative.febgfxSetDebug(debug, debugGame);
        }

        public void OnHide()
        {
        }

        public void OnShow()
        {
        }

        public void OnTick()
        {
        }
    }
}
