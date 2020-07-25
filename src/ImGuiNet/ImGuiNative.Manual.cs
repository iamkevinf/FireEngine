using System;
using System.Numerics;
using System.Runtime.InteropServices;

namespace ImGuiNET
{
    public static unsafe partial class ImGuiNative
    {
        public const string ImGuiDllName = "FireWrapper";

        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ImGuiPlatformIO_Set_Platform_GetWindowPos(ImGuiPlatformIO* platform_io, IntPtr funcPtr);
        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ImGuiPlatformIO_Set_Platform_GetWindowSize(ImGuiPlatformIO* platform_io, IntPtr funcPtr);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void igCallback();
        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int igApp_MainLoop(byte* title, igCallback OnInit, igCallback OnGUI, igCallback OnExit);
        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int igSetWindowTitle(ImGuiViewport* viewport, byte* title);

        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool igLoadTexture(byte* filename, uint* out_texture_id, int* out_width, int* out_height);

        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte igImageAnimButton(IntPtr user_texture_id, IntPtr user_texture_id2, Vector2 size, Vector2 uv0, Vector2 uv1, Vector2 uv2, Vector2 uv3, int frame_padding, Vector4 bg_col, Vector4 tint_col);
        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte igToggleButton(IntPtr user_texture_id, IntPtr user_texture_id2, bool* v, Vector2 size, Vector2 uv0, Vector2 uv1, Vector2 uv2, Vector2 uv3, int frame_padding, Vector4 bg_col, Vector4 tint_col);
        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void igToggleButtonStr(byte* str_id, bool* v);
        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool igImageRadioButtonBool(IntPtr tid_active, IntPtr tid_deactivate, Vector2 size, bool active);
        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool igImageRadioButtonIntPtr(IntPtr tid_active, IntPtr tid_deactivate, Vector2 size, int* v, int v_button);
        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr igGetTestFramebuffer();

        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void igTestInitDrawInCPP(int viewportWidth, int viewportHeight);
        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void igTestRenderInCPP(int viewportWidth, int viewportHeight);
        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void igTestFiniDrawInCPP();
        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr igGetTestRenderInCPPFBO();

        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void igbgfxInit(int viewportWidth, int viewportHeight);
        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void igbgfxTick();
        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void igbgfxFini();
        [DllImport(ImGuiDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr iggetbgfxfbo();
    }
}
