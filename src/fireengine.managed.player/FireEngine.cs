using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FireEngine
{
    public static unsafe partial class FireEngineNative
    {
        public static UInt16 kInvalidHandle = UInt16.MaxValue;
        public static TextureHandle LoadTexture(string _name, ushort _flags, Byte _skip, TextureInfoPtr _info, ref Orientation.Enum _orientation)
        {
            TextureInfo* native_info = _info.NativePtr;
            fixed (Orientation.Enum* orientation_native = &_orientation)
            {
                return feLoadTexture(_name, _flags, _skip, native_info, orientation_native);
            }
        }

        public static TextureHandle LoadTexture(string name, out Vector2 size)
        {
            TextureInfo i = new TextureInfo();
            TextureInfoPtr info = new TextureInfoPtr(&i);
            Orientation.Enum orientation = new Orientation.Enum();
            ushort flag = (ushort)(Defines.BGFX_TEXTURE_NONE | Defines.BGFX_SAMPLER_NONE);

            var handle = LoadTexture(name, flag, 0, info, ref orientation);

            if (info.NativePtr != null)
            {
                size.X = info.width;
                size.Y = info.height;
            }
            else
            {
                size = Vector2.Zero;
            }

            return handle;
        }

        public static void SetIMAssertHacker(delegateIMAssertHacker callback)
        {
            igSetIMAssertHacker(callback);
        }

        public static void SetOnEngineLogHacker(LogHacker callback)
        {
            feSetOnLogHacker(callback);
        }

    }
}
