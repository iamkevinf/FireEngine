using System;
using System.Collections.Generic;
using System.Text;

namespace FireEngine.Editor
{
    class WindowBase : iWindow
    {
        public WindowBase()
        {
            Init();
        }

        public bool visible
        {
            get;
            protected set;
        }
        public virtual bool isInWIndowList => false;

        public virtual bool canDock => false;

        public virtual string title => "";

        public virtual void Init() { }

        public virtual void OnHide() { }

        public virtual void Hide()
        {
            if (!visible)
                return;

            OnHide();

            visible = false;
        }

        public virtual void OnGUI()
        {
        }

        public virtual void OnShow() { }
        public virtual void Show()
        {
            if (visible)
                return;

            OnShow();

            visible = true;
        }
    }
}
