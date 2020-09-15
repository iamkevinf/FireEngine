using FireEditor;
using FireEngine;
using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FireEngine.Editor
{
    public class InspectorRenderer : InspectorComponent
    {
        public virtual InspectorType type
        {
            get
            {
                return InspectorType.Renderer;
            }
        }

        protected ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags.DefaultOpen;

        int mat_size = -1;
        int mat_size_ori = 0;

        public override void OnGUIInspector(object userData)
        {
            base.OnGUIInspector(userData);

            Transform transform = userData as Transform;
            if (transform == null)
                return;

            Renderer renderer = transform.gameObject.GetComponent<Renderer>();
            if (renderer == null)
                return;

            Material[] mats = renderer.GetMaterials();
            if (mat_size < 0)
            {
                mat_size = mats.Length;
                mat_size_ori = mat_size;
            }

            if (ImGui.CollapsingHeader("Renderer##Inspector", treeNodeFlags))
            {
                if (ImGui.TreeNodeEx("Materials##Renderer##Inspector", treeNodeFlags))
                {
                    if (ImGui.DragInt("Size", ref mat_size, 1, 0, int.MaxValue))
                    {

                    }

                    for (int i = 0; i < mat_size; ++i)
                    {
                        ImGui.Text($"Element {i}"); ImGui.SameLine();
                        if (i < mat_size_ori)
                        {
                            ImGui.Text(mats[i].name);
                        }
                        else
                        {
                            ImGui.Text("");
                        }
                        ImGui.SameLine();
                        if (ImGui.Button($"o##{i}##Click#Materials#renderer#Inspector"))
                        {

                        }
                    }
                    ImGui.TreePop();
                }
            }
        }

        public override void OnGUIInspectorEnd(object userData)
        {
            Transform transform = userData as Transform;
            if (transform == null)
                return;

            Renderer renderer = transform.gameObject.GetComponent<Renderer>();
            if (renderer == null)
                return;

            Material[] mats = renderer.GetMaterials();
            if (mats.Length <= 0)
                return;

            ImporterMaterial.s_mat_count = 0;
            foreach (var mat in mats)
                ImporterMaterial.OnGUI(mat);
        }
    }
}
