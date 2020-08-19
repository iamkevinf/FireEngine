using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
    public static unsafe  partial class FireEngineNative
    {
        public const string FireEngineDllName = "fireengine.native.player";

        [DllImport(FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern TextureHandle feLoadTexture(string _name, ushort _flags, Byte _skip, TextureInfoPtr _info, Orientation.Enum* _orientation);
        [DllImport(FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern TextureHandle feGetGameViewTexture();


        public unsafe delegate void delegateIMAssertHacker(char* _expr, char* filename, int line);

        [DllImport(FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void igSetIMAssertHacker(delegateIMAssertHacker callback);
    }
}
