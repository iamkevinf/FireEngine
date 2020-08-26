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

        private static Dictionary<TransformNative.TransformHandle, Component> s_pool = new Dictionary<TransformNative.TransformHandle, Component>();
        public static void AddToPool(TransformNative.TransformHandle handle, Component component)
        {
            if (s_pool.ContainsKey(handle))
                s_pool[handle] = component;
            else
                s_pool.Add(handle, component);
        }

        public static Component GetComponent(TransformNative.TransformHandle handle)
        {
            Component ret = null;
            s_pool.TryGetValue(handle, out ret);
            return ret;
        }


        public static Component GetComponentSelected()
        {
            if (!s_curSelectedTransform.IsValid())
                return null;

            Component ret = null;
            s_pool.TryGetValue(s_curSelectedTransform, out ret);
            return ret;
        }

        public override void Init()
        {
            Show();
        }


        Camera mainCamera;
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
                int count = TransformNative.TransformChildCount(handle);
                string _name = "Transform";
                if (count > 0)
                    _name = string.Format("Transform ({0})", count);
                GameObject.Inner_Create(handle, _name);
            }

            if (ImGui.MenuItem("Camera", handle.IsValid()))
            {
                mainCamera = Camera.Inner_Create(handle, "Main Camera");
                AddToPool(mainCamera.transformHandle, mainCamera);
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

        public static TransformNative.TransformHandle s_curSelectedTransform = TransformNative.TransformHandle.InValid;
        private TransformNative.TransformHandle m_curSelectedTransform = TransformNative.TransformHandle.InValid;
        private TransformNative.TransformHandle curSelectedTransform
        {
            get
            {
                return m_curSelectedTransform;
            }
            set
            {
                m_curSelectedTransform = value;
                s_curSelectedTransform = m_curSelectedTransform;
            }
        }

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

                    if (!selected)
                    {
                        bool clicked = ImGui.IsItemClicked();
                        bool focused = childCount == 0 ? ImGui.IsItemFocused() : ImGui.IsItemHovered();
                        if (clicked || focused)
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

                if (ImGui.IsWindowFocused() && mainCamera != null)
                {

                    if (ImGui.GetIO().KeysDown[87])
                    {
                        Vector3 pos = Vector3.Zero;
                        TransformNative.TransformGetWorldPosition(mainCamera.transformHandle, ref pos);
                        pos.Y++;
                        TransformNative.TransformSetWorldPosition(mainCamera.transformHandle, pos);
                    }
                    if (ImGui.GetIO().KeysDown[83])
                    {
                        Vector3 pos = Vector3.Zero;
                        TransformNative.TransformGetWorldPosition(mainCamera.transformHandle, ref pos);
                        pos.Y--;
                        TransformNative.TransformSetWorldPosition(mainCamera.transformHandle, pos);
                    }
                }
            }
        }
    }
}
