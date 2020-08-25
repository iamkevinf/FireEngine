using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FireEngine.Editor
{
    class WindowInspector : WindowBase
    {
        public override void Init()
        {
            Show();
        }

        public override bool isInWIndowList => true;

        public override bool canDock => true;

        public override string title => "Inspector";

        public override void OnGUI()
        {
            var comp = WindowHierarchy.GetComponentSelected();
            if (comp != null)
                comp.OnGUI_Inspector();
        }
    }
}
