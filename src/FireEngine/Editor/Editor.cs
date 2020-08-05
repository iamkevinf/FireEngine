using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Text;

namespace FireEngine.Editor
{
    class Editor
    {
        static bool bShowDemo = false;
        static bool bShowState = false;
        Dictionary<Type, iWindow> windows = new Dictionary<Type, iWindow>();
        Menu menu = new Menu();

        public void Run()
        {
            AppNative.feApp_MainLoop("FireEngine DotNetCore", OnInit, OnGUI, OnExit);
        }

        void OnInit()
        {
            ImGuiIOPtr io = ImGui.GetIO();
            ImFontPtr font = io.Fonts.AddFontFromFileTTF(
                "fonts/AlibabaPH-Regular.otf", 14f, null, io.Fonts.GetGlyphRangesChineseFull());

            menu.CreateMenuGUI("Test/test secondary menu", OnMenuGUI_TestSecondaryMenu);

            menu.CreateMenuEvent("Help/About", () =>
            {
                CreateWindow(typeof(WindowAbout)).Show();
            });
        }

        void OnGUI()
        {
            DrawDock();

            //simple window system
            foreach (var win in windows.Values)
            {
                if (win.visible)
                {
                    bool open = true;
                    ImGuiWindowFlags flag = ImGuiWindowFlags.NoCollapse;
                    if (!win.candock) flag |= ImGuiWindowFlags.NoDocking;
                    if (ImGui.Begin(win.title, ref open, flag))
                    {
                        if (open == false)
                        {
                            win.Hide();
                            ImGui.End();
                            continue;
                        }
                        win.OnGUI();
                        ImGui.End();
                    }
                }
            }

            //simple menu system
            if (ImGui.BeginMainMenuBar())
            {
                menu.OnGUI();

                //this is for demo
                if (ImGui.BeginMenu("Demo"))
                {
                    ImGui.MenuItem("show demo", "", ref bShowDemo);
                    ImGui.MenuItem("show state", "", ref bShowState);
                    ImGui.EndMenu();
                }
                ImGui.EndMainMenuBar();
            }

            if (bShowDemo)
            {
                ImGui.ShowDemoWindow();
            }

            if(bShowState)
            {
                AppNative.feApp_ShowState();
            }

        }

        void OnExit()
        {

        }

        void OnMenuGUI_TestSecondaryMenu()
        {
            ImGui.MenuItem("SecondaryMenu A");
            ImGui.MenuItem("SecondaryMenu B");
        }


        void DrawDock()
        {
            var viewport = ImGui.GetMainViewport();
            ImGui.SetNextWindowPos(viewport.Pos);
            ImGui.SetNextWindowSize(viewport.Size);
            ImGui.SetNextWindowViewport(viewport.ID);
            ImGui.PushStyleVar(ImGuiStyleVar.WindowRounding, 0.0f);
            ImGui.PushStyleVar(ImGuiStyleVar.WindowBorderSize, 0.0f);
            ImGui.PushStyleVar(ImGuiStyleVar.WindowPadding, System.Numerics.Vector2.Zero);
            ImGuiWindowFlags window_flags = ImGuiWindowFlags.MenuBar | ImGuiWindowFlags.NoDocking;
            window_flags |= ImGuiWindowFlags.NoTitleBar | ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove;
            window_flags |= ImGuiWindowFlags.NoBringToFrontOnFocus | ImGuiWindowFlags.NoNavFocus;


            if (ImGui.Begin("DockWindow", window_flags))
            {
                uint rootdockid = ImGui.GetID("rootdock");
                ImGui.DockSpace(rootdockid, System.Numerics.Vector2.Zero, ImGuiDockNodeFlags.PassthruCentralNode);
                ImGui.PopStyleVar();
                ImGui.PopStyleVar();
                ImGui.PopStyleVar();

                ImGui.End();
            }

        }

        public iWindow CreateWindow(Type type)
        {
            if (windows.TryGetValue(type, out iWindow win))
                return win;

            if (type.GetInterface(typeof(iWindow).FullName) == null)
                return null;

            var ctor = type.GetConstructor(new Type[0]);

            var nwin = ctor.Invoke(null) as iWindow;
            windows[type] = nwin;
            return nwin;
        }

    }
}
