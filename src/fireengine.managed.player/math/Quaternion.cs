using System;
using System.Collections.Generic;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;

namespace FireEngine
{
    public struct Quaternion
    {
        public float x;
        public float y;
        public float z;
        public float w;

        public static volatile float FloatMinNormal = 1.17549435E-38f;

        public static volatile float FloatMinDenormal = 1.401298E-45f;

        public static bool IsFlushToZeroEnabled = FloatMinDenormal == 0f;

        public static readonly float Epsilon = (!IsFlushToZeroEnabled) ? FloatMinDenormal : FloatMinNormal;


        private static readonly Quaternion identityQuaternion = new Quaternion(0f, 0f, 0f, 1f);

        public float this[int index]
        {
            get
            {
                float result;
                switch (index)
                {
                    case 0:
                        result = this.x;
                        break;
                    case 1:
                        result = this.y;
                        break;
                    case 2:
                        result = this.z;
                        break;
                    case 3:
                        result = this.w;
                        break;
                    default:
                        throw new IndexOutOfRangeException("Invalid Quaternion index!");
                }
                return result;
            }
            set
            {
                switch (index)
                {
                    case 0:
                        this.x = value;
                        break;
                    case 1:
                        this.y = value;
                        break;
                    case 2:
                        this.z = value;
                        break;
                    case 3:
                        this.w = value;
                        break;
                    default:
                        throw new IndexOutOfRangeException("Invalid Quaternion index!");
                }
            }
        }


        public static Quaternion identity
        {
            get
            {
                return identityQuaternion;
            }
        }

        public Vector3 eulerAngles
        {
            get
            {
                return Internal_MakePositive(Internal_ToEulerRad(this) * Mathf.Rad2Deg);
            }
            set
            {
                this = Internal_FromEulerRad(value);
            }
        }

        public Quaternion normalized
        {
            get
            {
                return Normalize(this);
            }
        }

        public Quaternion(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }


        private static Vector3 Internal_MakePositive(Vector3 euler)
        {
            float negativeFlip = -0.0001f * Mathf.Rad2Deg;
            float positiveFlip = 360.0f + negativeFlip;

            if (euler.X < negativeFlip)
                euler.X += 360.0f;
            else if (euler.X > positiveFlip)
                euler.X -= 360.0f;

            if (euler.Y < negativeFlip)
                euler.Y += 360.0f;
            else if (euler.Y > positiveFlip)
                euler.Y -= 360.0f;

            if (euler.Z < negativeFlip)
                euler.Z += 360.0f;
            else if (euler.Z > positiveFlip)
                euler.Z -= 360.0f;

            return euler;
        }


        private static Vector3 Internal_ToEulerRad(Quaternion rotation)
        {
            rotation.Normalize();
            Vector3 result = QuaternionToEuler(rotation);
            return result;
            //Vector3 ret = Vector3.Zero;
            //Internal_ToEulerRad_Injected(rotation, ref ret);
            //return ret;
        }
        private static Quaternion Internal_FromEulerRad(Vector3 euler)
        {
            Quaternion result = EulerToQuaternion(euler);
            result.Normalize();
            return result;
            //Quaternion ret = identity;
            //Internal_FromEulerRad_Injected(euler, ref ret);
            //return ret;
        }
        public static Quaternion Normalize(Quaternion q)
        {
            float v = Dot(q, q);
            float num = (float)Math.Sqrt(v);
            Quaternion result;
            if (num < Epsilon)
            {
                result = identity;
            }
            else
            {
                result = new Quaternion(q.x / num, q.y / num, q.z / num, q.w / num);
            }
            return result;
        }

        public void Normalize()
        {
            this = Normalize(this);
        }
        public static float Dot(Quaternion a, Quaternion b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        }


        private static bool IsEqualUsingDot(float dot)
        {
            return dot > 0.999999f;
        }

        public static bool operator ==(Quaternion lhs, Quaternion rhs)
        {
            return IsEqualUsingDot(Dot(lhs, rhs));
        }

        public static bool operator !=(Quaternion lhs, Quaternion rhs)
        {
            return !(lhs == rhs);
        }


        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Internal_ToEulerRad_Injected(Quaternion euler, ref Vector3 ret);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Internal_FromEulerRad_Injected(Vector3 euler, ref Quaternion ret);

        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern Vector3 QuaternionToEuler(Quaternion q);
        [DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern Quaternion EulerToQuaternion(Vector3 v);
    }
}
