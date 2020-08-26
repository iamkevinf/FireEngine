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
        }


        #endregion

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

        #region Native
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern IntPtr GameObjectGetTransform(IntPtr native);

        #endregion
    }
}
