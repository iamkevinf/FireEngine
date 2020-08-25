using System;
using System.Collections.Generic;
using System.Text;

namespace FireEngine
{
    public class Object
    {
        public string name { set; get; }

        public override string ToString()
        {
            return name;
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public override bool Equals(object other)
        {
            Object @object = other as Object;
            return (!(@object == null) || other == null || other is Object);
        }

        public static bool operator ==(Object x, Object y)
        {
            return x == y;
        }

        public static bool operator !=(Object x, Object y)
        {
            return x != y;
        }

        public static implicit operator bool(Object exists)
        {
            return exists != null;
        }
    }
}
