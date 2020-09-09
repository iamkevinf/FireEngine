using FireEditor;
using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Text;

namespace FireEngine.Editor
{
    public class ImporterMaterial : iImporter
    {
        public string[] exts => new string[] {"mat"};

        public void Import(string fullname)
        {
            string metaPath = string.Format("{0}.meta", fullname);
            if (!System.IO.File.Exists(metaPath))
            {
                using (var fs = System.IO.File.OpenWrite(metaPath))
                {
                    fs.Close();
                }
            }
        }

        public void OnGUI()
        {
            ImGui.Text("这里处理一下额外的GUI渲染");
        }
    }
}
