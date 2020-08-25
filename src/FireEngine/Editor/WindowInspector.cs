using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FireEngine.Editor
{
    class WindowInspector : WindowBase
    {
        public override void Init()
        {
            Show();
        }

        public override bool isInWIndowList => true;

        public override bool canDock => true;

        public override string title => "Inspector";

        public override void OnGUI()
        {
            float r = 0, g = 0, b = 0, a = 0; ;
            Vector4 color = ImGui.ColorConvertU32ToFloat4(WindowSceneView.s_clearColor);
            a = color.X;
            b = color.Y;
            g = color.Z;
            r = color.W;
            Vector4 color4 = Vector4.Zero;
            color4.X = r;
            color4.Y = g;
            color4.Z = b;
            color4.W = a;
            ImGui.ColorEdit4("clear color", ref color4);
            color.W = color4.X;
            color.Z = color4.Y;
            color.Y = color4.Z;
            color.X = color4.W;
            WindowSceneView.s_clearColor = ImGui.ColorConvertFloat4ToU32(color);
        }
    }
}
