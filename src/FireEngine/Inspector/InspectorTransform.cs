using System;
using System.Collections.Generic;
using System.Text;
using FireEditor;
using FireEngine;

namespace FireEngine.Editor
{
    public class InspectorTransform : InspectorComponent
    {
        public override InspectorType type
        {
            get
            {
                return InspectorType.Transform;
            }
        }

    }
}
