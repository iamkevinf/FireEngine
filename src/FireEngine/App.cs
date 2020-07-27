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

        bool v1 = false, v2 = false;
        int radio_index = 0;

        IntPtr m_tid_headerPlayNor = IntPtr.Zero;
        IntPtr m_tid_headerPlayDwn = IntPtr.Zero;
        Vector2 m_headerPlaySize;

        bool m_activePlay = false;
        bool m_activePause = false;
        bool m_activeStop = false;

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

            IntPtr intPtr = (IntPtr)textureHandle1.idx;

            ImGui.Image(intPtr, m_texture_size);

            //ImGui.ImageButton(m_texture_id2, m_texture_size);
            //ImGui.PushID(1);
            //ImGui.ImageAnimButton(m_texture_id, m_texture_id2, m_texture_size);
            //ImGui.PopID();

            //ImGui.PushID(2);
            //ImGui.ToggleButton(m_texture_id, m_texture_id2, m_texture_size, ref v2);
            //ImGui.PopID();
            //ImGui.ToggleButton("1", ref v1);

            //ImGui.PushID(3);
            //ImGui.ImageRadioButtonIntPtr(m_texture_id, m_texture_id2, m_texture_size, ref radio_index, 0); ImGui.SameLine();
            //ImGui.PopID();
            //ImGui.PushID(4);
            //ImGui.ImageRadioButtonIntPtr(m_texture_id, m_texture_id2, m_texture_size, ref radio_index, 1); ImGui.SameLine();
            //ImGui.PopID();
            //ImGui.PushID(5);
            //ImGui.ImageRadioButtonIntPtr(m_texture_id, m_texture_id2, m_texture_size, ref radio_index, 2);
            //ImGui.PopID();

            ImGui.ShowDemoWindow();
        }

        void OnExit()
        {
            mainMenuBar.OnExit();
        }

        void InitHeader()
        {
            m_tid_headerPlayNor = ImGui.LoadTexture("icon_play_nor.png", out m_headerPlaySize);
            m_tid_headerPlayDwn = ImGui.LoadTexture("icon_play_dwn.png", out m_headerPlaySize);
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
                ImGui.Text("Left 1"); ImGui.SameLine();
                ImGui.Text("Left 2"); ImGui.SameLine();

                //ImGui.SetCursorPosX((ImGui.GetWindowSize().X - m_headerPlaySize.X) * 0.5f);
                ImGui.SetCursorPosX((ImGui.GetWindowSize().X - m_headerPlaySize.X * 3) * 0.5f);
                ImGui.BeginChild("Play&Pause&Stop", new Vector2(m_headerPlaySize.X * 3, 40));
                {
                    ImGui.PushID(1);
                    ImGui.ToggleButton(m_tid_headerPlayNor, m_tid_headerPlayDwn, m_headerPlaySize, ref m_activePlay); ImGui.SameLine(0.0f, 0.0f);
                    ImGui.PopID();
                    ImGui.PushID(2);
                    ImGui.ToggleButton(m_tid_headerPlayNor, m_tid_headerPlayDwn, m_headerPlaySize, ref m_activePause); ImGui.SameLine(0.0f, 0.0f);
                    ImGui.PopID();
                    ImGui.PushID(3);
                    ImGui.ToggleButton(m_tid_headerPlayNor, m_tid_headerPlayDwn, m_headerPlaySize, ref m_activeStop);
                    ImGui.PopID();
                }
                ImGui.EndChild();
                //ImGui.ImageAnimButton(m_tid_headerPlayNor, m_tid_headerPlayDwn, m_headerPlaySize);
                ImGui.End();
            }
        }

    }
}
