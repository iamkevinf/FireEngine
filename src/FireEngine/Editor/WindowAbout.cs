using ImGuiNET;
using FireEditor;

namespace FireEngine.Editor
{
    class WindowAbout : iWindow
    {
        public void Init()
        {
        }

        public void OnGUI()
        {
            var fonts = ImGui.GetIO().Fonts.Fonts;
            for (var i = 0; i < fonts.Size; i++)
            {
                var font = fonts[i];
                ImGui.PushFont(font);//临时换字体
                ImFontGlyphPtr fontGlyphPtr = font.FindGlyphNoFallback('中');
                unsafe
                {
                    if (fontGlyphPtr.NativePtr != null)
                        ImGui.Text("这是一个中文 \"关于\" 窗口。");
                    else
                        ImGui.Text("this is a english \"about\" window.");
                }
                ImGui.PopFont();
            }
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

        public void OnDestroy()
        {

        }
    }
}
