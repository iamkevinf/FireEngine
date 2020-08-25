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

            Vector2 contentRegionMax = ImGui.GetWindowContentRegionMax();
            Vector2 contentRegionMin = ImGui.GetWindowContentRegionMin();
            Vector2 pos = ImGui.GetCursorScreenPos();
            Vector2 contentSize = contentRegionMax - contentRegionMin;
            Vector2 gameViewSize = Vector2.One * 1024 * 2;

            Vector2 center = pos + (contentRegionMin + contentRegionMax) * 0.5f;

            float aspectGame = gameViewSize.X / gameViewSize.Y;
            float aspectContent = contentSize.X / contentSize.Y;

            Vector2 bgn, end;

            // 按照gameview的宽高比从新计算content的size
            float contentHeightNeo = contentSize.X / aspectGame;
            if (contentHeightNeo > contentSize.Y)
            {
                bgn.X = center.X - contentSize.Y * 0.5f;
                bgn.Y = pos.Y;

                end.X = bgn.X + contentSize.Y;
                end.Y = bgn.Y + contentSize.Y;
            }
            else
            {
                bgn.X = center.X - contentSize.X * 0.5f;
                bgn.Y = center.Y - contentHeightNeo * 0.5f;

                end.X = bgn.X + contentHeightNeo;
                end.Y = bgn.Y + contentHeightNeo;
            }

            ImGui.GetWindowDrawList().AddImage((IntPtr)tex.idx, bgn, end, new Vector2(0, 1), new Vector2(1, 0));
        }
    }
}
