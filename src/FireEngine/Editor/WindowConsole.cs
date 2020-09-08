using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;
using FireEditor;

namespace FireEngine.Editor
{
    class WindowConsole : iWindow, iLogger
    {
        enum LogLevel
        {
            Log,
            Warning,
            Error
        }

        struct LogNode
        {
            public LogLevel level;
            public string str;
        }

        List<LogNode> logs = new List<LogNode>();


        public void Init()
        {
            unsafe
            {
                FireEngineNative.SetIMAssertHacker(OnIMGUIAsset);
                FireEngineNative.SetOnEngineLogHacker(OnLogEngine);
            }
        }

        unsafe void OnIMGUIAsset(char* expr, char* filename, int line)
        {
            LogNode node;
            node.level = LogLevel.Log;
            node.str = string.Format("IMGuiLog: (file:{0}, line:{1}): {2}",
                new string(filename), line, new string(expr));

            logs.Add(node);
        }
        unsafe void OnLogEngine(char* expr, char* filename, int line, int level)
        {
            LogNode node;
            node.level = (LogLevel)level;
            node.str = string.Format("(file:{0}, line:{1}): {2}",
                new string(filename), line, new string(expr));

            logs.Add(node);
        }


        public void Log(string str)
        {
            LogNode node;
            node.level = LogLevel.Log;
            node.str = str;
            logs.Add(node);
        }

        public void LogFormat(string fmt, params object[] paramList)
        {
            LogNode node;
            node.level = LogLevel.Log;
            node.str = string.Format(fmt, paramList);
            logs.Add(node);
        }

        public void LogWarning(string str)
        {
            LogNode node;
            node.level = LogLevel.Warning;
            node.str = str;
            logs.Add(node);
        }

        public void LogWarningFormat(string fmt, params object[] paramList)
        {
            LogNode node;
            node.level = LogLevel.Warning;
            node.str = string.Format(fmt, paramList);
            logs.Add(node);
        }

        public void LogError(string str)
        {
            LogNode node;
            node.level = LogLevel.Error;
            node.str = str;
            logs.Add(node);
        }

        public void LogErrorFormat(string fmt, params object[] paramList)
        {
            LogNode node;
            node.level = LogLevel.Error;
            node.str = string.Format(fmt, paramList);
            logs.Add(node);
        }

        bool bEnableT = true, bEnableW = true, bEnableE = true;
        Vector2 pos = Vector2.Zero;
        public void OnGUI()
        {
            if (ImGui.Button("Clear"))
            {
                logs.Clear();
            }

            int beginPosX = 30;
            float widthToggle = 32 + ImGui.CalcTextSize("E").X;
            float widthText = 10 + ImGui.CalcTextSize("E").X;

            ImGui.SameLine(ImGui.GetWindowContentRegionMax().X - beginPosX);
            ImGui.ToggleButton("E", ref bEnableE);
            ImGui.SameLine(ImGui.GetWindowContentRegionMax().X - beginPosX - widthText);
            ImGui.Text("E");

            ImGui.SameLine(ImGui.GetWindowContentRegionMax().X - beginPosX - widthText - widthToggle);
            ImGui.ToggleButton("W", ref bEnableW);
            ImGui.SameLine(ImGui.GetWindowContentRegionMax().X - beginPosX - widthText * 2 - widthToggle);
            ImGui.Text("W");

            ImGui.SameLine(ImGui.GetWindowContentRegionMax().X - beginPosX - widthText * 2 - widthToggle * 2);
            ImGui.ToggleButton("T", ref bEnableT);
            ImGui.SameLine(ImGui.GetWindowContentRegionMax().X - beginPosX - widthText * 3 - widthToggle * 2);
            ImGui.Text("T");
            float footer_height_to_reserve = ImGui.GetStyle().ItemSpacing.Y + ImGui.GetFrameHeightWithSpacing();
            pos.Y = -footer_height_to_reserve;
            if (ImGui.BeginChild("LogChildForm", pos, false, ImGuiWindowFlags.HorizontalScrollbar))
            {
                for (int i = 0; i < logs.Count; ++i)
                {
                    LogNode node = logs[i];

                    if (!bEnableT && node.level == LogLevel.Log)
                        continue;
                    if (!bEnableW && node.level == LogLevel.Warning)
                        continue;
                    if (!bEnableE && node.level == LogLevel.Error)
                        continue;

                    Vector4 color = System.Numerics.Vector4.One;
                    switch (node.level)
                    {
                        case LogLevel.Warning:
                            color.X = 1;
                            color.Y = 1;
                            color.Z = 0;
                            color.W = 1;
                            break;
                        case LogLevel.Error:
                            color.X = 1;
                            color.Y = 0;
                            color.Z = 0;
                            color.W = 1;
                            break;

                        default:
                            color = System.Numerics.Vector4.One;
                            break;
                    }


                    ImGui.PushStyleColor(ImGuiCol.Text, color);
                    ImGui.TextWrapped(node.str);
                    ImGui.Spacing();
                    ImGui.PopStyleColor();
                }

            }

            if (ImGui.GetScrollY() >= ImGui.GetScrollMaxY())
                ImGui.SetScrollHereY(1.0f);

            ImGui.EndChild();
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
    }
}
