using ImGuiNET;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Numerics;
using FireEngine;
using FireEditor;

namespace FireEngine.Editor
{
    class WindowHierarchy : iWindow
    {
        ImGuiTreeNodeFlags flagsBase = ImGuiTreeNodeFlags.DefaultOpen
            | ImGuiTreeNodeFlags.OpenOnArrow;

        bool createdDefault = false;

        static int tmp_t = 0;

        public void Init()
        {
            int count = SceneNative.SceneCount();
            string _name = "Scene";
            if (count > 0)
                _name = string.Format("Scene ({0})", count);
            SceneNative.SceneCreate(_name);

            SceneNative.SceneHandle handle = SceneNative.SceneFindByIndex(0);

            Scene.Open(handle);
        }

        void OnCreateDefaultComponent()
        {
            if (Scene.current == null)
                return;

            var mainCamera = Camera.Inner_Create(Scene.current.root.handle, "Main Camera");
            SceneNode sceneCamera = new SceneNode();
            sceneCamera.name = "Main Camera";
            sceneCamera.handle = mainCamera.transformHandle;
            sceneCamera.component = mainCamera;
            sceneCamera.inspector = new InspectorCamera();
            Scene.current.root.AddChild(sceneCamera);

            mainCamera.transform.position = new Vector3(0, 0, -10);
        }

        void OnGUI_RightMenu(SceneNode scene)
        {
            // 暂时不支持创建多个场景
            if (ImGui.MenuItem("Create Scene", false))//!handle.IsValid()))
            {
                int count = SceneNative.SceneCount();
                string _name = "Scene";
                if (count > 0)
                    _name = string.Format("Scene ({0})", count);
                SceneNative.SceneCreate(_name);
            }

            ImGui.Separator();
            if (ImGui.MenuItem("Rename", "F2", false, scene != null))
            {
                //curSelectedTransform = handle;
                renameing = true;
            }

            ImGui.Separator();

            if (ImGui.MenuItem("Create Empty", scene != null))
            {
                int count = TransformNative.TransformChildCount(scene.handle);
                string _name = "GameObject";
                if (count > 0)
                    _name = string.Format("GameObject ({0})", count);
                var gameObject = GameObject.Inner_Create(scene.handle, _name);
                SceneNode gameObjectNode = new SceneNode();
                gameObjectNode.name = _name;
                gameObjectNode.handle = gameObject.transform.transformHandle;
                gameObjectNode.component = gameObject.transform;
                gameObjectNode.inspector = new InspectorTransform();
                Scene.current.root.AddChild(gameObjectNode);
            }

            if(ImGui.BeginMenu("3D Object", scene != null))
            {
                if (ImGui.MenuItem("Cube"))
                {
                    string name = "Cube";
                    IntPtr native = TransformNative.TransformGetNativeByHandle(scene.handle);
                    IntPtr gameObjectNative = AppNative.InnerGeoCreateCube(native, name);
                    var gameObject = GameObject.CreateFromNative(gameObjectNative);
                    SceneNode gameObjectNode = new SceneNode();
                    gameObjectNode.name = name;
                    gameObjectNode.handle = gameObject.transform.transformHandle;
                    gameObjectNode.component = gameObject.transform;
                    gameObjectNode.inspector = new InspectorRenderer();
                    Scene.current.root.AddChild(gameObjectNode);
                }

                ImGui.EndMenu();
            }

            if (ImGui.MenuItem("Camera", scene != null))
            {
                Camera mainCamera = Camera.Inner_Create(scene.handle, "Main Camera");
                SceneNode sceneCamera = new SceneNode();
                sceneCamera.name = "Main Camera";
                sceneCamera.handle = mainCamera.transformHandle;
                sceneCamera.component = mainCamera;
                sceneCamera.inspector = new InspectorCamera();
                Scene.current.root.AddChild(sceneCamera);
            }
        }

        void OnGUI_SceneMenu(Scene scene, string label, bool active)
        {
            if (ImGui.BeginPopupContextItem(label))
            {
                OnGUI_RightMenu(scene.root);
                ImGui.EndPopup();
            }
        }

        void OnGUI_TransfromMenu(SceneNode scene, string label)
        {
            if (ImGui.BeginPopupContextItem(label))
            {
                OnGUI_RightMenu(scene);
                ImGui.EndPopup();
            }
        }

        bool OnGUI_TransformTree(SceneNode scene, int idx, ref bool selected)
        {
            int count = scene.children.Count;

            SceneNode curSelected = null;
            var _selected = Selector.GetSelected();
            if (_selected?.Count > 0)
                curSelected = _selected[0] as SceneNode;

            for (var i = 0; i < count; i++)
            {
                SceneNode node = scene.children[i];
                string label = string.Format("{0}##_Transform_{1}_{2}", node.name, idx, i);

                int childCount = node.children.Count;

                ImGuiTreeNodeFlags flags = flagsBase
                    | (childCount == 0 ? ImGuiTreeNodeFlags.Leaf : ImGuiTreeNodeFlags.None);
                flags |= curSelected == node ? ImGuiTreeNodeFlags.Selected : ImGuiTreeNodeFlags.None;

                if (renameing && curSelected == node)
                    label = string.Format("###_{0}", label);
                if (ImGui.TreeNodeEx(label, flags))
                {
                    if (renameing && curSelected == node)
                    {
                        neo_name = node.name;
                        ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags.EnterReturnsTrue
                            | ImGuiInputTextFlags.AutoSelectAll;
                        if (!ImGui.IsAnyItemActive() && !ImGui.IsMouseClicked(0))
                            ImGui.SetKeyboardFocusHere(0);

                        ImGui.SameLine(ImGui.GetCursorPosX());
                        Vector2 padding = ImGui.GetStyle().FramePadding;
                        padding.Y = 0;
                        ImGui.PushStyleVar(ImGuiStyleVar.FramePadding, padding);

                        if (ImGui.InputText(
                            string.Format("##Transform_{0}_{1}", node.name, i),
                            ref neo_name, 64, inputFlags))
                        {
                            TransformNative.TransformSetName(curSelected.handle, neo_name);
                            renameing = false;
                        }

                        ImGui.PopStyleVar();
                    }
                    if (DragDropManager.DragDrop(node.name, node.handle.idx,
                    DragDropWindow.Hierarchy, DragDropTree.Transforms,
                    "_DDTreeWindow", ImGuiDragDropFlags.None,
                    DragDropManager.OnDragDropAction))
                    {
                        ImGui.TreePop();
                        return false;
                    }

                    OnGUI_TransfromMenu(node, label);
                    OnGUI_TransformTree(node, i, ref selected);
                    ImGui.TreePop();
                }
                else
                {
                    OnGUI_TransfromMenu(node, label);
                }

                if (!selected)
                {
                    bool clicked = ImGui.IsItemClicked();
                    //bool focused = childCount == 0 ? ImGui.IsItemFocused() : ImGui.IsItemHovered();
                    if (clicked)
                    {
                        curSelected = node;
                        selected = true;
                        Selector.Select(node);
                    }
                }
            }


            if (curSelected != null)
            {
                // F2
                if (ImGui.GetIO().KeysDown[113])
                {
                    neo_name = TransformNative.TransformGetName(curSelected.handle);
                    renameing = true;
                }
            }

            return true;
        }

        bool renameing = false;
        string neo_name = "";
        public void OnGUI()
        {
            Scene curSelected = null;
            var _selected = Selector.GetSelected();
            if (_selected?.Count > 0)
                curSelected = _selected[0] as Scene;

            int count = SceneNative.SceneCount();

            if (ImGui.BeginPopupContextWindow("SceneMenu", ImGuiMouseButton.Right))
            {
                OnGUI_RightMenu(null);
                ImGui.EndPopup();
            }

            //if (ImGui.TreeNodeEx("Scenes##Hierachy", flagsBase))
            {
                bool selected = false;
                for (int i = 0; i < count; ++i)
                {
                    SceneNative.SceneHandle handle = SceneNative.SceneFindByIndex(i);
                    string name = SceneNative.SceneGetName(handle);
                    string label = string.Format("{0}##_Scene_{1}", name, i);
                    SceneNative.ActiveOption active = SceneNative.SceneGetActive(handle);

                    Scene.Open(handle);

                    int childCount = Scene.current.root.children.Count;

                    ImGuiTreeNodeFlags flags = flagsBase
                        | (childCount == 0 ? ImGuiTreeNodeFlags.Leaf : ImGuiTreeNodeFlags.None);
                    flags |= curSelected == Scene.current ? ImGuiTreeNodeFlags.Selected : ImGuiTreeNodeFlags.None;

                    if (renameing && curSelected == Scene.current)
                        label = string.Format("###_{0}", label);
                    if (ImGui.CollapsingHeader(label, flags))
                    {

                        if (renameing && curSelected == Scene.current)
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

                        OnGUI_SceneMenu(Scene.current, label, active == SceneNative.ActiveOption.Active);

                        bool dragdroped = DragDropManager.DragDropTarget(handle.idx,
                            DragDropWindow.Hierarchy, DragDropTree.Scenes,
                            "_DDTreeWindow", ImGuiDragDropFlags.None,
                            DragDropManager.OnDragDropAction);

                        bool breakif = !OnGUI_TransformTree(Scene.current.root, i, ref selected);

                        //ImGui.TreePop();

                        if (dragdroped || breakif)
                            break;
                    }
                    else
                    {
                        OnGUI_SceneMenu(Scene.current, label, active == SceneNative.ActiveOption.Active);
                    }

                    if (!selected)
                    {
                        bool clicked = ImGui.IsItemClicked();
                        //bool focused = childCount == 0 ? ImGui.IsItemFocused() : ImGui.IsItemHovered();
                        if (clicked)
                        {
                            Selector.Select(Scene.current);
                            selected = true;
                        }
                    }

                    if (Scene.current != null)
                    {
                        // F2
                        if (ImGui.GetIO().KeysDown[113])
                        {
                            neo_name = SceneNative.SceneGetName(Scene.current.handle);
                            renameing = true;
                        }
                    }
                }

                //ImGui.TreePop();
            }
        }

        public void OnTick()
        {
            if(!createdDefault)
            {
                OnCreateDefaultComponent();
                createdDefault = true;
            }
        }

        public void OnShow()
        {
        }

        public void OnHide()
        {
        }
    }
}
