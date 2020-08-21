using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FireEngine.Editor
{
    class WindowSceneView : WindowBase
    {
        public override string title => "Scene";
        public override bool isInWIndowList => true;
        public override bool canDock => true;

        public override void Init()
        {
            Show();
        }

        public override void OnGUI()
        {
            base.OnGUI();

            TextureHandle tex = FireEngine.FireEngineNative.feGetGameViewTexture();

            Vector2 contentRegionMax = ImGui.GetWindowContentRegionMax();
            Vector2 contentRegionMin = ImGui.GetWindowContentRegionMin();
            Vector2 pos = ImGui.GetCursorScreenPos();
            Vector2 gameViewSize = Vector2.One * 1024 * 2;

            Vector2 center = pos + (contentRegionMin + contentRegionMax) * 0.5f;

            Vector2 bgn, end;

            bgn = center - gameViewSize * 0.5f;
            end = center + gameViewSize * 0.5f;

            ImGui.GetWindowDrawList().AddImage((IntPtr)tex.idx, bgn, end, new Vector2(0, 1), new Vector2(1, 0));
        }
    }
}
