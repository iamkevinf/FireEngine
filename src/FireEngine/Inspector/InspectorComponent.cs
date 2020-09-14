using FireEditor;
using FireEngine;
using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FireEngine.Editor
{
    public class InspectorComponent : iInspector
    {
        public virtual InspectorType type
        {
            get
            {
                return InspectorType.Component;
            }
        }

        protected ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags.DefaultOpen;
        Vector3 pos = Vector3.Zero;
        Vector3 rot = Vector3.Zero;
        Vector3 scl = Vector3.One;

        public virtual void OnGUIInspector(object userData)
        {
            Component component = userData as Component;
            if (component == null)
                return;

            if (ImGui.CollapsingHeader("Component##Inspector", treeNodeFlags))
            {
                float textWidth = Math.Min(ImGui.GetContentRegionAvail().X * 0.75f, 300);
                ImGui.SetNextItemWidth(textWidth);
                pos = component.gameObject.transform.position;
                ImGui.DragFloat3("##Position##Component##Inspector", ref pos); ImGui.SameLine();
                component.gameObject.transform.position = pos;
                ImGui.Text("Position");

                ImGui.SetNextItemWidth(textWidth);
                rot = component.gameObject.transform.rotation.eulerAngles;
                ImGui.DragFloat3("##Rotation#Component##Inspector", ref rot); ImGui.SameLine();
                FireEngine.Quaternion quaternion = FireEngine.Quaternion.identity;
                quaternion.eulerAngles = rot;
                component.gameObject.transform.rotation = quaternion;
                ImGui.Text("Rotation");

                ImGui.SetNextItemWidth(textWidth);
                scl = component.gameObject.transform.scale;
                ImGui.DragFloat3("##Scale##Component##Inspector", ref scl); ImGui.SameLine();
                component.gameObject.transform.scale = scl;
                ImGui.Text("Scale");
            }
        }

        public virtual void OnGUIInspectorEnd(object userData)
        {

        }
    }
}
