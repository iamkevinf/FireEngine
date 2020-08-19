﻿using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FireEngine.Editor
{
    class WindowConsole : WindowBase, iLogger
    {
        public override string title => "Console";
        public override bool isInWIndowList => true;
        public override bool canDock => true;

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


        public override void Init()
        {
            Show();
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
        public override void OnGUI()
        {
            base.OnGUI();

            if(ImGui.Button("Clear"))
            {
                logs.Clear();
            }

            int beginPosX = 30;
            int widthToggle = 32;
            int widthText = 10;
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
                ImGui.Text(node.str);
                ImGui.PopStyleColor();
            }


        }
    }
}
