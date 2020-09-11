using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
	public enum ComponentType
	{
		None = -1,
		Transform,
		Camera,
		Renderer,
		MeshRenderer,
	};

	public class ComponentNative
    {
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern ComponentType ComponentGetComponentType(IntPtr native);

    }
}
