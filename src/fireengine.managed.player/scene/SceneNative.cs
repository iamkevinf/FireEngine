using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
    public static unsafe partial class SceneNative
    {
        public struct SceneHandle
        {
            public UInt16 idx;
            public static SceneHandle InValid = new SceneHandle() { idx = FireEngineNative.kInvalidHandle };
            public bool IsValid()
            {
                return idx != FireEngineNative.kInvalidHandle;
            }
            public static bool operator ==(SceneHandle lhs, SceneHandle rhs)
            {
                return lhs.idx == rhs.idx;
            }
            public static bool operator !=(SceneHandle lhs, SceneHandle rhs)
            {
                return lhs.idx != rhs.idx;
            }
        }

        public enum ActiveOption
        {
            Active = 1,
            Deactive = 0,
        };


        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern SceneHandle SceneCreate(string name);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void SceneRemove(SceneHandle handle);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern int SceneCount();
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern SceneHandle SceneFindByIndex(int index);



        //场景名字
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void SceneSetName(SceneHandle scene, string name);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, EntryPoint="SceneGetName")]
        public static extern IntPtr SceneGetNameNative(SceneHandle scene);

        //场景是否活动
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void SceneSetActive(SceneHandle scene, ActiveOption active);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern ActiveOption SceneGetActive(SceneHandle scene);


        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern TransformNative.TransformHandle SceneGetRoot(SceneHandle scene);

    }
}
