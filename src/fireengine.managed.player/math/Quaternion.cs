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
				return Quaternion.identityQuaternion;
			}
		}

		public Vector3 eulerAngles
		{
			get
			{
				return Quaternion.Internal_MakePositive(Quaternion.Internal_ToEulerRad(this) * 57.29578f);
			}
			set
			{
				this = Quaternion.Internal_FromEulerRad(value * 0.0174532924f);
			}
		}

		public Quaternion normalized
		{
			get
			{
				return Quaternion.Normalize(this);
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
			float num = -0.005729578f;
			float num2 = 360f + num;
			if (euler.X < num)
			{
				euler.X += 360f;
			}
			else if (euler.X > num2)
			{
				euler.X -= 360f;
			}
			if (euler.Y < num)
			{
				euler.Y += 360f;
			}
			else if (euler.Y > num2)
			{
				euler.Y -= 360f;
			}
			if (euler.Z < num)
			{
				euler.Z += 360f;
			}
			else if (euler.Z > num2)
			{
				euler.Z -= 360f;
			}
			return euler;
		}
		private static Vector3 Internal_ToEulerRad(Quaternion rotation)
		{
			Vector3 result = Vector3.Zero;
			Quaternion.Internal_ToEulerRad_Injected(rotation, ref result);
			return result;
		}
		private static Quaternion Internal_FromEulerRad(Vector3 euler)
		{
			Quaternion result = Quaternion.identity;
			Quaternion.Internal_FromEulerRad_Injected(euler, ref result);
			return result;
		}
		public static Quaternion Normalize(Quaternion q)
		{
			float v = Quaternion.Dot(q, q);
			float num = (float)Math.Sqrt(v);
			Quaternion result;
			if (num < Epsilon)
			{
				result = Quaternion.identity;
			}
			else
			{
				result = new Quaternion(q.x / num, q.y / num, q.z / num, q.w / num);
			}
			return result;
		}

		public void Normalize()
		{
			this = Quaternion.Normalize(this);
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
			return Quaternion.IsEqualUsingDot(Quaternion.Dot(lhs, rhs));
		}

		public static bool operator !=(Quaternion lhs, Quaternion rhs)
		{
			return !(lhs == rhs);
		}


		[DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
		public static extern void Internal_ToEulerRad_Injected(Quaternion euler, ref Vector3 ret);
		[DllImport(FireEngineNative.FireEngineDllName, CallingConvention = CallingConvention.Cdecl)]
		public static extern void Internal_FromEulerRad_Injected(Vector3 euler, ref Quaternion ret);
	}
}
