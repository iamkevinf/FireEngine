using System;
using System.Collections.Generic;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
    public unsafe class MaterialNative
    {
        public struct PropertyVec4Node
        {
            public int name_size;
            public int desc_size;
            public byte* name;
            public byte* desc;
            public Vector4 value;
        }
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern IntPtr MaterialGetShader(IntPtr native);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern IntPtr MaterialCreate(string name);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, EntryPoint = "MaterialGetName")]
        public static extern byte* MaterialGetNameNative(IntPtr native);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void MaterialSetVector(IntPtr native, string name, Vector4 v);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
		public static extern Vector4 MaterialGetVector(IntPtr native, string name);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
		public static extern bool MaterialHasVector(IntPtr native, string name);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void MaterialSetTexture(IntPtr native, string name, IntPtr v);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
		public static extern IntPtr MaterialGetTexture(IntPtr native, string name);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
		public static extern bool MaterialHasTexture(IntPtr native, string name);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern int MaterialGetPropertyVec4Count(IntPtr native);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern IntPtr MaterialGetPropertyVec4(IntPtr native, int index);

        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern byte* MaterialGetPropertyVec4Name(IntPtr native);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern byte* MaterialGetPropertyVec4Desc(IntPtr native);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern Vector4 MaterialGetPropertyVec4Value(IntPtr native);

        public static string MaterialGetName(IntPtr native)
        {
            byte* ptr = MaterialGetNameNative(native);
            if (ptr == null)
                return "";

            int characters = 0;
            while (ptr[characters] != 0)
            {
                characters++;
            }

            return Encoding.UTF8.GetString(ptr, characters);
        }

        public static string MaterialGetPropertyVec4NameStr(IntPtr native)
        {
            byte* ptr = MaterialGetPropertyVec4Name(native);
            if (ptr == null)
                return "";

            int characters = 0;
            while (ptr[characters] != 0)
            {
                characters++;
            }

            return Encoding.UTF8.GetString(ptr, characters);
        }

        public static string MaterialGetPropertyVec4DescStr(IntPtr native)
        {
            byte* ptr = MaterialGetPropertyVec4Desc(native);
            if (ptr == null)
                return "";

            int characters = 0;
            while (ptr[characters] != 0)
            {
                characters++;
            }

            return Encoding.UTF8.GetString(ptr, characters);
        }
    }
}
