using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Text;
using FireEngine;

namespace FireEngine.Editor
{
    class WindowHierarchy : WindowBase
    {
        public override string title => "Hierachy";
        public override bool isInWIndowList => true;
        public override bool canDock => true;

        ImGuiTreeNodeFlags flagsBase = ImGuiTreeNodeFlags.DefaultOpen
            | ImGuiTreeNodeFlags.OpenOnArrow;

        public override void Init()
        {
            Show();
        }



        void OnGUI_RightMenu(TransformNative.TransformHandle handle)
        {
            if (ImGui.MenuItem("Create Scene", !handle.IsValid()))
            {
                int count = SceneNative.SceneCount();
                string _name = "Scene";
                if (count > 0)
                    _name = string.Format("Scene ({0})", count);
                SceneNative.SceneCreate(_name);
            }

            ImGui.Separator();

            if (ImGui.MenuItem("Create Empty", handle.IsValid()))
            {
                int count = TransformNative.TransformChildCount(handle);
                string _name = "GameObject";
                if (count > 0)
                    _name = string.Format("GameObject ({0})", count);
                TransformNative.TransformCreate(handle, _name);
            }
        }

        void OnGUI_SceneMenu(SceneNative.SceneHandle handle, string name, int idx, bool active)
        {
            if (ImGui.BeginPopupContextItem(string.Format("{0}_{1}_{2}", name, -1, idx)))
            {
                var root = SceneNative.SceneGetRoot(handle);
                OnGUI_RightMenu(root);
                ImGui.EndPopup();
            }
        }

        void OnGUI_TransfromMenu(TransformNative.TransformHandle handle, string name, int idxParent, int idx)
        {
            if (ImGui.BeginPopupContextItem(string.Format("{0}_{1}_{2}", name, idxParent, idx)))
            {
                OnGUI_RightMenu(handle);
                ImGui.EndPopup();
            }
        }

        private TransformNative.TransformHandle curSelectedTransform = TransformNative.TransformHandle.InValid;
        bool OnGUI_TransformTree(TransformNative.TransformHandle handle, int idx, ref bool selected)
        {
            int count = TransformNative.TransformChildCount(handle);

            for (var i = 0; i < count; i++)
            {
                var item = TransformNative.TransformFindChildByIndex(handle, i);
                string name = TransformNative.TransformGetName(item);

                ImGuiTreeNodeFlags flags = flagsBase 
                    | (TransformNative.TransformChildCount(item) == 0
                        ? ImGuiTreeNodeFlags.Leaf : ImGuiTreeNodeFlags.None);
                flags |= curSelectedTransform == item ? ImGuiTreeNodeFlags.Selected : ImGuiTreeNodeFlags.None;
                if (ImGui.TreeNodeEx(name, flags))
                {
                    if (DragDropManager.DragDrop(name, item.idx,
                        DragDropWindow.Hierarchy, DragDropTree.Transforms,
                        "_DDTreeWindow", ImGuiDragDropFlags.None,
                        DragDropManager.OnDragDropAction))
                    {
                        ImGui.TreePop();
                        return false;
                    }

                    OnGUI_TransfromMenu(item, name, idx, i);
                    OnGUI_TransformTree(item, i, ref selected);
                    ImGui.TreePop();
                }
                else
                {
                    OnGUI_TransfromMenu(item, name, idx, i);
                }

                if (!selected && ImGui.IsItemClicked())
                {
                    curSelectedTransform = item;
                    curSelectedScene = SceneNative.SceneHandle.InValid;
                    selected = true;
                }

            }

            return true;
        }

        private SceneNative.SceneHandle curSelectedScene = SceneNative.SceneHandle.InValid;

        public override void OnGUI()
        {
            base.OnGUI();

            int count = SceneNative.SceneCount();

            if (ImGui.BeginPopupContextWindow("SceneMenu", ImGuiMouseButton.Right))
            {
                OnGUI_RightMenu(TransformNative.TransformHandle.InValid);
                ImGui.EndPopup();
            }

            if (ImGui.TreeNodeEx("Scenes", flagsBase))
            {
                bool selected = false;
                for (int i = 0; i < count; ++i)
                {
                    SceneNative.SceneHandle handle = SceneNative.SceneFindByIndex(i);
                    string name = SceneNative.SceneGetName(handle);
                    SceneNative.ActiveOption active = SceneNative.SceneGetActive(handle);

                    var root = SceneNative.SceneGetRoot(handle);

                    ImGuiTreeNodeFlags flags = flagsBase
                        | (TransformNative.TransformChildCount(root) == 0
                            ? ImGuiTreeNodeFlags.Leaf : ImGuiTreeNodeFlags.None);
                    flags |= curSelectedScene == handle ? ImGuiTreeNodeFlags.Selected : ImGuiTreeNodeFlags.None;
                    if (ImGui.TreeNodeEx(name, flags))
                    {
                        OnGUI_SceneMenu(handle, name, i, active == SceneNative.ActiveOption.Active);

                        bool dragdroped = DragDropManager.DragDropTarget(handle.idx,
                            DragDropWindow.Hierarchy, DragDropTree.Scenes,
                            "_DDTreeWindow", ImGuiDragDropFlags.None,
                            DragDropManager.OnDragDropAction);

                        bool breakif = !OnGUI_TransformTree(root, i, ref selected);

                        ImGui.TreePop();

                        if (dragdroped || breakif)
                            break;
                    }
                    else
                    {
                        OnGUI_SceneMenu(handle, name, i, active == SceneNative.ActiveOption.Active);
                    }

                    if (!selected && ImGui.IsItemClicked())
                    {
                        curSelectedScene = handle;
                        curSelectedTransform = TransformNative.TransformHandle.InValid;
                    }

                }

                ImGui.TreePop();
            }
        }
    }
}
