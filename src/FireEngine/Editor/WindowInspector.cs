using System;
using System.Collections.Generic;
using System.Text;

namespace FireEngine.Editor
{
    class WindowInspector : WindowBase
    {
        public override void Init()
        {
            Show();
        }

        public override bool isInWIndowList => true;

        public override bool canDock => true;

        public override string title => "Inspector";

        public override void OnGUI()
        {
        }
    }
}
