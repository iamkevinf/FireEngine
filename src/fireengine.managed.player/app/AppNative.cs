using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
    public static unsafe partial class AppNative
    {
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern int feApp_MainLoop(string _name, Action OnInit, Action OnGUI, Action OnExit);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern int feApp_SetWindowTitle(string _name);

    }
}
