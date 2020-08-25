using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FireEngine
{
    public class Component
    {
        protected ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags.DefaultOpen;
        Vector3 pos = Vector3.Zero;
        Vector3 rot = Vector3.Zero;
        Vector3 scl = Vector3.One;
        public virtual void OnGUI_Inspector()
        {
            if(ImGui.CollapsingHeader("Transform##Inspector", treeNodeFlags))
            {
                float textWidth = Math.Min(ImGui.GetContentRegionAvail().X * 0.75f, 300);
                ImGui.SetNextItemWidth(textWidth);
                ImGui.DragFloat3("##Position##Component##Inspector", ref pos); ImGui.SameLine();
                ImGui.Text("Position");
                ImGui.SetNextItemWidth(textWidth);
                ImGui.DragFloat3("##Rotation#Component##Inspector", ref rot); ImGui.SameLine();
                ImGui.Text("Rotation");
                ImGui.SetNextItemWidth(textWidth);
                ImGui.DragFloat3("##Scale##Component##Inspector", ref scl); ImGui.SameLine();
                ImGui.Text("Scale");
            }
        }
    }
}
