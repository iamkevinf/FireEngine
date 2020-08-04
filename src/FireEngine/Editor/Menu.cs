using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Text;

namespace FireEngine.Editor
{
    public class MenuInfo
    {
        public Action onMenuEvent;
        public Action onMenuGUI;
        public Dictionary<string, MenuInfo> menus;
        public void OnGUI(string title)
        {
            if (onMenuEvent != null)
            {
                if (ImGui.MenuItem(title))
                {
                    onMenuEvent();
                }
            }
            else if (onMenuGUI != null)
            {
                if (ImGui.BeginMenu(title))
                {
                    onMenuGUI();

                    ImGui.EndMenu();
                }
            }
            else if (menus != null)
            {
                if (ImGui.BeginMenu(title))
                {
                    foreach (var m in this.menus)
                    {
                        m.Value.OnGUI(m.Key);
                    }
                    ImGui.EndMenu();
                }
            }
            else
            {

            }
        }


        public void CreteMenuEvent(string[] paths, int pathindex, Action onMenuEvent)
        {
            if (pathindex == paths.Length)
            {
                this.onMenuEvent = onMenuEvent;
            }
            else
            {
                var curkey = paths[pathindex];
                if (menus == null)
                    menus = new Dictionary<string, MenuInfo>();
                if (!menus.ContainsKey(curkey))
                {
                    menus[curkey] = new MenuInfo();
                }
                menus[curkey].CreteMenuEvent(paths, pathindex + 1, onMenuEvent);
            }
        }
        public void CreteMenuGUI(string[] paths, int pathindex, Action onMenuGUI)
        {
            if (pathindex == paths.Length)
            {
                this.onMenuGUI = onMenuGUI;
            }
            else
            {
                var curkey = paths[pathindex];
                if (menus == null)
                    menus = new Dictionary<string, MenuInfo>();
                if (!menus.ContainsKey(curkey))
                {
                    menus[curkey] = new MenuInfo();
                }
                menus[curkey].CreteMenuGUI(paths, pathindex + 1, onMenuGUI);
            }
        }

    }

    public class Menu
    {
        Dictionary<string, MenuInfo> menus = new Dictionary<string, MenuInfo>();
        public void CreateMenuEvent(string path, Action onMenuEvent)
        {
            var paths = path.Split('/');
            if (!menus.ContainsKey(paths[0]))
            {
                menus[paths[0]] = new MenuInfo();
            }
            menus[paths[0]].CreteMenuEvent(paths, 1, onMenuEvent);
        }

        public void CreateMenuGUI(string path, Action onMenuGUI)
        {
            var paths = path.Split('/');
            if (!menus.ContainsKey(paths[0]))
            {
                menus[paths[0]] = new MenuInfo();
            }
            menus[paths[0]].CreteMenuGUI(paths, 1, onMenuGUI);

        }


        public void OnGUI()
        {
            foreach (var m in menus)
            {
                m.Value.OnGUI(m.Key);
            }
        }

    }
}
