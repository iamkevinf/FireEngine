using System;

namespace FireEngine
{
    public static class Mathf
    {
        public const float PI = (float)Math.PI;

        // A representation of positive infinity (RO).
        public const float Infinity = Single.PositiveInfinity;

        // A representation of negative infinity (RO).
        public const float NegativeInfinity = Single.NegativeInfinity;

        // Degrees-to-radians conversion constant (RO).
        public const float Deg2Rad = PI * 2F / 360F;

        // Radians-to-degrees conversion constant (RO).
        public const float Rad2Deg = 1F / Deg2Rad;

        public static int Max(int a, int b)
        {
            return Math.Max(a, b);
        }

    }
}
