using FireEngine;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace FireEditor
{
    public class SceneNode : iSelectable
    {
        public string name
        {
            get;
            set;
        }

        public Component component;
        public IInspector inspector;

        public TransformNative.TransformHandle handle;
        private List<SceneNode> m_children = new List<SceneNode>();
        public List<SceneNode> children
        {
            get => m_children;
            private set => m_children = value;
        }
        public SceneNode parent = null;

        public void AddChild(SceneNode node)
        {
            if (children.Contains(node))
                return;

            children.Add(node);
            node.parent = this;
        }

        public void Move(SceneNode scene)
        {
            if (parent == scene)
                return;

            parent.children.Remove(this);
            parent = scene;
            parent.children.Add(this);
        }
    }

    public class Scene : iSelectable
    {
        public string name
        {
            get;
            set;
        }

        public SceneNode root = new SceneNode();
        public static Scene current
        {
            get;
            private set;
        }

        public SceneNative.SceneHandle handle;

        public static void Open(SceneNative.SceneHandle handle)
        {
            if (Scene.current == null)
                Scene.current = new Scene();
            
            var root = SceneNative.SceneGetRoot(handle);

            Scene.current.root.handle = root;
            Scene.current.handle = handle;
        }

        public static Scene GetScene(SceneNative.SceneHandle handle)
        {
            if (Scene.current.handle == handle)
                return Scene.current;

            return null;
        }

        public static SceneNode GetSceneNode(TransformNative.TransformHandle handle)
        {
            return _GetSceneNode(Scene.current.root, handle);
        }

        public static SceneNode _GetSceneNode(SceneNode node, TransformNative.TransformHandle handle)
        {
            if (node.handle == handle)
                return node;

            foreach(SceneNode child in node.children)
            {
                SceneNode ret = _GetSceneNode(child, handle);
                if (ret != null)
                    return ret;
            }

            return null;
        }
    }
}
