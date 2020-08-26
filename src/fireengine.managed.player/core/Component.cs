using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
    public class Component
    {
        protected ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags.DefaultOpen;
        Vector3 pos = Vector3.Zero;
        Vector4 rot = Vector4.Zero;
        Vector3 scl = Vector3.One;

        protected IntPtr m_nativePtr = IntPtr.Zero;
        private Component()
        {

        }

        public Component(IntPtr native) : this()
        {
            m_nativePtr = native;

            ComponentRegisterCallScriptable(native, ScriptableFuncType.Awake, Awake);
            ComponentRegisterCallScriptable(native, ScriptableFuncType.Start, Start);
            ComponentRegisterCallScriptable(native, ScriptableFuncType.Update, Update);
            ComponentRegisterCallScriptable(native, ScriptableFuncType.LateUpdate, LateUpdate);
            ComponentRegisterCallScriptable(native, ScriptableFuncType.OnEnable, OnEnable);
            ComponentRegisterCallScriptable(native, ScriptableFuncType.OnDisable, OnDisable);
        }

        public virtual void OnGUI_Inspector()
        {
            if (ImGui.CollapsingHeader("Transform##Inspector", treeNodeFlags))
            {
                float textWidth = Math.Min(ImGui.GetContentRegionAvail().X * 0.75f, 300);
                ImGui.SetNextItemWidth(textWidth);
                pos = gameObject.transform.position;
                ImGui.DragFloat3("##Position##Component##Inspector", ref pos); ImGui.SameLine();
                gameObject.transform.position = pos;
                ImGui.Text("Position");

                ImGui.SetNextItemWidth(textWidth);
                rot = gameObject.transform.rotation;
                ImGui.DragFloat4("##Rotation#Component##Inspector", ref rot); ImGui.SameLine();
                gameObject.transform.rotation = rot;
                ImGui.Text("Rotation");

                ImGui.SetNextItemWidth(textWidth);
                scl = gameObject.transform.scale;
                ImGui.DragFloat3("##Scale##Component##Inspector", ref scl); ImGui.SameLine();
                gameObject.transform.scale = scl;
                ImGui.Text("Scale");
            }
        }

        #region Callback
        public virtual void Awake()
        {
        }

        public virtual void Start()
        {
        }

        public virtual void Update()
        {
        }
        public virtual void LateUpdate()
        {
        }


        public virtual void OnEnable()
        {
        }

        public virtual void OnDisable()
        {
        }
        #endregion

        private GameObject m_innerGameObject = null;
        public GameObject gameObject
        {
            get
            {
                if(m_innerGameObject == null)
                {
                    IntPtr gameObjectNative = ComponentGetGameObject(m_nativePtr);
                    m_innerGameObject = GameObject.CreateFromNative(gameObjectNative);
                }

                return m_innerGameObject;
            }
        }


        #region Native
        public enum ScriptableFuncType
        {
            Awake,
            Start,
            Update,
            LateUpdate,
            OnEnable,
            OnDisable
        };

        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void ComponentRegisterCallScriptable(IntPtr native, ScriptableFuncType type, Action cbk);

        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern IntPtr ComponentGetGameObject(IntPtr native);

        #endregion
    }
}
