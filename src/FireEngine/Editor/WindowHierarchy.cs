using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Text;

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
                ImGui.EndPopup();
            }
        }

        string selectkey = "";

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

            if (ImGui.TreeNode("Scenes"))
            {
                for (int i = 0; i < SceneNative.SceneCount(); ++i)
                {
                    SceneNative.SceneHandle handle = SceneNative.SceneFindByIndex(i);
                    string name = SceneNative.SceneGetName(handle);
                    SceneNative.ActiveOption active = SceneNative.SceneGetActive(handle);
                    string scenekey = string.Format("scene::{0}", i);

                    ImGuiTreeNodeFlags select = scenekey == selectkey ? ImGuiTreeNodeFlags.Selected : ImGuiTreeNodeFlags.None;

                    bool selected = false;

                    if (ImGui.TreeNodeEx(string.Format("{0}|{1}_{2}", name, active, i), select))
                    {
                        OnGUI_SceneMenu(handle, scenekey, name, active == SceneNative.ActiveOption.Active);

                        var root = SceneNative.SceneGetRoot(handle);
                        ImGui.TreePop();
                    }
                    else
                    {
                        OnGUI_SceneMenu(handle, scenekey, name, active == SceneNative.ActiveOption.Active);
                    }

                    if (!selected && ImGui.IsItemClicked())
                        selectkey = scenekey;

                }
                ImGui.TreePop();
            }
        }
    }
}
