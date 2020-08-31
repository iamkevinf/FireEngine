using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEditor
{
    public static unsafe class nfdnative
    {
        public const string FireEditorDllName = "fireengine.native.editor";

        public enum nfdresult
        {
            NFD_ERROR,       /* programmatic error */
            NFD_OKAY,        /* user pressed okay, or successful return */
            NFD_CANCEL       /* user pressed cancel */
        }

        [DllImport(FireEditorDllName, CallingConvention = CallingConvention.Cdecl)]
        public extern static nfdresult feNFD_PickFolder(byte* defaultPath, byte** outPath);

        public static nfdresult NFD_PickFolder(string defaultPath, ref string outPath, int maxLength=128)
        {
            int utf8DefaultByteCount = Encoding.UTF8.GetByteCount(defaultPath);
            byte* utf8Defalutytes;
            if (utf8DefaultByteCount > 2048)
            {
                utf8Defalutytes = (byte*)Marshal.AllocHGlobal(utf8DefaultByteCount + 1);
            }
            else
            {
                byte* stackPtr = stackalloc byte[utf8DefaultByteCount + 1];
                utf8Defalutytes = stackPtr;
            }
            fixed (char* utf16Ptr = defaultPath)
            {
                Encoding.UTF8.GetBytes(utf16Ptr, defaultPath.Length, utf8Defalutytes, utf8DefaultByteCount);
            }


            int utf8OutByteCount = Encoding.UTF8.GetByteCount(outPath);
            int outBufSize = Math.Max((int)maxLength + 1, utf8OutByteCount + 1);

            byte* utfOutBytes;
            byte* originalUtf8OutBytes;
            if (outBufSize > 2048)
            {
                utfOutBytes = (byte*)Marshal.AllocHGlobal(outBufSize);
                originalUtf8OutBytes = (byte*)Marshal.AllocHGlobal(outBufSize);
            }
            else
            {
                byte* inputStackBytes = stackalloc byte[outBufSize];
                utfOutBytes = inputStackBytes;
                byte* originalInputStackBytes = stackalloc byte[outBufSize];
                originalUtf8OutBytes = originalInputStackBytes;
            }

            fixed (char* utf16Ptr = outPath)
            {
                Encoding.UTF8.GetBytes(utf16Ptr, outPath.Length, utfOutBytes, outBufSize);
            }
            uint clearBytesCount = (uint)(outBufSize - utf8OutByteCount);
            Unsafe.InitBlockUnaligned(utfOutBytes + utf8OutByteCount, 0, clearBytesCount);
            Unsafe.CopyBlock(originalUtf8OutBytes, utfOutBytes, (uint)outBufSize);

            var ret = feNFD_PickFolder(utf8Defalutytes, &utfOutBytes);

            int characters = 0;
            while (utfOutBytes[characters] != 0)
            {
                characters++;
            }

            outPath = Encoding.UTF8.GetString(utfOutBytes, characters);

            return ret;
        }
    }
}
