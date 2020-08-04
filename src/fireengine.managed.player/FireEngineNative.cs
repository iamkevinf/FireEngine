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

    }
}
