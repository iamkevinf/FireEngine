using System;
using System.Collections.Generic;
using System.Text;

namespace FireEngine
{
    public class Shader : Object
    {
        public IntPtr native = IntPtr.Zero;
        public static Shader Find(string name)
        {
            var native = ShaderNative.ShaderCreate(name);
            if (native == IntPtr.Zero)
                return null;

            Shader shader = new Shader();
            shader.native = native;
            return shader;
        }

        #region Inner
        #endregion
    }
}
