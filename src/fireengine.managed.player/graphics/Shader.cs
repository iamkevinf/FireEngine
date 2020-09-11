using System;
using System.Collections.Generic;
using System.Text;

namespace FireEngine
{
    public class Shader : Object
    {
        public IntPtr native = IntPtr.Zero;
        public Shader(IntPtr native)
        {
            this.native = native;
            name = ShaderNative.ShaderGetName(native);
        }

        public static Shader Find(string name)
        {
            var native = ShaderNative.ShaderCreate(name);
            if (native == IntPtr.Zero)
                return null;

            Shader shader = new Shader(native);
            shader.name = name;
            return shader;
        }

        #region Inner
        #endregion
    }
}
