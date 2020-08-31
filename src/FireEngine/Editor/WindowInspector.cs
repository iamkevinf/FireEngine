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
            // 暂时兼容没修改完的
            var comp = WindowHierarchy.GetComponentSelected();
            if (comp != null)
            {
                comp.OnGUI_Inspector();
                return;
            }

            var selectArr = Selector.GetSelected();
            if (selectArr == null || selectArr.Count == 0)
                return;

            iSelectable selectable = selectArr[0];
            if (selectable == null)
                return;

            if(selectable is Path)
            {
                ImGui.Text(selectable.name);
                return;
            }

            ImGui.Text(string.Format("{0} Import Settings", selectable.name));
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
