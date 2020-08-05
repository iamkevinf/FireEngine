using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Text;

namespace FireEngine.Editor
{
    class WindowAbout : iWindow
    {
        public WindowAbout()
        {
            visible = false;
        }

        public bool inwindowlist => false;
        public bool visible
        {
            get;
            private set;
        }

        public bool candock => false;
        public string title => "about";


        public void OnGUI()
        {
            ImGui.Text("this is a about windows中文.");
            if (ImGui.Button("close me!"))
                Hide();
        }

        public void Hide()
        {
            visible = false;
        }

        public void Show()
        {
            visible = true;
        }
    }
}
