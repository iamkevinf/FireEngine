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
        public TransformNative.TransformHandle transformHandle
        {
            get;
            protected set;
        }

        public Transform(IntPtr native) : base(native)
        {
            transformHandle = TransformNative.TransformGetTransformHandle(native);
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

        public Quaternion rotation
        {
            set
            {
                TransformSetWorldRotation(m_nativePtr, value);
            }
            get
            {
                Quaternion rot = Quaternion.identity;
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

        public Vector3 localPosition
        {
            set
            {
                TransformSetLocalPosition(m_nativePtr, value);
            }
            get
            {
                Vector3 pos = Vector3.Zero;
                TransformGetLocalPosition(m_nativePtr, ref pos);
                return pos;
            }
        }

        public Quaternion localRotation
        {
            set
            {
                TransformSetLocalRotation(m_nativePtr, value);
            }
            get
            {
                Quaternion rot = Quaternion.identity;
                TransformGetLocalRotation(m_nativePtr, ref rot);
                return rot;
            }
        }

        public Vector3 localScale
        {
            set
            {
                TransformSetLocalScale(m_nativePtr, value);
            }
            get
            {
                Vector3 scl = Vector3.Zero;
                TransformGetLocalScale(m_nativePtr, ref scl);
                return scl;
            }
        }


        #region Native
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformGetWorldPosition(IntPtr native, ref Vector3 pos);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformSetWorldPosition(IntPtr native, Vector3 pos);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformGetWorldRotation(IntPtr native, ref Quaternion rot);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformSetWorldRotation(IntPtr native, Quaternion rot);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformGetWorldScale(IntPtr native, ref Vector3 scl);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformSetWorldScale(IntPtr native, Vector3 scl);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformGetLocalPosition(IntPtr native, ref Vector3 pos);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformSetLocalPosition(IntPtr native, Vector3 pos);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformGetLocalRotation(IntPtr native, ref Quaternion rot);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformSetLocalRotation(IntPtr native, Quaternion rot);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformGetLocalScale(IntPtr native, ref Vector3 scl);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformSetLocalScale(IntPtr native, Vector3 scl);
        #endregion


    }
}
