using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Text;
using static FireEngine.TransformNative;

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

        void DragTransform(TransformHandle handle, string name)
        {
            if (ImGui.BeginDragDropTarget())
            {
                var payload = ImGui.AcceptDragDropPayload("_TREENODE");
                unsafe
                {
                    if (payload.NativePtr != null)
                    {
                        IntPtr data = payload.Data;

                        int size = sizeof(int);
                        byte[] managedArray = new byte[size];
                        System.Runtime.InteropServices.Marshal.Copy(data, managedArray, 0, size);
                        UInt16 from = BitConverter.ToUInt16(managedArray);
                        TransformHandle fromHandle;
                        fromHandle.idx = from;
                        TransformMove(fromHandle, handle);
                    }
                }

                ImGui.EndDragDropTarget();
            }

            if (ImGui.BeginDragDropSource())
            {
                IntPtr data;
                unsafe
                {
                    data = (IntPtr)(&handle.idx);
                }
                ImGui.SetDragDropPayload("_TREENODE", data, sizeof(UInt16));
                ImGui.Text(name);
                ImGui.EndDragDropSource();
            }
        }

        void OnGUI_TransformTree(TransformNative.TransformHandle handle, string selectppkey)
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
                    DragTransform(item, name);

                    OnGUI_TransfromMenu(item, name);
                    OnGUI_TransformTree(item, selectitemkey);
                    ImGui.TreePop();
                }
                else
                {
                    OnGUI_TransfromMenu(item, name);
                }
            }
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
                    if (ImGui.TreeNodeEx(string.Format("{0} {1} {2}", scenename, active, i), select))
                    {
                        OnGUI_SceneMenu(handle, scenekey, scenename, active == SceneNative.ActiveOption.Active);

                        OnGUI_TransformTree(root, scenekey);

                        ImGui.TreePop();
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
