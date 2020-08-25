#ifndef __MATH_H__
#define __MATH_H__

#include <cmath>
#include <cstdint>
#include <iostream>

const float MAX_FLOAT_VALUE = 3.402823466e+38F;
const float MIN_FLOAT_VALUE = -MAX_FLOAT_VALUE;

const float Deg2Rad = 0.0174533f;
const float Rad2Deg = 57.2958f;

#define MATH_PI 3.14159265f

#ifndef PI_FULL
#define PI_FULL MATH_PI
#endif // PI_FULL

#ifndef PI_HALF
#define PI_HALF PI_FULL * 0.5f
#endif // PI_HALF

#ifndef PI_QUAR
#define PI_QUAR PI_HALF * 0.5f
#endif // PI_QUAR

namespace FireEngine
{

    const float FloatMax = 1e30f;
    const float FloatMin = -FloatMax;
    const float DefaultEpsilon = 0.000001f;


    template<typename T>
    inline const T& min2(const T& a, const T& b)
    {
        return a < b ? a : b;
    }

    template<typename T>
    inline const T& min3(const T& a, const T& b, const T& c)
    {
        return (a < b) ? min2(a, c) : min2(b, c);
    }

    template<typename T>
    inline const T& max2(const T& a, const T& b)
    {
        return a > b ? a : b;
    }

    template<typename T>
    inline const T& max3(const T& a, const T& b, const T& c)
    {
        return (a > b) ? max2(a, c) : max2(b, c);
    }

    inline bool almostZero(float d, float epsilon = DefaultEpsilon)
    {
        return d > -epsilon && d < epsilon;
    }

    inline bool almostEqual(float a, float b, float epsilon = DefaultEpsilon)
    {
        return fabs(a - b) < epsilon;
    }

    template<typename T>
    const T& clamp(const T& low, const T& v, const T& hig)
    {
        if (v < low) return low;
        else if (v > hig) return hig;

        return v;
    }

    inline float clamp01(float value)
    {
        return min2<float>(max2<float>(value, 0), 1);
    }

    inline float lerp(float from, float to, float t, bool clamp_01 = true)
    {
        if (clamp_01)
            t = clamp01(t);

        return from + (to - from) * t;
    }


    inline float log2(float x) { return logf(x) / logf(2); }

    template<typename T>
    void limit_in_range(T& v, const T& low, const T& high)
    {
        if (v < low) v = low;
        else if (v > high) v = high;
    }

    template<typename T>
    void limit_in_order(T& a, T& b)
    {
        if (a > b) std::swap(a, b);
    }
}

#endif // __MATH_H__