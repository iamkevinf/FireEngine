using System;
using System.Collections.Generic;
using System.Text;

namespace FireEditor
{
    public enum InspectorType
    {
        // 
        Component,
        Transform,
        Camera,
        // assert
        Material,
        Shader,
    }

    public interface IInspector
    {
        InspectorType type
        {
            get;
        }

        void OnGUIInspector(object userData);
    }


    public static class InspectorManager
    {
        static Dictionary<InspectorType, IInspector> pool = new Dictionary<InspectorType, IInspector>();

        public static void RegisterInspector(IInspector inspector)
        {
            if (pool.ContainsKey(inspector.type))
                return;
            pool.Add(inspector.type, inspector);
        }
    }
}
