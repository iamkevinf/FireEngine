using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;
using static FireEngine.CameraNative;

namespace FireEngine
{
    public class Camera : Transform
    {
        #region Inner

        public TransformNative.TransformHandle transformHandle
        {
            get;
            private set;
        }
        private Camera(IntPtr native) : base(native)
        {
            transformHandle = CameraGetTransformHandle(native);
        }
  
        #endregion

        #region Transform

        #endregion

        public static Camera Inner_Create(TransformNative.TransformHandle parent, string name)
        {
            IntPtr native = CameraCreate(parent, name);
            Camera camera = new Camera(native);
            return camera;
        }

        uint clearColor
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

        public override void OnGUI_Inspector()
        {
            base.OnGUI_Inspector();

            if (ImGui.CollapsingHeader("Camera##Inspector", treeNodeFlags))
            {
                float r = 0, g = 0, b = 0, a = 0; ;
                Vector4 color = ImGui.ColorConvertU32ToFloat4(clearColor);
                a = color.X;
                b = color.Y;
                g = color.Z;
                r = color.W;
                Vector4 color4 = Vector4.Zero;
                color4.X = r;
                color4.Y = g;
                color4.Z = b;
                color4.W = a;
                ImGui.ColorEdit4("Background", ref color4); ImGui.SameLine();
                if(ImGui.Button("↙##Inspector##Camera##PickColor"))
                {
                    ImGui.OpenPopup("PickColor##RightMenu##Inspector##Camera");
                }
                if (ImGui.BeginPopup("PickColor##RightMenu##Inspector##Camera"))
                {
                    ImGui.ColorPicker4("PickColor##Inspector##Camera#", ref color4);
                    ImGui.EndPopup();
                }
                color.W = color4.X;
                color.Z = color4.Y;
                color.Y = color4.Z;
                color.X = color4.W;
                clearColor = ImGui.ColorConvertFloat4ToU32(color4);

            }
        }

    }
}
