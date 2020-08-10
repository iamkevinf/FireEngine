using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FireEngine.Editor
{
    class WindowGameView : WindowBase
    {
        public override string title => "Game";
        public override bool isInWIndowList => true;
        public override bool canDock => true;

        public override void Init()
        {
            Show();
        }

        public override void OnGUI()
        {
            TextureHandle tex = FireEngine.FireEngineNative.feGetGameViewTexture();

            Vector2 pos = ImGui.GetCursorScreenPos();
            Vector2 viewPortSize = ImGui.GetWindowContentRegionMax() - ImGui.GetWindowContentRegionMin();
            ImGui.GetWindowDrawList().AddImage((IntPtr)tex.idx, pos, pos + viewPortSize, new Vector2(0, 1), new Vector2(1, 0));
        }
    }
}
