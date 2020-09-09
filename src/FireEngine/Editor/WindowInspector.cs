using FireEditor;
using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FireEngine.Editor
{
    class WindowInspector : iWindow
    {
        public void Init()
        {
        }

        public void OnGUI()
        {
            var selectArr = Selector.GetSelected();
            if (selectArr == null || selectArr.Count == 0)
                return;

            iSelectable selectable = selectArr[0];
            if (selectable == null)
                return;

            if (selectable is Path)
            {
                ImGui.Text(selectable.name);
                return;
            }

            if(selectable is File)
            {
                File f = selectable as File;
                OnGUI_File(f);
            }
            else if(selectable is SceneNode)
            {
                SceneNode node = selectable as SceneNode;
                OnGUI_SceneNode(node);
            }

        }

        void OnGUI_File(File f)
        {
            if (f == null)
                return;

            ImGui.Text(string.Format("{0} Import Settings", f.name));
            iImporter importer = ImporterManager.GetImporter(f.name);
            if(importer != null)
            {
                importer.OnGUI();
            }
        }

        void OnGUI_SceneNode(SceneNode node)
        {
            if (node == null)
                return;

            node.inspector.OnGUIInspector(node.component);
        }

        public void OnHide()
        {
        }

        public void OnShow()
        {
        }

        public void OnTick()
        {
        }
    }
}
