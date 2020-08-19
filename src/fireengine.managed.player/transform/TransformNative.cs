using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
    public static unsafe partial class TransformNative
    {
        public struct TransformHandle
        {
            public UInt16 idx;
        }

        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern TransformHandle TransformCreate(TransformHandle parent, string name);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void TransformRemove(TransformHandle handle);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int TransformChildCount(TransformHandle parent);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void TransformMove(TransformHandle who, TransformHandle to);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern TransformHandle TransformFindChildByIndex(TransformHandle handle, int index);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void TransformSetName(TransformHandle handle, string name);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, EntryPoint="TransformGetName")]
        public static extern byte* TransformGetNameNative(TransformHandle handle);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void TransformSetActive(TransformHandle handle, string name);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern SceneNative.ActiveOption TransformGetActive(TransformHandle handle);
    }
}
