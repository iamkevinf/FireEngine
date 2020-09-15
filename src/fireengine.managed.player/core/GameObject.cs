using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using static FireEngine.GameObjectNative;

namespace FireEngine
{
    public class GameObject
    {
        #region Inner

        private IntPtr m_nativePtr = IntPtr.Zero;
        private GameObject(IntPtr native)
        {
            m_nativePtr = native;
            IsDestroy = false;
        }
        ~GameObject()
        {
            if (m_nativePtr != IntPtr.Zero)
            {
                GameObject.Destroy(this);
                m_nativePtr = IntPtr.Zero;
            }
        }


        #endregion
        public bool IsDestroy
        {
            get;
            private set;
        }

        public static GameObject Inner_Create(TransformNative.TransformHandle parent, string name)
        {
            return null;
        }

        public static GameObject CreateFromNative(IntPtr native)
        {
            return new GameObject(native);
        }

        private Transform m_innerTransform = null;
        public Transform transform
        {
            get
            {
                if (m_innerTransform == null)
                {
                    IntPtr transformNative = GameObjectGetTransform(m_nativePtr);
                    m_innerTransform = Transform.CreateFromNative(transformNative);
                }

                return m_innerTransform;
            }
        }

        public Component[] GetComponents()
        {
            int size = GameObjectGetComponentsCount(m_nativePtr);
            if (size <= 0)
                return null;

            Component[] ret = new Component[size];
            for(int i = 0; i < size; ++i)
            {
                var comNativePtr = GameObjectGetComponent(m_nativePtr, i);
                var type = ComponentNative.ComponentGetComponentType(comNativePtr);
                Component comp = null;
                switch(type)
                {
                    case ComponentType.Renderer:
                        comp = new Renderer(comNativePtr);
                        break;
                    case ComponentType.MeshRenderer:
                        comp = new MeshRenderer(comNativePtr);
                        break;
                    default:
                        comp = new Component(comNativePtr);
                        break;
                }
                ret[i] = comp;
            }
            return ret;
        }

        public Component GetComponent()
        {
            Component[] comps = GetComponents();
            if (comps == null)
                return null;
            
            return comps[0];
        }

        public T[] GetComponents<T>() where T : Component
        {
            var comps = GetComponents();
            if (comps ==  null)
                return null;

            List<T> ret = new List<T>();
            foreach (Component comp in comps)
            {
                if (comp is T)
                    ret.Add(comp as T);

            }

            return ret.ToArray();
        }

        public T GetComponent<T>() where T : Component
        {
            var comps = GetComponents<T>();
            if (comps == null)
                return null;

            return comps[0];
        }

        public static void Destroy(GameObject gameObject)
        {
            if(gameObject.IsDestroy)
            {
                Debug.LogErrorFormat($"{gameObject.transform.name} is already destroy!");
                return;
            }

            GameObjectNative.GameObjectDestroy(gameObject.m_nativePtr);
            gameObject.IsDestroy = true;
        }
    }
}
