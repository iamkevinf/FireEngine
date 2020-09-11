using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
    public static unsafe class GameObjectNative
    {
        public struct GameObjectHandle
        {
            public UInt16 idx;
            public static GameObjectHandle InValid = new GameObjectHandle() { idx = FireEngineNative.kInvalidHandle };

            public bool IsValid()
            {
                return idx != FireEngineNative.kInvalidHandle;
            }
            public static bool operator ==(GameObjectHandle lhs, GameObjectHandle rhs)
            {
                return lhs.idx == rhs.idx;
            }
            public static bool operator !=(GameObjectHandle lhs, GameObjectHandle rhs)
            {
                return lhs.idx != rhs.idx;
            }
        }

        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern GameObjectHandle GameObjectCreate(TransformNative.TransformHandle parent, string name);

        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern TransformNative.TransformHandle GameObjectGetTransformHandle(GameObjectHandle handle);

        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GameObjectGetComponentsCount(IntPtr native);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GameObjectGetComponent(IntPtr native, int index);

    }
}
