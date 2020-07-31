using System;
using System.Runtime.InteropServices;

namespace RenderCoreNet
{
    public static unsafe partial class RC
    {
        public const string RCDllName = "FireWrapper";

        [DllImport(RCDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern TextureHandle rcLoadTexture(byte* _name, ushort _flags, Byte _skip, TextureInfoPtr _info, Orientation.Enum* _orientation);
        [DllImport(RCDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int rcCreateCube();
        [DllImport(RCDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void rcDestroyCube(int handle);
        [DllImport(RCDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void rcDrawCube(Byte viewId, int handle);



    }
}
