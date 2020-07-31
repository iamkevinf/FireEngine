using ImGuiNET;
using RenderCoreNet;
using System;
using System.Numerics;

namespace FireEngine
{
    class App
    {
        MainMenuBar mainMenuBar = new MainMenuBar();
        Vector2 m_texture_size;
        TextureHandle textureHandle1;

        public void Run()
        {
            ImGui.App_MainLoop("FireEngine NetCore", OnInit, OnGui, OnExit);
        }

        void OnInit()
        {
            textureHandle1 = RC.LoadTexture("images/test_texture.png", out m_texture_size);
            mainMenuBar.OnInit();
            InitHeader();
        }

        void OnGui()
        {
            mainMenuBar.OnGui();

            DrawHeader();

            ImGui.Image((IntPtr)textureHandle1.idx, m_texture_size);
            ImGui.ShowDemoWindow();
        }

        void OnExit()
        {
            mainMenuBar.OnExit();

            RC.DestroyCube(cubeHandle);
        }

        int cubeHandle = -1;
        void InitHeader()
        {
            cubeHandle = RC.CreateCube( );
        }

        void DrawHeader()
        {
            ImGuiIOPtr io = ImGui.GetIO();
            ImGuiViewportPtr viewport = ImGui.GetMainViewport();
            Vector2 work_area_pos = viewport.GetWorkPos();
            Vector2 work_area_size = viewport.GetWorkSize();
            Vector2 window_pos = work_area_pos;
            Vector2 window_pos_pivot = Vector2.Zero;
            ImGui.SetNextWindowPos(window_pos, ImGuiCond.Always, window_pos_pivot);
            work_area_size.Y = 40;
            ImGui.SetNextWindowSize(work_area_size);
            ImGui.SetNextWindowViewport(viewport.ID);

            ImGuiWindowFlags windowFlags = ImGuiWindowFlags.NoDecoration | ImGuiWindowFlags.NoDocking | ImGuiWindowFlags.NoSavedSettings | ImGuiWindowFlags.NoFocusOnAppearing | ImGuiWindowFlags.NoNav | ImGuiWindowFlags.NoMove;
            bool open = true;
            if (ImGui.Begin("HeaderBar", ref open, windowFlags))
            {
                ImGui.BeginChild("header");

                    ImGui.Text("Left 1"); ImGui.SameLine();
                    ImGui.Text("Left 2"); ImGui.SameLine();

                ImGui.EndChild();

                ImGui.End();
            }

            RC.DrawCube(9, cubeHandle);
        }

    }
}
