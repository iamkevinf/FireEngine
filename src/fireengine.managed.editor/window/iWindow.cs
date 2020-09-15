using System;
using System.Collections.Generic;
using System.Text;

namespace FireEditor
{
    public interface iWindow
    {
        void Init();
        void OnGUI();
        void OnTick();
        void OnShow();
        void OnHide();
        void OnDestroy();
    }

    public interface iWindowAttribute
    {
        iWindow window
        {
            get;
        }
        uint UUID
        {
            get;
        }
        bool visible
        {
            get;
        }
        bool register2menu
        {
            get;
        }
        bool canDock
        {
            get;
        }
        string title
        {
            get;
        }
    }

}
