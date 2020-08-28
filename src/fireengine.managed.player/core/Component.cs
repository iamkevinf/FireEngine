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
        Vector3 rot = Vector3.Zero;
        Vector3 scl = Vector3.One;

        protected IntPtr m_nativePtr = IntPtr.Zero;
        private Component()
        {

        }

        private ScriptableCallback cbkAwake = null;
        private ScriptableCallback cbkStart = null;
        private ScriptableCallback cbkUpdate = null;
        private ScriptableCallback cbkLateUpdate = null;
        private ScriptableCallback cbkOnEnable = null;
        private ScriptableCallback cbkDisable = null;

        public Component(IntPtr native) : this()
        {
            m_nativePtr = native;

            cbkAwake = new ScriptableCallback(Awake);
            cbkStart = new ScriptableCallback(Start);
            cbkUpdate = new ScriptableCallback(Update);
            cbkLateUpdate = new ScriptableCallback(LateUpdate);
            cbkOnEnable = new ScriptableCallback(OnEnable);
            cbkDisable = new ScriptableCallback(OnDisable);

            ComponentRegisterCallScriptable(native, ScriptableFuncType.Awake, cbkAwake);
            ComponentRegisterCallScriptable(native, ScriptableFuncType.Start, cbkStart);
            ComponentRegisterCallScriptable(native, ScriptableFuncType.Update, cbkUpdate);
            ComponentRegisterCallScriptable(native, ScriptableFuncType.LateUpdate, cbkLateUpdate);
            ComponentRegisterCallScriptable(native, ScriptableFuncType.OnEnable, cbkOnEnable);
            ComponentRegisterCallScriptable(native, ScriptableFuncType.OnDisable, cbkDisable);
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
                rot = gameObject.transform.rotation.eulerAngles;
                ImGui.DragFloat3("##Rotation#Component##Inspector", ref rot); ImGui.SameLine();
                Quaternion quaternion = Quaternion.identity;
                quaternion.eulerAngles = rot;
                gameObject.transform.rotation = quaternion;
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

        public Transform transform
        {
            get
            {
                return gameObject?.transform;
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
        public delegate void ScriptableCallback();

        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void ComponentRegisterCallScriptable(IntPtr native, ScriptableFuncType type, ScriptableCallback cbk);

        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern IntPtr ComponentGetGameObject(IntPtr native);

        #endregion
    }
}
