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
        private static byte[] s_tmpBytesForShort = new byte[2];
        public static bool DragDropSource(string name, ushort handleIdx,
            DragDropWindow window, DragDropTree tree,
            string payloadType, ImGuiDragDropFlags flags)
        {
            if (ImGui.BeginDragDropSource(flags))
            {
                s_dragdropDataD[0] = (byte)window;
                s_dragdropDataD[1] = (byte)tree;

                Util.GetBytesGC0(handleIdx, ref s_tmpBytesForShort);
                Array.Copy(s_tmpBytesForShort, 0, s_dragdropDataD, 2, 2);

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

                return true;
            }

            return false;
        }

        public static bool DragDropTarget(ushort handleIdx,
            DragDropWindow window, DragDropTree tree,
            string payloadType, ImGuiDragDropFlags flags,
            Action<DragDropWindow, DragDropTree, ushort,
                DragDropWindow, DragDropTree, ushort> action)
        {
            bool ret = false;
            if (ImGui.BeginDragDropTarget())
            {
                var payload = ImGui.AcceptDragDropPayload(payloadType, flags);
                unsafe
                {
                    if (payload.NativePtr != null)
                    {
                        IntPtr dataSource = payload.Data;
                        System.Runtime.InteropServices.Marshal.Copy(dataSource, s_dragdropDataE, 0, s_dragdrop_size);
                        DragDropWindow windowSource = (DragDropWindow)s_dragdropDataE[0];
                        DragDropTree treeSource = (DragDropTree)s_dragdropDataE[1];
                        ushort handleIdxSource = 0;
                        unsafe
                        {
                            fixed (byte* pbyte = &s_dragdropDataE[2])
                            {
                                handleIdxSource = *((ushort*)pbyte);
                            }
                        }

                        if (action != null)
                            action(windowSource, treeSource, handleIdxSource,
                                window, tree, handleIdx);

                        ret = true;
                    }
                }

                ImGui.EndDragDropTarget();
            }

            return ret;
        }

        public static bool DragDrop(string name, ushort handleIdx,
            DragDropWindow window, DragDropTree tree,
            string payloadType, ImGuiDragDropFlags flags,
            Action<DragDropWindow, DragDropTree, ushort,
                DragDropWindow, DragDropTree, ushort> action)
        {
            bool ret = DragDropTarget(handleIdx, window, tree, payloadType, flags, action);
            DragDropSource(name, handleIdx, window, tree, payloadType, flags);
            return ret;
        }


        public static void OnDragDropAction(
            DragDropWindow windowS, DragDropTree treeS, ushort handleIdxS,
            DragDropWindow windowT, DragDropTree treeT, ushort handleIdxT)
        {
            Console.WriteLine(string.Format("Source {0} {1} {2}", windowS, treeS, handleIdxS));
            Console.WriteLine(string.Format("Target {0} {1} {2}", windowT, treeT, handleIdxT));

            switch (windowS)
            {
                case DragDropWindow.Hierarchy:
                    OnDragDropFromHierarchy(treeS, handleIdxS, windowT, treeT, handleIdxT);
                    break;

                default:
                    Console.WriteLine("未处理");
                    break;
            }

        }

        private static void OnDragDropFromHierarchy(DragDropTree treeS, ushort handleIdxS,
            DragDropWindow windowT, DragDropTree treeT, ushort handleIdxT)
        {
            switch (windowT)
            {
                case DragDropWindow.Hierarchy:
                    OnDragDropFromHierarchyToHierachy(treeS, handleIdxS, treeT, handleIdxT);
                    break;

                default:
                    Console.WriteLine("未处理2");
                    break;
            }
        }

        private static void OnDragDropFromHierarchyToHierachy(
            DragDropTree treeS, ushort handleIdxS,
            DragDropTree treeT, ushort handleIdxT)
        {
            switch(treeS)
            {
                case DragDropTree.Scenes:
                    OnDragDropFromHierarchySceneToHierachy(handleIdxS, treeT, handleIdxT);
                    break;

                case DragDropTree.Transforms:
                    OnDragDropFromHierarchyTransformToHierachy(handleIdxS, treeT, handleIdxT);
                    break;

                default:
                    break;
            }
        }

        private static void OnDragDropFromHierarchySceneToHierachy(ushort handleIdxS,
            DragDropTree treeT, ushort handleIdxT)
        {
            switch(treeT)
            {
                case DragDropTree.Scenes:
                    OnDragDropFromHierarchySceneToHierachyScene(handleIdxS, handleIdxT);
                    break;

                case DragDropTree.Transforms:
                    OnDragDropFromHierarchySceneToHierachyTransform(handleIdxS, handleIdxT);
                    break;

                default:
                    break;
            }
        }

        private static void OnDragDropFromHierarchyTransformToHierachy(ushort handleIdxS,
            DragDropTree treeT, ushort handleIdxT)
        {
            switch (treeT)
            {
                case DragDropTree.Scenes:
                    OnDragDropFromHierarchyTransformToHierachyScene(handleIdxS, handleIdxT);
                    break;

                case DragDropTree.Transforms:
                    OnDragDropFromHierarchyTransformToHierachyTransform(handleIdxS, handleIdxT);
                    break;

                default:
                    break;
            }
        }

        private static void OnDragDropFromHierarchySceneToHierachyScene(
            ushort handleIdxS, ushort handleIdxT)
        {
            Console.WriteLine("不允许这样做");
        }

        private static void OnDragDropFromHierarchySceneToHierachyTransform(
            ushort handleIdxS, ushort handleIdxT)
        {
            Console.WriteLine("不允许这样做");
        }


        private static void OnDragDropFromHierarchyTransformToHierachyScene(
            ushort handleIdxS, ushort handleIdxT)
        {
            TransformNative.TransformHandle handleS;
            handleS.idx = handleIdxS;

            string name = TransformNative.TransformGetName(handleS);
            TransformNative.TransformRemove(handleS);

            SceneNative.SceneHandle handleT;
            handleT.idx = handleIdxT;
            var root = SceneNative.SceneGetRoot(handleT);

            TransformNative.TransformCreate(root, name);
        }

        private static void OnDragDropFromHierarchyTransformToHierachyTransform(
            ushort handleIdxS, ushort handleIdxT)
        {
            TransformNative.TransformHandle handleS;
            handleS.idx = handleIdxS;
            TransformNative.TransformHandle handleT;
            handleT.idx = handleIdxT;
            TransformNative.TransformMove(handleS, handleT);
        }
    }
}
