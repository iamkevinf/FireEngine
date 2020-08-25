#ifndef __COLOR_H__
#define __COLOR_H__

#include "math/math.h"

namespace FireEngine
{
	struct Color
	{
	public:
		static Color Lerp(const Color& from, const Color& to, float t, bool clamp_01 = true);

		Color(float r = 0, float g = 0, float b = 0, float a = 0);
		uint32_t GetHex();

		bool operator ==(const Color& c) const;
		bool operator !=(const Color& c) const;
		Color operator *(const Color& c) const;
		Color& operator *=(const Color& c);
		Color operator *(float v) const;
		Color operator /(float v) const;

		float r;
		float g;
		float b;
		float a;
	};
}

#endif // __COLOR_H__