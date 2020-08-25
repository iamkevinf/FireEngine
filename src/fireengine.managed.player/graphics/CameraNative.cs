using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
    public static unsafe class CameraNative
    {
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern IntPtr CameraCreate(TransformNative.TransformHandle parent, string name);

        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern TransformNative.TransformHandle CameraGetTransformHandle(IntPtr native);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern UInt32 CameraGetClearColor(IntPtr native);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CameraSetClearColor(IntPtr native, UInt32 color);

    }
}
