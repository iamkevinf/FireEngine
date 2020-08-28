using System;
using System.Collections.Generic;
using System.Text;

namespace FireEditor
{
    public static class WindowManager
    {

        class WindowAttribute : iWindowAttribute
        {
            public iWindow window { get; set; }
            public uint UUID { get; set; }

            public bool visible { get; set; }

            public bool register2menu { get; set; }

            public bool canDock { get; set; }

            public string title { get; set; }
        }


        static Dictionary<uint, WindowAttribute> pool = new Dictionary<uint, WindowAttribute>();

        static uint s_uuid = 0;
        public static iWindowAttribute Add(string title, iWindow win,
            bool visible=true, bool canDock=true, bool register2menu=true)
        {
            uint uuid = s_uuid++;
            WindowAttribute attr = new WindowAttribute();
            attr.window = win;
            attr.UUID = uuid;
            attr.title = title;
            attr.visible = visible;
            attr.canDock = canDock;
            attr.register2menu = register2menu;
            pool.Add(uuid, attr);

            if (visible)
                ShowWindow(uuid);
            else
                CloseWindow(uuid);

            win.Init();

            return attr;
        }

        public static void Remove(uint uuid)
        {
            if (!pool.ContainsKey(uuid))
                throw new KeyNotFoundException(string.Format("{0} not found", uuid));

            CloseWindow(uuid);
            pool.Remove(uuid);
        }

        public static iWindowAttribute Get(uint uuid)
        {
            WindowAttribute attr;
            if (pool.TryGetValue(uuid, out attr))
            {
                return attr;
            }

            return null;
        }

        public static iWindowAttribute[] GetAllWindowAttributes()
        {
            List<iWindowAttribute> wins = new List<iWindowAttribute>();
            foreach (var w in pool.Values)
            {
                if (w.visible)
                    wins.Add(w);
            }
            return wins.ToArray();
        }


        public static void ShowWindow(uint uuid)
        {
            WindowAttribute attr;
            if(pool.TryGetValue(uuid, out attr))
            {
                attr.window.OnShow();
                attr.visible = true;
            }
        }

        public static void CloseWindow(uint uuid)
        {
            WindowAttribute attr;
            if (pool.TryGetValue(uuid, out attr))
            {
                attr.window.OnHide();
                attr.visible = false;
            }
        }
    }
}
