using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;
using static FireEngine.CameraNative;

namespace FireEngine
{
    public class Camera : Component
    {
        #region Inner
        public TransformNative.TransformHandle transformHandle
        {
            get;
            protected set;
        }

        private Camera(IntPtr native) : base(native)
        {
            transformHandle = CameraGetTransformHandle(native);
        }

        #endregion

        public static Camera Inner_Create(TransformNative.TransformHandle parent, string name)
        {
            IntPtr native = CameraCreate(parent, name);
            Camera camera = new Camera(native);
            return camera;
        }

        public uint clearColor
        {
            get
            {
                if (m_nativePtr == IntPtr.Zero)
                    return 0;

                return CameraGetClearColor(m_nativePtr);
            }
            set
            {
                if (m_nativePtr == IntPtr.Zero)
                    return;

                CameraSetClearColor(m_nativePtr, value);
            }
        }

        private static Camera s_MainCamera = null;
        public static Camera Main
        {
            get
            {
                if(s_MainCamera == null)
                {
                    IntPtr native = CameraGetMainCamera();

                    s_MainCamera = native == IntPtr.Zero ? null : new Camera(native);
                }

                return s_MainCamera;
            }
        }

        public IntPtr frameBufferTexture
        {
            get
            {
                return CameraGetFrameBufferTexture(m_nativePtr);
            }
        }
    }
}
