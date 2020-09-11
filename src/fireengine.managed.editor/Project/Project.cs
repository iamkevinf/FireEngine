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

        FileSystemWatcher watcher;

        System.Collections.Concurrent.ConcurrentQueue<string> modifyFilePaths = new System.Collections.Concurrent.ConcurrentQueue<string>();
        System.Collections.Concurrent.ConcurrentQueue<string> deleteFilePaths = new System.Collections.Concurrent.ConcurrentQueue<string>();
        bool changed = false;
        public bool Changed
        {
            get
            {
                if(changed)
                {
                    changed = false;
                    return true;
                }

                return false;
            }
        }



        /// <summary>
        /// 遍历资源
        /// </summary>
        /// <param name="addToModifyQueue">是否加入到文件已经改变的列表</param>
        public void IterFile(bool addToModifyQueue = false)
        {
            string assetsPath = System.IO.Path.Combine(path, "Assets");
            DirectoryInfo info = new DirectoryInfo(assetsPath);
            CollectFile(info, ref root, addToModifyQueue);

            while(modifyFilePaths.TryDequeue(out string fullname))
                OnFileModify(fullname);

            while (deleteFilePaths.TryDequeue(out string fullname))
                OnFileDelete(fullname);

            if (addToModifyQueue)
                changed = true;
        }

        void OnFileModify(string fullname)
        {
            iImporter importer = ImporterManager.GetImporter(fullname);
            if (importer == null)
                return;

            ImporterManager.Import(importer, fullname);
        }

        void OnFileDelete(string fullname)
        {

        }

        void CollectFile(DirectoryInfo info, ref Path _path, bool addToModifyQueue)
        {
            var files = info.GetFiles();
            if (files.Length > 0)
            {
                List<File> tmp = new List<File>();
                for (int i = 0; i < files.Length; ++i)
                {
                    var ele = files[i];
                    if (ele.Extension.ToLower() == ".meta")
                        continue;
                    tmp.Add(new File() { name = ele.Name, fullname = ele.FullName});

                    if (addToModifyQueue)
                        modifyFilePaths.Enqueue(ele.FullName);
                }

                // file
                _path.files = tmp.ToArray();
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
                    CollectFile(ele, ref path, addToModifyQueue);
                }
            }
        }

        void _ImportAsset(string name)
        {

        }

        void WatchFile()
        {
            watcher = new FileSystemWatcher();
            watcher.Path = System.IO.Path.Combine(path, "Assets");
            watcher.IncludeSubdirectories = true;
            watcher.Changed += (s, e) =>
            {
                // 这里只处理文件不处理路径
                if (!Directory.Exists(e.FullPath))
                {
                    modifyFilePaths.Enqueue(e.FullPath);
                    changed = true;
                }

            };
            watcher.Created += (s, e) =>
            {
                modifyFilePaths.Enqueue(e.FullPath);
                changed = true;
            };
            watcher.Deleted += (s, e) =>
            {
                deleteFilePaths.Enqueue(e.FullPath);
                changed = true;
            };
            watcher.Renamed += (s, e) =>
            {
                deleteFilePaths.Enqueue(e.OldFullPath);
                modifyFilePaths.Enqueue(e.FullPath);
                changed = true;
            };
            watcher.EnableRaisingEvents = true;
        }

        public static ProjectCreateResult Create(string path)
        {
            if (Directory.GetDirectories(path).Length > 0 || Directory.GetFiles(path).Length > 0)
                return ProjectCreateResult.NotEmpty;

            if (!Directory.Exists(path))
                Directory.CreateDirectory(path);

            _CreateProjectFolder(path);

            Open(path);

            return ProjectCreateResult.OK;
        }

        public static void Open(string path)
        {
            string assetsPath = System.IO.Path.Combine(path, "Assets");
            if (!Directory.Exists(assetsPath))
            {
                Debug.LogErrorFormat("{0} 不是有效的工程路径", path);
                return;
            }

            _CreateProjectImmFolder(path);

            Project project = new Project();
            project.path = path;

            project.IterFile(true);
            project.WatchFile();

            current = project;
        }

        static void _CreateProjectFolder(string projectPath)
        {
            string assetsPath = System.IO.Path.Combine(projectPath, "Assets");
            if (!Directory.Exists(assetsPath))
                Directory.CreateDirectory(projectPath);

            string settingPath = System.IO.Path.Combine(projectPath, "ProjectSettings");
            if (!Directory.Exists(settingPath))
                Directory.CreateDirectory(settingPath);
        }

        static void _CreateProjectImmFolder(string projectPath)
        {
            string libraryPath = System.IO.Path.Combine(projectPath, "Library");
            if (!Directory.Exists(libraryPath))
                Directory.CreateDirectory(libraryPath);

            string tempPath = System.IO.Path.Combine(projectPath, "Temp");
            if (!Directory.Exists(tempPath))
                Directory.CreateDirectory(tempPath);
        }
    }
}
