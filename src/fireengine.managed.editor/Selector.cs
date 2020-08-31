using System;
using System.Collections.Generic;
using System.Text;

namespace FireEditor
{
    public interface iSelectable
    {
        string name
        {
            get;
        }
    }

    public static class Selector
    {
        static List<iSelectable> pool = new List<iSelectable>();

        public static void SelectAppend(iSelectable obj)
        {
            pool.Add(obj);
        }

        public static void Select(iSelectable obj)
        {
            Clear();

            SelectAppend(obj);
        }

        public static void Clear()
        {
            pool.Clear();
        }

        public static bool IsSelect(iSelectable obj)
        {
            return pool.Contains(obj);
        }

        public static IList<iSelectable> GetSelected()
        {
            return pool;
        }
    }
}
