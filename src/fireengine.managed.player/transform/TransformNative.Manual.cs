using System;
using System.Collections.Generic;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
    public static unsafe partial class TransformNative
    {

        public static string TransformGetName(TransformHandle handle)
        {
            var name = TransformGetNameNative(handle);

            string namestr;
            unsafe
            {
                namestr = new string((sbyte*)name);
            }

            return namestr;
        }

        public static void TransformGetWorldPosition(TransformHandle handle, ref Vector3 pos)
        {
            fixed (Vector3* native_pos = &pos)
            {
                TransformGetWorldPosition(handle, native_pos);
            }
        }

    }
}
