using FireEditor;
using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FireEngine.Editor
{

    class ShaderNameNode
    {
        public string name = "";
        public ShaderNameNode parent;
        public ShaderNameNode child;

        public string fullname
        {
            get
            {
                ShaderNameNode tmp = this;
                tmp.child = null;
                string _fullname = "";
                while (tmp.parent != null)
                    tmp = tmp.parent;

                while (tmp.child != null)
                {
                    if (tmp.name != null && tmp.name != "")
                    {
                        _fullname += tmp.name;
                        _fullname += "/";

                        tmp = tmp.child;
                    }
                    else
                    {
                        tmp = tmp.child;
                    }
                }

                if (tmp.name != null && tmp.name != "")
                    _fullname = _fullname + tmp.name;

                return _fullname;
            }
        }
    }


    public class ImporterMaterial : iImporter
    {
        public string[] exts => new string[] { "mat" };

        public bool HasGUI => true;

        Material curMat = null;
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

            var content = System.IO.File.ReadAllBytes(fullname);
            // 这里临时存的是string
            string txt = Encoding.UTF8.GetString(content, 0, content.Length);
            int idx = txt.IndexOf('\r');
            if (idx >= 0)
                txt = txt.Substring(0, idx);
            Shader shader = Shader.Find(txt);
            curMat = new Material(shader);

            AssetManager.AddOrUpAsset(fullname, curMat);
        }

        public void OnGUI()
        {
            OnGUI(curMat);
        }

        public static int mat_count = 0;

        static void OnGUI_SelectShader(Material mat)
        {
            string shaderName = mat.shader.name;
            int idx = shaderName.LastIndexOf('/');
            if (idx >= 0)
                shaderName = shaderName.Substring(idx + 1);

            string nameList = ShaderNative.ShaderGetNameList();
            string[] arr = nameList.Split(',');

            List<ShaderNameNode> nameNode = new List<ShaderNameNode>();
            if (ImGui.BeginCombo($"Shader##Combo#ShaderName#ImporterMaterial#{mat_count++}", shaderName))
            {
                for (int i = 0; i < arr.Length; ++i)
                {
                    string ele = arr[i];
                    ShaderNameNode root = new ShaderNameNode();
                    DeepSplitName(ref root, ele);
                    nameNode.Add(root);
                }

                foreach (var node in nameNode)
                {
                    IterNode(node, mat.shader.name, ref mat);
                }

                ImGui.EndCombo();
            }
        }

        public static void OnGUI(Material mat)
        {
            if (!mat)
                return;

            if (!mat.shader)
                return;


            if (ImGui.TreeNodeEx($"{mat.name}##DragFloat4#SImporterMaterial#OnGUI#{mat_count}", ImGuiTreeNodeFlags.None))
            {
                OnGUI_SelectShader(mat);

                int propertiesCount = MaterialNative.MaterialGetPropertyVec4Count(mat.native);
                for (int i = 0; i < propertiesCount; ++i)
                {
                    IntPtr n = MaterialNative.MaterialGetPropertyVec4(mat.native, i);
                    string name = MaterialNative.MaterialGetPropertyVec4NameStr(n);
                    string desc = MaterialNative.MaterialGetPropertyVec4DescStr(n);
                    Vector4 value = MaterialNative.MaterialGetPropertyVec4Value(n);
                    if (ImGui.DragFloat4($"{desc}##DragFloat4#SImporterMaterial#OnGUI#{mat_count}", ref value))
                    {
                        mat.SetVector(name, value);
                    }

                }
                ImGui.TreePop();
            }
        }


        static void DeepSplitName(ref ShaderNameNode parent, string name)
        {
            int idx = name.IndexOf('/');
            if (idx < 0)
            {
                ShaderNameNode node = new ShaderNameNode();
                node.name = name;
                node.parent = parent;
                parent.child = node;
            }
            else
            {
                string h = name.Substring(0, idx);
                string t = name.Substring(idx + 1);
                ShaderNameNode node = new ShaderNameNode();
                node.name = h;
                node.parent = parent;
                parent.child = node;
                DeepSplitName(ref node, t);
            }
        }

        static void IterNode(ShaderNameNode node, string oriName, ref Material curMat)
        {
            if (node.name == null || node.name == "")
            {
                if (node.child != null)
                    IterNode(node.child, oriName, ref curMat);
            }
            else
            {
                if (node.child == null)
                {
                    if (ImGui.MenuItem(node.name, "", node.fullname == oriName))
                        OnSelectShader(node, oriName, ref curMat);
                }
                else
                {
                    if (ImGui.BeginMenu(node.name))
                    {
                        IterNode(node.child, oriName, ref curMat);
                        ImGui.EndMenu();
                    }
                }
            }
        }

        static void OnSelectShader(ShaderNameNode node, string oriName, ref Material curMat)
        {
            if (node.fullname != oriName)
            {
                curMat.shader = Shader.Find(node.fullname);
                Debug.LogFormat("切换了shader {0}", node.fullname);
            }
        }
    }
}
