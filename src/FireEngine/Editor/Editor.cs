using FireEditor;
using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FireEngine.Editor
{
    class Editor
    {
        static bool bLoading = true;

        static bool bShowDemo = false;
        static bool bShowState = false;
        static bool bShowFont = false;
        Menu menu = new Menu();

        public void Run()
        {
            AppNative.feApp_MainLoop("FireEngine", OnInit, OnGUI, OnTick, OnExit);
        }

        void OnInit()
        {
            Loading();

            _OnRegisterAsset();

            _OnCreateWindow();

            _OnCreateMenu();
        }

        private Queue<Action> mainthreadAction = new Queue<Action>();

        void Loading()
        {
            bLoading = true;
            System.Threading.ThreadPool.QueueUserWorkItem((e) =>
            {
                var io = ImGui.GetIO();
                ImFontPtr font = io.Fonts.AddFontFromFileTTF(
                                "Assets/fonts/AlibabaPH-Regular.otf", 16f, null, io.Fonts.GetGlyphRangesChineseFull());

                mainthreadAction.Enqueue(() =>
                {
                    var oldfont = io.Fonts;
                    io.SetFontDefault(font);

                    bLoading = false;
                });
            });
        }

        void _OnRegisterAsset()
        {
            ImporterManager.AddImporter(new ImporterMaterial());

            InspectorManager.RegisterInspector(new InspectorComponent());
        }

        iWindowAttribute attr_about = null;
        iWindowAttribute attr_debug = null;
        void _OnCreateWindow()
        {
            WindowManager.Add("Game", new WindowGameView());
            WindowManager.Add("Scene", new WindowSceneView());
            WindowManager.Add("Inspector", new WindowInspector());
            WindowManager.Add("Hierarchy", new WindowHierarchy());
            WindowManager.Add("Project", new WindowProject());
            var logger = WindowManager.Add("Console", new WindowConsole()).window as iLogger;
            Debug.SetLogger(logger);

            attr_about = WindowManager.Add("About", new WindowAbout(), false, false, false);
            attr_debug = WindowManager.Add("Debug", new WindowDebug(), false, false, false);
            WindowManager.GetAllWindowAttributes();
        }

        void _OnCreateMenu()
        {
            menu.CreateMenuGUI("File", OnMenuGUI_FileMenu);
            menu.CreateMenuGUI("Test/test secondary menu", OnMenuGUI_TestSecondaryMenu);
            menu.CreateMenuGUI("Windows/Views", OnMenuGUI_WindowsView);
            menu.CreateMenuEvent("Help/About", () =>
            {
                WindowManager.ShowWindow(attr_about.UUID);
            });
            menu.CreateMenuEvent("Help/Debug", () =>
            {
                WindowManager.ShowWindow(attr_debug.UUID);
            });

        }

        void OnGUI()
        {
            if (bLoading)
                DrawLoading();
            else
                DrawEditor();
        }

        void DrawLoading()
        {
            var viewport = ImGui.GetMainViewport();
            ImGui.SetNextWindowPos(viewport.Pos);
            ImGui.SetNextWindowSize(viewport.Size);
            ImGui.SetNextWindowViewport(viewport.ID);
            ImGui.PushStyleVar(ImGuiStyleVar.WindowRounding, 0.0f);
            ImGui.PushStyleVar(ImGuiStyleVar.WindowBorderSize, 0.0f);
            ImGui.PushStyleVar(ImGuiStyleVar.WindowPadding, System.Numerics.Vector2.Zero);
            ImGuiWindowFlags window_flags = ImGuiWindowFlags.NoDocking;
            window_flags |= ImGuiWindowFlags.NoTitleBar | ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove;
            window_flags |= ImGuiWindowFlags.NoBringToFrontOnFocus | ImGuiWindowFlags.NoNavFocus;

            if (ImGui.Begin("loading", window_flags))
            {

                ImGui.Text("loading...");

                ImGui.PopStyleVar();
                ImGui.PopStyleVar();
                ImGui.PopStyleVar();

                ImGui.End();
            }

        }

        void DrawEditor()
        {
            DrawDock();

            DrawWindows();

            //simple menu system
            if (ImGui.BeginMainMenuBar())
            {
                menu.OnGUI();

                //this is for demo
                if (ImGui.BeginMenu("Demo"))
                {
                    ImGui.MenuItem("show demo", "", ref bShowDemo);
                    ImGui.MenuItem("show state", "", ref bShowState);
                    ImGui.MenuItem("show font", "", ref bShowFont);
                    ImGui.EndMenu();
                }
                ImGui.EndMainMenuBar();
            }

            if (bShowDemo)
            {
                ImGui.ShowDemoWindow();
            }

            if (bShowState)
            {
                AppNative.feApp_ShowState();
            }

            if (bShowFont)
            {
                ImGui.ShowFontSelector("font selector");
            }

        }

        void OnTick()
        {
            foreach (var win in WindowManager.GetAllWindowAttributes())
            {
                win.window.OnTick();
            }

            while (mainthreadAction.Count > 0)
                mainthreadAction.Dequeue()();
        }

        void OnExit()
        {
            foreach (var win in WindowManager.GetAllWindowAttributes())
            {
                win.window.OnDestroy();
            }
        }

        void FileCreateProject()
        {
            string path = "";

            string curPath = System.IO.Directory.GetCurrentDirectory();

            var rt = nfdnative.NFD_PickFolder(curPath, ref path);
            if (rt == nfdnative.nfdresult.NFD_OKAY)
            {
                ProjectNative.SetProjectPath(path);
                var ret = Project.Create(path);
                if (ret == Project.ProjectCreateResult.NotEmpty)
                {
                    Debug.LogError("directory is not empty");
                    return;
                }

            }

        }

        void FileOpenProject()
        {
            string path = "";

            string curPath = System.IO.Directory.GetCurrentDirectory();

            var rt = nfdnative.NFD_PickFolder(curPath, ref path);
            if (rt == nfdnative.nfdresult.NFD_OKAY)
            {
                ProjectNative.SetProjectPath(path);
                try
                {
                    Project.Open(path);
                }
                catch (Exception e)
                {
                    Debug.LogErrorFormat("打开项目失败 {0}", e.ToString());
                    return;
                }

            }
        }

        void OnMenuGUI_FileMenu()
        {
            ImGui.Separator();
            if (ImGui.MenuItem("CreateProject"))
            {
                FileCreateProject();
            }
            else if (ImGui.MenuItem("OpenProject"))
            {
                FileOpenProject();
            }
            ImGui.Separator();
        }

        void OnMenuGUI_TestSecondaryMenu()
        {
            ImGui.MenuItem("SecondaryMenu A");
            ImGui.MenuItem("SecondaryMenu B");
        }

        void OnMenuGUI_WindowsView()
        {
            foreach (var attr in WindowManager.GetAllWindowAttributes())
            {
                iWindow window = attr.window;
                bool visible = attr.visible;
                if (attr.register2menu && ImGui.MenuItem(attr.title, "", ref visible))
                {
                    if (visible != attr.visible)
                    {
                        if (visible)
                            WindowManager.ShowWindow(attr.UUID);
                        else
                            WindowManager.CloseWindow(attr.UUID);
                    }
                }
            }
        }

        void DrawDock()
        {
            var viewport = ImGui.GetMainViewport();
            ImGui.SetNextWindowPos(viewport.Pos);
            ImGui.SetNextWindowSize(viewport.Size);
            ImGui.SetNextWindowViewport(viewport.ID);
            ImGui.PushStyleVar(ImGuiStyleVar.WindowRounding, 0.0f);
            ImGui.PushStyleVar(ImGuiStyleVar.WindowBorderSize, 0.0f);
            ImGui.PushStyleVar(ImGuiStyleVar.WindowPadding, Vector2.Zero);
            ImGuiWindowFlags window_flags = ImGuiWindowFlags.MenuBar | ImGuiWindowFlags.NoDocking;
            window_flags |= ImGuiWindowFlags.NoTitleBar | ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove;
            window_flags |= ImGuiWindowFlags.NoBringToFrontOnFocus | ImGuiWindowFlags.NoNavFocus;

            if (ImGui.Begin("DockWindow", window_flags))
            {
                uint rootdockid = ImGui.GetID("rootdock");
                ImGui.DockSpace(rootdockid, Vector2.Zero, ImGuiDockNodeFlags.PassthruCentralNode);
                ImGui.PopStyleVar();
                ImGui.PopStyleVar();
                ImGui.PopStyleVar();

                ImGui.End();
            }

        }

        void DrawWindows()
        {
            foreach (var win in WindowManager.GetAllWindowAttributes())
            {
                if (win.visible)
                {
                    bool open = true;
                    ImGuiWindowFlags flag = ImGuiWindowFlags.NoCollapse;
                    if (!win.canDock) flag |= ImGuiWindowFlags.NoDocking;
                    ImGui.SetNextWindowSizeConstraints(Vector2.One * 100, Vector2.One * 10240);
                    if (ImGui.Begin(win.title, ref open, flag))
                    {
                        if (open == false)
                        {
                            WindowManager.CloseWindow(win.UUID);
                            ImGui.End();
                            continue;
                        }
                        win.window.OnGUI();
                    }
                    ImGui.End();
                }
            }
        }
    }
}
