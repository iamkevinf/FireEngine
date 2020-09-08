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


        public unsafe delegate void delegateIMAssertHacker(char* _expr, char* filename, int line);
        public unsafe delegate void LogHacker(char* _expr, char* filename, int line, int level);

        [DllImport(FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void igSetIMAssertHacker(delegateIMAssertHacker callback);
        [DllImport(FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void febgfxSetDebug(UInt32 debug, bool debugGame);
        [DllImport(FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void feSetOnLogHacker(LogHacker callback);

    }
}
