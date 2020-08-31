using ImGuiNET;
using FireEditor;

namespace FireEngine.Editor
{
    class WindowProject : FireEditor.iWindow
    {
        public void Init()
        {
        }

        private ImGuiTreeNodeFlags baseFlag = ImGuiTreeNodeFlags.None;
        void _OnGUI_Path(Path path)
        {
            for(int i = 0; i < path.paths?.Length; ++i)
            {
                Path elePath = path.paths[i];
                string label = string.Format("{0}##Project_Path_{1}", elePath.name, path.name);
                if (ImGui.TreeNodeEx(label, baseFlag))
                {
                    _OnGUI_Path(elePath);

                    ImGui.TreePop();
                }

            }

            for(int i = 0; i < path.files?.Length; ++i)
            {
                File eleFile = path.files[i];
                string label = string.Format("{0}##Project_File_{1}", eleFile.name, path.name);
                if (ImGui.TreeNodeEx(label, baseFlag | ImGuiTreeNodeFlags.Leaf))
                {
                    ImGui.TreePop();
                }
            }
        }

        public void OnGUI()
        {
            if (Project.current == null)
                return;

            _OnGUI_Path(Project.current.root);
        }

        public void OnTick()
        {
        }

        public void OnShow()
        {
        }
        public void OnHide()
        {
        }
    }
}
