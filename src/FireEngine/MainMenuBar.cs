using ImGuiNET;
using System;
using System.Numerics;

namespace FireEngine
{
    class MainMenuBar
    {
        public void OnInit()
        {
        }

        public void OnGui()
        {
            if (ImGui.BeginMainMenuBar())
            {
                if (ImGui.BeginMenu("File"))
                {
                    _ShowMenuFile();
                    ImGui.EndMenu();
                }
                if (ImGui.BeginMenu("Edit"))
                {
                    if (ImGui.MenuItem("Undo", "CTRL+Z")) { }
                    if (ImGui.MenuItem("Redo", "CTRL+Y", false, false)) { }  // Disabled item
                    ImGui.Separator();
                    if (ImGui.MenuItem("Cut", "CTRL+X")) { }
                    if (ImGui.MenuItem("Copy", "CTRL+C")) { }
                    if (ImGui.MenuItem("Paste", "CTRL+V")) { }
                    ImGui.EndMenu();
                }
                if (ImGui.BeginMenu("Window"))
                {
                    if (ImGui.MenuItem("Game", "CTRL+1"))
                    {
                    }

                    if (ImGui.MenuItem("bgfx", "CTRL+2"))
                    {
                    }
                    ImGui.EndMenu();
                }
                ImGui.EndMainMenuBar();
            }
        }

        public void OnExit()
        {
        }

        void _ShowMenuFile()
        {
            ImGui.MenuItem("(dummy menu)", null, false, false);
            if (ImGui.MenuItem("New")) { }
            if (ImGui.MenuItem("Open", "Ctrl+O")) { }
            if (ImGui.BeginMenu("Open Recent"))
            {
                ImGui.MenuItem("fish_hat.c");
                ImGui.MenuItem("fish_hat.inl");
                ImGui.MenuItem("fish_hat.h");
                if (ImGui.BeginMenu("More.."))
                {
                    ImGui.MenuItem("Hello");
                    ImGui.MenuItem("Sailor");
                    if (ImGui.BeginMenu("Recurse.."))
                    {
                        ImGui.EndMenu();
                    }
                    ImGui.EndMenu();
                }
                ImGui.EndMenu();
            }
            if (ImGui.MenuItem("Save", "Ctrl+S")) { }
            if (ImGui.MenuItem("Save As..")) { }

            ImGui.Separator();
            if (ImGui.BeginMenu("Options"))
            {
                bool enabled = true;
                ImGui.MenuItem("Enabled", "", ref enabled);
                ImGui.BeginChild("child", new Vector2(0, 60), true);
                for (int i = 0; i < 10; i++)
                    ImGui.Text($"Scrolling Text {i}");
                ImGui.EndChild();
                float f = 0.5f;
                int n = 0;
                ImGui.SliderFloat("Value", ref f, 0.0f, 1.0f);
                ImGui.InputFloat("Input", ref f, 0.1f);
                ImGui.Combo("Combo", ref n, "Yes\0No\0Maybe\0\0");
                ImGui.EndMenu();
            }
            if (ImGui.MenuItem("Quit", "Alt+F4")) { }
        }
    }
}
