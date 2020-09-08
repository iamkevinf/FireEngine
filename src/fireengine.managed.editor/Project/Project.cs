using FireEngine;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace FireEditor
{
    public class Project
    {
        public enum ProjectCreateResult
        {
            OK,
            NotEmpty
        }
        public string path;
        public Path root = new Path();
        public static Project current
        {
            get;
            private set;
        }

        void IterFile()
        {
            DirectoryInfo info = new DirectoryInfo(path);
            CollectFile(info, ref root);
        }

        void CollectFile(DirectoryInfo info, ref Path _path)
        {
            var files = info.GetFiles();
            if (files.Length > 0)
            {
                _path.files = new File[files.Length];
                // file
                for (int i = 0; i < files.Length; ++i)
                {
                    var ele = files[i];
                    if (ele.Extension.ToLower() == ".meta")
                        continue;

                    File file = new File();
                    file.name = ele.Name;
                    _path.files[i] = file;

                    string meta = string.Format("{0}.meta", ele.Name);
                    if (!System.IO.File.Exists(meta))
                    {
                        using (var fs = System.IO.File.OpenWrite(meta))
                        {
                            fs.Close();
                        }
                    }
                }
            }

            var dirs = info.GetDirectories();
            if (dirs.Length > 0)
            {
                _path.paths = new Path[dirs.Length];
                // dir
                for (int i = 0; i < dirs.Length; ++i)
                {
                    var ele = dirs[i];
                    Path path = new Path();
                    path.name = ele.Name;
                    _path.paths[i] = path;
                    CollectFile(ele, ref path);
                }
            }
        }

        public static ProjectCreateResult Create(string path)
        {
            if (Directory.GetDirectories(path).Length > 0 || Directory.GetFiles(path).Length > 0)
                return ProjectCreateResult.NotEmpty;

            if (!Directory.Exists(path))
                Directory.CreateDirectory(path);

            string assetsPath = string.Format("{0}\\Assets", path);
            if (!Directory.Exists(assetsPath))
                Directory.CreateDirectory(assetsPath);

            Open(path);

            return ProjectCreateResult.OK;
        }

        public static void Open(string path)
        {
            string assetsPath = string.Format("{0}\\Assets", path);
            if (!Directory.Exists(assetsPath))
            {
                Debug.LogErrorFormat("{0} 不是有效的工程路径", path);
                return;
            }

            Project project = new Project();
            project.path = assetsPath;

            project.IterFile();

            current = project;
        }
    }
}
