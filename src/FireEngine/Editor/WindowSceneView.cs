using FireEditor;
using ImGuiNET;
using System;
using System.Numerics;

namespace FireEngine.Editor
{
    public class WindowSceneView : iWindow
    {
        private Vector2 size;
        AppNative.TargetHandle target = new AppNative.TargetHandle();
        IntPtr targetTexture = IntPtr.Zero;
        int targetW;
        int targetH;
        public static uint s_clearColor = 0x314D79FF;

        public void Init()
        {
        }

        public void OnGUI()
        {
            Vector2 canvas_sz = ImGui.GetContentRegionAvail();

            size = canvas_sz;
            if (target.ptr != IntPtr.Zero)
            {
                var texid = AppNative.feTarget_GetImGuiTexID(target);
                ImGui.Image(texid, size);
            }
        }

        public void OnTick()
        {
            var w = (int)size.X;
            var h = (int)size.Y;
            if (w == 0 || h == 0)
                return;

            if (target.ptr == IntPtr.Zero)
            {
                targetW = w;
                targetH = h;
                target = AppNative.feTarget_Create(w, h);
            }
            else if (w != targetW || h != targetH)
            {
                targetW = w;
                targetH = h;
                AppNative.feTarget_Reset(target, w, h);
            }

            AppNative.feFrame_SetTarget(target);
            AppNative.feFrame_Clear(s_clearColor, true, true);
            AppNative.feFrame_Flush();

        }

        public void OnShow()
        {
        }

        public void OnHide()
        {
        }
    }
}
