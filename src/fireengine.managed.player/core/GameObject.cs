using System;
using System.Collections.Generic;
using System.Text;
using static FireEngine.GameObjectNative;

namespace FireEngine
{
    public class GameObject
    {
        #region Inner

        private GameObjectHandle inner_handle = GameObjectHandle.InValid;
        private GameObject(GameObjectNative.GameObjectHandle handle)
        {
            inner_handle = handle;
        }

        #endregion

        #region Transform



        public TransformNative.TransformHandle transformHandle
        {
            get
            {
                return GameObjectGetTransformHandle(inner_handle);
            }
        }

        #endregion

        public static GameObject Inner_Create(TransformNative.TransformHandle parent, string name)
        {
            GameObjectHandle handle = GameObjectCreate(parent, name);
            return new GameObject(handle);
        }
    }
}
