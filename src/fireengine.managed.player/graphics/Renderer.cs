using System;
using System.Collections.Generic;
using System.Text;

namespace FireEngine
{
    public class Renderer : Component
    {
        public Renderer(IntPtr native) : base(native)
        {
        }

        public Material[] GetMaterials()
        {
            int size = RendererNative.RendererGetMaterialsCount(m_nativePtr);
            if (size <= 0)
                return null;
            Material[] ret = new Material[size];
            for(int i = 0; i < size; ++i)
            {
                IntPtr matNative = RendererNative.RendererGetMaterial(m_nativePtr, i);
                ret[i] = new Material(matNative);
            }
            return ret;
        }
    }
}
