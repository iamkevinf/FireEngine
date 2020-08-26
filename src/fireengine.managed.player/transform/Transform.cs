using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
    public partial class Transform : Component
    {
        public Transform(IntPtr native) : base(native)
        {

        }

        public static Transform CreateFromNative(IntPtr native)
        {
            return new Transform(native);
        }

        public Vector3 position
        {
            set
            {
                TransformSetWorldPosition(m_nativePtr, value);
            }
            get
            {
                Vector3 pos = Vector3.Zero;
                TransformGetWorldPosition(m_nativePtr, ref pos);
                return pos;
            }
        }

        public Vector4 rotation
        {
            set
            {
                TransformSetWorldRotation(m_nativePtr, value);
            }
            get
            {
                Vector4 rot = Vector4.Zero;
                TransformGetWorldRotation(m_nativePtr, ref rot);
                return rot;
            }
        }

        public Vector3 scale
        {
            set
            {
                TransformSetWorldScale(m_nativePtr, value);
            }
            get
            {
                Vector3 scl = Vector3.Zero;
                TransformGetWorldScale(m_nativePtr, ref scl);
                return scl;
            }
        }


        #region Native
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformGetWorldPosition(IntPtr native, ref Vector3 pos);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformSetWorldPosition(IntPtr native, Vector3 pos);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformGetWorldRotation(IntPtr native, ref Vector4 rot);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformSetWorldRotation(IntPtr native, Vector4 rot);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformGetWorldScale(IntPtr native, ref Vector3 scl);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformSetWorldScale(IntPtr native, Vector3 scl);
        #endregion


    }
}
