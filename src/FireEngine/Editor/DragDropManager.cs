using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Text;

namespace FireEngine.Editor
{
    public enum DragDropWindow : byte
    {
        Hierarchy
    }

    public enum DragDropTree : byte
    {
        Scenes,
        Transforms
    }

    public static class DragDropManager
    {
        const int s_dragdrop_size = 32;
        private static byte[] s_dragdropDataD = new byte[s_dragdrop_size];
        private static byte[] s_dragdropDataE = new byte[s_dragdrop_size];
        private static byte[] s_dragdropPayloadData = new byte[s_dragdrop_size];
        private static byte[] s_tmpBytesForShort = new byte[2];
        public static void BeginDragDropSource(string name, byte[] data, UInt16 data_size,
            DragDropWindow window, DragDropTree tree,
            string payloadType, ImGuiDragDropFlags flags)
        {
            if (ImGui.BeginDragDropSource(flags))
            {
                s_dragdropDataD[0] = (byte)window;
                s_dragdropDataD[1] = (byte)tree;

                unsafe
                {
                    fixed (byte* b = s_tmpBytesForShort)
                    {
                        *((UInt16*)b) = data_size;
                    }

                    s_dragdropDataD[2] = s_tmpBytesForShort[0];
                    s_dragdropDataD[3] = s_tmpBytesForShort[1];
                }


                Array.Copy(data, 0, s_dragdropDataD, 4, data_size);
                IntPtr dataPtr;
                unsafe
                {
                    fixed (byte* native = &s_dragdropDataD[0])
                    {
                        dataPtr = (IntPtr)(native);
                    }
                }

                ImGui.SetDragDropPayload(payloadType, dataPtr, s_dragdrop_size);
                ImGui.Text(name);
                ImGui.EndDragDropSource();
            }
        }

        public static void BeginDragDropTarget(string playloadType, ImGuiDragDropFlags flags, Action<DragDropWindow, DragDropTree, UInt16, byte[]> action)
        {
            if (ImGui.BeginDragDropTarget())
            {
                var payload = ImGui.AcceptDragDropPayload(playloadType, flags);
                unsafe
                {
                    if (payload.NativePtr != null)
                    {
                        IntPtr data = payload.Data;
                        System.Runtime.InteropServices.Marshal.Copy(data, s_dragdropDataE, 0, s_dragdrop_size);
                        DragDropWindow window = (DragDropWindow)s_dragdropDataE[0];
                        DragDropTree tree = (DragDropTree)s_dragdropDataE[1];
                        UInt16 data_size = 0;
                        unsafe
                        {
                            fixed (byte* pbyte = &s_dragdropDataE[2])
                            {
                                data_size = *((UInt16*)pbyte);
                            }
                        }

                        Array.Copy(s_dragdropDataE, 4, s_dragdropPayloadData, 0, data_size);

                        if (action != null)
                            action(window, tree, data_size, s_dragdropPayloadData);
                    }
                }

                ImGui.EndDragDropTarget();
            }
        }
    }
}
