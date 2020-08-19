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


        public override void Init()
        {
            Show();
        }

        void OnGUI_SceneMenu(SceneNative.SceneHandle handle, string selectkey, string name, bool active)
        {
            if (ImGui.BeginPopupContextItem(selectkey))
            {
                if (ImGui.MenuItem("Change Name"))
                {
                }
                if (ImGui.MenuItem("Create Empty"))
                {
                    var root = SceneNative.SceneGetRoot(handle);
                    TransformNative.TransformCreate(root, "Empty");
                }

                ImGui.EndPopup();
            }
        }

        void OnGUI_TransfromMenu(TransformNative.TransformHandle handle, string name)
        {
            if (ImGui.BeginPopupContextItem(name))
            {
                if (ImGui.MenuItem("Change Name"))
                {
                }
                if (ImGui.MenuItem("Create Empty"))
                {
                    TransformNative.TransformCreate(handle, "Empty");
                }
                ImGui.EndPopup();
            }

        }

        bool OnGUI_TransformTree(TransformNative.TransformHandle handle, string selectppkey)
        {
            int count = TransformNative.TransformChildCount(handle);

            for (var i = 0; i < count; i++)
            {
                var item = TransformNative.TransformFindChildByIndex(handle, i);
                string name = TransformNative.TransformGetName(handle) + " " + i;
                string selectitemkey = selectppkey + "_" + i;

                ImGuiTreeNodeFlags select = ImGuiTreeNodeFlags.DefaultOpen
                    | (TransformNative.TransformChildCount(item) == 0 ? ImGuiTreeNodeFlags.Leaf : ImGuiTreeNodeFlags.None);
                if (ImGui.TreeNodeEx(name, select))
                {
                    if(DragDropManager.DragDrop(name, item.idx,
                        DragDropWindow.Hierarchy, DragDropTree.Transforms,
                        "_DDTreeWindow", ImGuiDragDropFlags.None,
                        DragDropManager.OnDragDropAction))
                    {
                        ImGui.TreePop();
                        return false;
                    }

                    OnGUI_TransfromMenu(item, name);
                    OnGUI_TransformTree(item, selectitemkey);
                    ImGui.TreePop();
                }
                else
                {
                    OnGUI_TransfromMenu(item, name);
                }

            }

            return true;
        }

        public override void OnGUI()
        {
            base.OnGUI();

            if (ImGui.BeginPopupContextWindow("Scene", ImGuiMouseButton.Right))
            {
                if (ImGui.MenuItem("Create"))
                {
                    SceneNative.SceneCreate("TestCreateScene");
                }
                ImGui.EndPopup();
            }

            if (ImGui.TreeNodeEx("Scenes", ImGuiTreeNodeFlags.DefaultOpen))
            {
                for (int i = 0; i < SceneNative.SceneCount(); ++i)
                {
                    SceneNative.SceneHandle handle = SceneNative.SceneFindByIndex(i);
                    string scenename = SceneNative.SceneGetName(handle);
                    SceneNative.ActiveOption active = SceneNative.SceneGetActive(handle);
                    string scenekey = string.Format("scene_{0}", i);

                    var root = SceneNative.SceneGetRoot(handle);

                    ImGuiTreeNodeFlags select = ImGuiTreeNodeFlags.DefaultOpen
                        | (TransformNative.TransformChildCount(root) == 0 ? ImGuiTreeNodeFlags.Leaf : ImGuiTreeNodeFlags.None);
                    string name = string.Format("{0} {1} {2}", scenename, active, i);
                    if (ImGui.TreeNodeEx(name, select))
                    {
                        OnGUI_SceneMenu(handle, scenekey, scenename, active == SceneNative.ActiveOption.Active);

                        bool dragdroped = DragDropManager.DragDropTarget(handle.idx,
                            DragDropWindow.Hierarchy, DragDropTree.Scenes,
                            "_DDTreeWindow", ImGuiDragDropFlags.None,
                            DragDropManager.OnDragDropAction);

                        bool breakif = !OnGUI_TransformTree(root, scenekey);

                        ImGui.TreePop();

                        if (dragdroped || breakif)
                            break;
                    }
                    else
                    {
                        OnGUI_SceneMenu(handle, scenekey, scenename, active == SceneNative.ActiveOption.Active);
                    }


                }

                ImGui.TreePop();
            }
        }
    }
}
