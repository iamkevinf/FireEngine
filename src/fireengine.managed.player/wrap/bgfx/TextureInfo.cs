using System;
using System.Runtime.CompilerServices;

namespace FireEngine
{
	public unsafe partial struct TextureInfo
	{
		public TextureFormat.Enum format; //!< Texture format.
		public uint storageSize;       //!< Total amount of bytes required to store texture.
		public ushort width;             //!< Texture width.
		public ushort height;            //!< Texture height.
		public ushort depth;             //!< Texture depth.
		public ushort numLayers;         //!< Number of layers in texture array.
		public Byte numMips;            //!< Number of MIP maps.
		public Byte bitsPerPixel;       //!< Format bits per pixel.
		public bool cubeMap;            //!< Texture is cubemap.
	};


	public unsafe partial struct TextureInfoPtr
	{
		public TextureInfo* NativePtr { get; }
		public TextureInfoPtr(TextureInfo* nativePtr) => NativePtr = nativePtr;
		public TextureInfoPtr(IntPtr nativePtr) => NativePtr = (TextureInfo*)nativePtr;
		public static implicit operator TextureInfoPtr(TextureInfo* nativePtr) => new TextureInfoPtr(nativePtr);
		public static implicit operator TextureInfo*(TextureInfoPtr wrappedPtr) => wrappedPtr.NativePtr;
		public static implicit operator TextureInfoPtr(IntPtr nativePtr) => new TextureInfoPtr(nativePtr);

		public ref TextureFormat.Enum format => ref Unsafe.AsRef<TextureFormat.Enum>(&NativePtr->format);
		public ref uint storageSize => ref Unsafe.AsRef<uint>(&NativePtr->storageSize);
		public ref ushort width => ref Unsafe.AsRef<ushort>(&NativePtr->width);
		public ref ushort height => ref Unsafe.AsRef<ushort>(&NativePtr->height);
		public ref ushort depth => ref Unsafe.AsRef<ushort>(&NativePtr->depth);
		public ref ushort numLayers => ref Unsafe.AsRef<ushort>(&NativePtr->numLayers);
		public ref Byte numMips => ref Unsafe.AsRef<Byte>(&NativePtr->numMips);
		public ref Byte bitsPerPixel => ref Unsafe.AsRef<Byte>(&NativePtr->bitsPerPixel);
		public ref bool cubeMap => ref Unsafe.AsRef<bool>(&NativePtr->cubeMap);
	}
}
