using ImGuiNET;
using FireEditor;
using System;

namespace FireEngine.Editor
{
    class WindowProject : FireEditor.iWindow
    {
        private ImGuiTreeNodeFlags baseFlag = ImGuiTreeNodeFlags.None;
        private iSelectable curSelected = null;
        private bool clicked = false;
        public void Init()
        {
        }

        void _OnGUI_Selected(Path path, iSelectable selectable, Action<iSelectable> cbk = null)
        {
            bool isPath = selectable is Path;
            string label = string.Format("{0}##Project_{1}_{2}",
                selectable.name, isPath ? "Path" : "File", path.name);
            bool isSelected = curSelected == selectable;

            ImGuiTreeNodeFlags flag = baseFlag
                | (isPath ? ImGuiTreeNodeFlags.None : ImGuiTreeNodeFlags.Leaf)
                | (isSelected ? ImGuiTreeNodeFlags.Selected : ImGuiTreeNodeFlags.None);

            if (ImGui.TreeNodeEx(label, flag))
            {
                if (cbk != null)
                    cbk(selectable);

                ImGui.TreePop();
            }

            if(ImGui.IsItemClicked())
            {
                if(!clicked && curSelected != selectable)
                {
                    curSelected = selectable;
                    Selector.Select(selectable);
                }

                clicked = true;
            }
        }

        void _OnGUI_Path(Path path)
        {
            for (int i = 0; i < path.paths?.Length; ++i)
            {
                Path elePath = path.paths[i];

                _OnGUI_Selected(path, elePath, (selectable)=>
                {
                    _OnGUI_Path(selectable as Path);
                });

            }

            for (int i = 0; i < path.files?.Length; ++i)
            {
                File eleFile = path.files[i];
                _OnGUI_Selected(path, eleFile);
            }
        }

        public void OnGUI()
        {
            if (Project.current == null)
                return;

            clicked = false;
            _OnGUI_Path(Project.current.root);
        }

        public void OnTick()
        {
            if (Project.current != null && Project.current.Changed)
                Project.current.IterFile();
        }

        public void OnShow()
        {
        }
        public void OnHide()
        {
        }

        public void OnDestroy()
        {

        }
    }
}
