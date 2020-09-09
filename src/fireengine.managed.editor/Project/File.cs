using System;
using System.Collections.Generic;
using System.Text;

namespace FireEditor
{
    public class File : iSelectable
    {
        public string name
        {
            get;
            set;
        }
        public string fullname
        {
            get;
            set;
        }
    }

    public class Path : iSelectable
    {
        public string name
        {
            get;
            set;
        }

        public Path[] paths;
        public File[] files;
    }
}
