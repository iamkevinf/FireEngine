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
            var comp = WindowHierarchy.GetComponentSelected();
            if (comp != null)
                comp.OnGUI_Inspector();
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
