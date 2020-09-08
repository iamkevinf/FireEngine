using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FireEngine.Editor
{
    public class InspectorCamera : InspectorComponent
    {
        public override void OnGUIInspector(object userData)
        {
            Camera camera = userData as Camera;
            if (camera == null)
                return;

            base.OnGUIInspector(userData);

            if (ImGui.CollapsingHeader("Camera##Inspector", treeNodeFlags))
            {
                float r = 0, g = 0, b = 0, a = 0; ;
                Vector4 color = ImGui.ColorConvertU32ToFloat4(camera.clearColor);
                a = color.X;
                b = color.Y;
                g = color.Z;
                r = color.W;
                Vector4 color4 = Vector4.Zero;
                color4.X = r;
                color4.Y = g;
                color4.Z = b;
                color4.W = a;
                ImGui.ColorEdit4("Background", ref color4); ImGui.SameLine();
                if (ImGui.Button("↙##Inspector##Camera##PickColor"))
                {
                    ImGui.OpenPopup("PickColor##RightMenu##Inspector##Camera");
                }
                if (ImGui.BeginPopup("PickColor##RightMenu##Inspector##Camera"))
                {
                    ImGui.ColorPicker4("PickColor##Inspector##Camera#", ref color4);
                    ImGui.EndPopup();
                }
                color.W = color4.X;
                color.Z = color4.Y;
                color.Y = color4.Z;
                color.X = color4.W;
                camera.clearColor = ImGui.ColorConvertFloat4ToU32(color4);

            }
        }
    }
}
