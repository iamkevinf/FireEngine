using System;
using System.Collections.Generic;
using System.Text;

namespace RenderCoreNet
{
    public unsafe partial struct Cube
    {
    }

	public unsafe partial struct CubePtr
	{
		public Cube* NativePtr { get; }
		public CubePtr(Cube* nativePtr) => NativePtr = nativePtr;
		public CubePtr(IntPtr nativePtr) => NativePtr = (Cube*)nativePtr;
		public static implicit operator CubePtr(Cube* nativePtr) => new CubePtr(nativePtr);
		public static implicit operator Cube*(CubePtr wrappedPtr) => wrappedPtr.NativePtr;
		public static implicit operator CubePtr(IntPtr nativePtr) => new CubePtr(nativePtr);
	}

}
