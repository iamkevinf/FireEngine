using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
    public static unsafe partial class SceneNative
    {

        public static string SceneGetName(SceneHandle scene)
        {
            var name = SceneGetNameNative(scene);

            string namestr;
            unsafe
            {
                namestr = new string((char*)name);
            }

            return namestr;
        }


    }
}
