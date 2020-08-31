using System;
using System.Collections.Generic;
using System.Text;

namespace FireEngine
{
    public class Defines
    {
        static public readonly ulong BGFX_TEXTURE_NONE = 0x0000000000000000;
        static public readonly uint BGFX_SAMPLER_NONE = 0x00000000;


        static public readonly uint BGFX_DEBUG_NONE = 0x00000000; //!< No debug.
    }

    namespace bgfx
    {
        public enum Debug
        {
            BGFX_DEBUG_NONE = 0x00000000,
            BGFX_DEBUG_WIREFRAME = 0x00000001,
            BGFX_DEBUG_IFH = 0x00000002,
            BGFX_DEBUG_STATS = 0x00000004,
            BGFX_DEBUG_TEXT = 0x00000008,
            BGFX_DEBUG_PROFILER = 0x00000010
        }

    }
}
