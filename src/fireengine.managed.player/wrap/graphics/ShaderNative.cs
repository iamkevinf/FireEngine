using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
    public static unsafe class ShaderNative
    {
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern IntPtr ShaderCreate(string name);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, EntryPoint = "ShaderGetName")]
        public static extern byte* ShaderGetNameNative(IntPtr native, ref int size);
        
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, EntryPoint = "ShaderGetNameList")]
        public static extern byte* ShaderGetNameListNative(ref int size);

        public static string ShaderGetName(IntPtr native)
        {
            int size = 0;
            var name = ShaderGetNameNative(native, ref size);
            string namestr;
            unsafe
            {
                namestr = new string((sbyte*)name, 0, size);
            }

            return namestr;
        }

        public static string ShaderGetNameList()
        {
            int size = 0;
            var name = ShaderGetNameListNative(ref size);

            string namestr;
            unsafe
            {
                namestr = new string((sbyte*)name, 0, size);
            }

            return namestr;
        }


    }
}
