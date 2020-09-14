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
        Renderer,
        // assert
        Material,
        Shader,
    }

    public interface iInspector
    {
        InspectorType type
        {
            get;
        }

        void OnGUIInspector(object userData);
        void OnGUIInspectorEnd(object userData);
    }


    public static class InspectorManager
    {
        static Dictionary<InspectorType, iInspector> pool = new Dictionary<InspectorType, iInspector>();

        public static void RegisterInspector(iInspector inspector)
        {
            if (pool.ContainsKey(inspector.type))
                return;
            pool.Add(inspector.type, inspector);
        }
    }
}
