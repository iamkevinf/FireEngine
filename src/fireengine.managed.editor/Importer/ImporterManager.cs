using System;
using System.Collections.Generic;
using System.Text;

namespace FireEditor
{
    public interface iImporter
    {
        /// <summary>
        /// 导入器需要处理的所有后缀名
        /// </summary>
        string[] exts { get; }

        void Import(string fullname);

        void OnGUI();
    }

    public static class ImporterManager
    {
        public static Dictionary<string, iImporter> pool = new Dictionary<string, iImporter>();

        public static void AddImporter(iImporter importer)
        {
            if (importer.exts == null)
                return;

            foreach(string ext in importer.exts)
            {
                if (pool.ContainsKey(ext))
                    continue;

                pool.Add(ext, importer);
            }
        }

        public static iImporter GetImporter(string filename)
        {
            int idx = filename.LastIndexOf('.');
            if (idx < 0)
                return null;

            string ext = filename.Substring(idx + 1).ToLower();

            if (pool.ContainsKey(ext))
                return pool[ext];

            return null;

        }

        public static void Import(iImporter importer, string fullname)
        {
            importer.Import(fullname);
        }
    }
}
