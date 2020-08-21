using System;
using System.Collections.Generic;
using System.Text;
using static FireEngine.CameraNative;

namespace FireEngine
{
    public class Camera
    {
        #region Inner

        private CameraHandle inner_handle = CameraHandle.InValid;
        private Camera(CameraHandle handle)
        {
            inner_handle = handle;
        }

        #endregion

        #region Transform
        public TransformNative.TransformHandle transformHandle
        {
            get
            {
                return CameraGetTransformHandle(inner_handle);
            }
        }

        #endregion

        public static Camera Inner_Create(TransformNative.TransformHandle parent, string name)
        {
            CameraHandle handle = CameraCreate(parent, name);
            return new Camera(handle);
        }

    }
}
