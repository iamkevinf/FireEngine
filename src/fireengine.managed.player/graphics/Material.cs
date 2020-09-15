using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace FireEngine
{
    public class Material : Object
    {
        IntPtr m_nativePtr = IntPtr.Zero;
        public string fullname
        {
            get;
            private set;
        }
        public IntPtr native
        {
            get => m_nativePtr;
        }

        public Material(IntPtr native)
        {
            m_nativePtr = native;
            IntPtr shaderNative = MaterialNative.MaterialGetShader(native);
            shader = new Shader(shaderNative);
            SetName();
        }

        public Material(string path)
        {
            m_nativePtr = MaterialNative.MaterialCreate(path);
            IntPtr shaderNative = MaterialNative.MaterialGetShader(native);
            shader = new Shader(shaderNative);
            SetName();
        }

        void SetName()
        {
            fullname = MaterialNative.MaterialGetName(native);
            int idxDot = fullname.LastIndexOf('.');
            if (idxDot >= 0)
                name = fullname.Substring(0, idxDot);
            int idxG = fullname.LastIndexOf('/');
            if(idxG < 0)
                idxG = fullname.LastIndexOf('\\');
            if (idxG >= 0)
                name = name.Substring(idxG + 1, idxDot - idxG - 1);
        }

        public Shader shader
        {
            get;
            set;
        }

        public void SetVector(string name, Vector4 v)
        {
            MaterialNative.MaterialSetVector(m_nativePtr, name, v);
        }
    }
}
