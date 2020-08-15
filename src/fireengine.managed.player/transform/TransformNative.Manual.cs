﻿using System;
using System.Collections.Generic;
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
                namestr = new string((char*)name);
            }

            return namestr;
        }


    }
}
