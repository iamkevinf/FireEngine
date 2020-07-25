using System;
using System.Runtime.InteropServices;

namespace RenderCoreNet
{
    public static unsafe partial class RC
    {
        public const string RCDllName = "FireWrapper";

        [DllImport(RCDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern TextureHandle rcLoadTexture(byte* _name, ushort _flags, Byte _skip, TextureInfoPtr _info, Orientation.Enum* _orientation);
    }
}
