using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FireEngine
{
    public class Material : Object
    {
        IntPtr native = IntPtr.Zero;
        public Material(IntPtr native)
        {
            this.native = native;
            IntPtr shaderNative = MaterialNative.MaterialGetShader(native);
            shader = new Shader(shaderNative);
        }

        public Material(Shader shader)
        {
            this.shader = shader;
            native = MaterialNative.MaterialCreate(shader.name);
            name = shader.name;
        }

        public Shader shader
        {
            get;
            set;
        }

        public void SetVector(string name, Vector4 v)
        {
            MaterialNative.MaterialSetVector(native, name, v);
        }
    }
}
