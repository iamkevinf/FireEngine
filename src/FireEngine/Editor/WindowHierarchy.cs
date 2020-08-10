using System;
using System.Collections.Generic;
using System.Text;

namespace FireEngine.Editor
{
    class WindowHierarchy : WindowBase
    {
        public override string title => "Hierachy";
        public override bool isInWIndowList => true;
        public override bool canDock => true;

        public override void Init()
        {
            Show();
        }
    }
}
