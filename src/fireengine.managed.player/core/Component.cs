using ImGuiNET;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
    public class Component : Object
    {
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

            //ComponentRegisterCallScriptable(native, ScriptableFuncType.Awake, cbkAwake);
            //ComponentRegisterCallScriptable(native, ScriptableFuncType.Start, cbkStart);
            //ComponentRegisterCallScriptable(native, ScriptableFuncType.Update, cbkUpdate);
            //ComponentRegisterCallScriptable(native, ScriptableFuncType.LateUpdate, cbkLateUpdate);
            //ComponentRegisterCallScriptable(native, ScriptableFuncType.OnEnable, cbkOnEnable);
            //ComponentRegisterCallScriptable(native, ScriptableFuncType.OnDisable, cbkDisable);
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
