using System;
using System.Collections.Generic;
using System.Text;

namespace FireEngine
{
    public class Material : Object
    {
        public Material(Shader shader)
        {
            this.shader = shader;
        }

        public Shader shader
        {
            get;
            set;
        }
    }
}
