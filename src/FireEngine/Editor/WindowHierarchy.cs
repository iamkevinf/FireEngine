using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Text;
using FireEngine;
using System.Numerics;

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
            if (ImGui.MenuItem("Rename", "F2", false, handle.IsValid()))
            {
                curSelectedTransform = handle;
                renameing = true;
            }

            ImGui.Separator();

            if (ImGui.MenuItem("Create Empty", handle.IsValid()))
            {
                GameObject.Inner_Create(handle, "Test GameObject");
            }

            if (ImGui.MenuItem("Temp Create Transform", handle.IsValid()))
            {
                int count = TransformNative.TransformChildCount(handle);
                string _name = "Transform";
                if (count > 0)
                    _name = string.Format("Transform ({0})", count);
                TransformNative.TransformCreate(handle, _name);
            }
        }

        void OnGUI_SceneMenu(SceneNative.SceneHandle handle, string label, bool active)
        {
            if (ImGui.BeginPopupContextItem(label))
            {
                var root = SceneNative.SceneGetRoot(handle);
                OnGUI_RightMenu(root);
                ImGui.EndPopup();
            }
        }

        void OnGUI_TransfromMenu(TransformNative.TransformHandle handle, string label)
        {
            if (ImGui.BeginPopupContextItem(label))
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
                string label = string.Format("{0}##_Transform_{1}_{2}", name, idx, i);

                int childCount = TransformNative.TransformChildCount(item);

                ImGuiTreeNodeFlags flags = flagsBase
                    | (childCount == 0 ? ImGuiTreeNodeFlags.Leaf : ImGuiTreeNodeFlags.None);
                flags |= curSelectedTransform == item ? ImGuiTreeNodeFlags.Selected : ImGuiTreeNodeFlags.None;

                if (renameing && curSelectedTransform == item)
                    label = string.Format("###_{0}", label);
                if (ImGui.TreeNodeEx(label, flags))
                {
                    if (renameing && curSelectedTransform == item)
                    {
                        neo_name = name;
                        ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags.EnterReturnsTrue
                            | ImGuiInputTextFlags.AutoSelectAll;
                        if (!ImGui.IsAnyItemActive() && !ImGui.IsMouseClicked(0))
                            ImGui.SetKeyboardFocusHere(0);

                        ImGui.SameLine(ImGui.GetCursorPosX());
                        Vector2 padding = ImGui.GetStyle().FramePadding;
                        padding.Y = 0;
                        ImGui.PushStyleVar(ImGuiStyleVar.FramePadding, padding);

                        if (ImGui.InputText(
                            string.Format("##Transform_{0}_{1}", name, i),
                            ref neo_name, 64, inputFlags))
                        {
                            TransformNative.TransformSetName(curSelectedTransform, neo_name);
                            Debug.LogFormat("neo name is {0}", neo_name);
                            renameing = false;
                        }

                        ImGui.PopStyleVar();
                    }
                    if (DragDropManager.DragDrop(name, item.idx,
                    DragDropWindow.Hierarchy, DragDropTree.Transforms,
                    "_DDTreeWindow", ImGuiDragDropFlags.None,
                    DragDropManager.OnDragDropAction))
                    {
                        ImGui.TreePop();
                        return false;
                    }

                    OnGUI_TransfromMenu(item, label);
                    OnGUI_TransformTree(item, i, ref selected);
                    ImGui.TreePop();
                }
                else
                {
                    OnGUI_TransfromMenu(item, label);
                }

                if (!selected)
                {
                    bool clicked = ImGui.IsItemClicked();
                    bool focused = childCount == 0 ? ImGui.IsItemFocused() : ImGui.IsItemHovered();
                    if (count > 0)
                        Debug.LogFormat("{0} {1} {2}", ImGui.IsItemActive(), ImGui.IsItemActivated(), 
                            ImGui.IsItemToggledOpen());
                    if (clicked || focused)
                    {
                        curSelectedTransform = item;
                        curSelectedScene = SceneNative.SceneHandle.InValid;
                        selected = true;
                    }
                }
            }


            if (curSelectedTransform.IsValid())
            {
                // F2
                if (ImGui.GetIO().KeysDown[113])
                {
                    neo_name = TransformNative.TransformGetName(curSelectedTransform);
                    renameing = true;
                }
            }

            return true;
        }

        private SceneNative.SceneHandle curSelectedScene = SceneNative.SceneHandle.InValid;

        bool renameing = false;
        string neo_name = "";
        public override void OnGUI()
        {
            base.OnGUI();

            int count = SceneNative.SceneCount();

            if (ImGui.BeginPopupContextWindow("SceneMenu", ImGuiMouseButton.Right))
            {
                OnGUI_RightMenu(TransformNative.TransformHandle.InValid);
                ImGui.EndPopup();
            }

            if (ImGui.TreeNodeEx("Scenes##Hierachy", flagsBase))
            {
                bool selected = false;
                for (int i = 0; i < count; ++i)
                {
                    SceneNative.SceneHandle handle = SceneNative.SceneFindByIndex(i);
                    string name = SceneNative.SceneGetName(handle);
                    string label = string.Format("{0}##_Scene_{1}", name, i);
                    SceneNative.ActiveOption active = SceneNative.SceneGetActive(handle);

                    var root = SceneNative.SceneGetRoot(handle);

                    int childCount = TransformNative.TransformChildCount(root);

                    ImGuiTreeNodeFlags flags = flagsBase
                        | (childCount == 0 ? ImGuiTreeNodeFlags.None : ImGuiTreeNodeFlags.None);
                    flags |= curSelectedScene == handle ? ImGuiTreeNodeFlags.Selected : ImGuiTreeNodeFlags.None;

                    if (renameing && curSelectedScene == handle)
                        label = string.Format("###_{0}", label);
                    if (ImGui.TreeNodeEx(label, flags))
                    {

                        if (renameing && curSelectedScene == handle)
                        {
                            neo_name = name;
                            ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags.EnterReturnsTrue
                                | ImGuiInputTextFlags.AutoSelectAll;
                            if (!ImGui.IsAnyItemActive() && !ImGui.IsMouseClicked(0))
                                ImGui.SetKeyboardFocusHere(0);

                            ImGui.SameLine(ImGui.GetCursorPosX());
                            Vector2 padding = ImGui.GetStyle().FramePadding;
                            padding.Y = 0;
                            ImGui.PushStyleVar(ImGuiStyleVar.FramePadding, padding);
                            if (ImGui.InputText(string.Format("##Scene_{0}_{1}", name, i), ref neo_name, 64, inputFlags))
                            {
                                SceneNative.SceneSetName(handle, neo_name);
                                Debug.LogFormat("neo name is {0}", neo_name);
                                renameing = false;
                            }

                            ImGui.PopStyleVar();
                        }

                        OnGUI_SceneMenu(handle, label, active == SceneNative.ActiveOption.Active);

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
                        OnGUI_SceneMenu(handle, label, active == SceneNative.ActiveOption.Active);
                    }

                    if(!selected)
                    {
                        bool clicked = ImGui.IsItemClicked();
                        bool focused = childCount == 0 ? ImGui.IsItemFocused() : ImGui.IsItemHovered();
                        if(clicked || focused)
                        {
                            curSelectedScene = handle;
                            curSelectedTransform = TransformNative.TransformHandle.InValid;
                            selected = true;
                        }
                    }

                    if (curSelectedScene.IsValid())
                    {
                        // F2
                        if (ImGui.GetIO().KeysDown[113])
                        {
                            neo_name = SceneNative.SceneGetName(curSelectedScene);
                            renameing = true;
                        }
                    }
                }

                ImGui.TreePop();
            }
        }
    }
}
