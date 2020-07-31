using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace RenderCoreNet
{
    public static unsafe partial class RC
    {
        public static TextureHandle LoadTexture(string _name, ushort _flags, Byte _skip, TextureInfoPtr _info, ref Orientation.Enum _orientation)
        {
            byte* native_label;
            int label_byteCount = 0;
            if (_name != null)
            {
                label_byteCount = System.Text.Encoding.UTF8.GetByteCount(_name);
                if (label_byteCount > Util.StackAllocationSizeLimit)
                {
                    native_label = Util.Allocate(label_byteCount + 1);
                }
                else
                {
                    byte* native_label_stackBytes = stackalloc byte[label_byteCount + 1];
                    native_label = native_label_stackBytes;
                }
                int native_label_offset = Util.GetUtf8(_name, native_label, label_byteCount);
                native_label[native_label_offset] = 0;
            }
            else { native_label = null; }

            TextureInfo* native_info = _info.NativePtr;
            fixed (Orientation.Enum* orientation_native = &_orientation)
            {
                return rcLoadTexture(native_label, _flags, _skip, native_info, orientation_native);
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

        public static int CreateCube()
        {
            return rcCreateCube();
        }

        public static void DestroyCube(int handle)
        {
            rcDestroyCube(handle);
        }

        public static void DrawCube(Byte viewId, int handle)
        {
            rcDrawCube(viewId, handle);
        }
    }
}
