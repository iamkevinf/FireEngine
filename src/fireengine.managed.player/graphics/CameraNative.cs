using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
    public static unsafe class CameraNative
    {
        public struct CameraHandle
        {
            public UInt16 idx;
            public static CameraHandle InValid = new CameraHandle() { idx = FireEngineNative.kInvalidHandle };

            public bool IsValid()
            {
                return idx != FireEngineNative.kInvalidHandle;
            }
            public static bool operator ==(CameraHandle lhs, CameraHandle rhs)
            {
                return lhs.idx == rhs.idx;
            }
            public static bool operator !=(CameraHandle lhs, CameraHandle rhs)
            {
                return lhs.idx != rhs.idx;
            }
        }

        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern CameraHandle CameraCreate(TransformNative.TransformHandle parent, string name);

        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern TransformNative.TransformHandle CameraGetTransformHandle(CameraHandle handle);

    }
}
