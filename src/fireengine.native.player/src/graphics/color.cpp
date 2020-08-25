#include "color.h"

#define F32_TO_INT8_SAT(_VAL)        ((int)(clamp01(_VAL) * 255.0f + 0.5f))
#define COL32_R_SHIFT    24
#define COL32_G_SHIFT    16
#define COL32_B_SHIFT    8
#define COL32_A_SHIFT    0

namespace FireEngine
{
	Color::Color(float r, float g, float b, float a) :
		r(r), g(g), b(b), a(a)
	{
	}

	uint32_t Color::GetHex()
	{
		uint32_t out;
		out  = ((uint32_t)F32_TO_INT8_SAT(r)) << COL32_R_SHIFT;
		out |= ((uint32_t)F32_TO_INT8_SAT(g)) << COL32_G_SHIFT;
		out |= ((uint32_t)F32_TO_INT8_SAT(b)) << COL32_B_SHIFT;
		out |= ((uint32_t)F32_TO_INT8_SAT(a)) << COL32_A_SHIFT;
		return out;
	}

	bool Color::operator ==(const Color& c) const
	{
		return
			almostEqual(r, c.r) &&
			almostEqual(g, c.g) &&
			almostEqual(b, c.b) &&
			almostEqual(a, c.a);
	}

	bool Color::operator !=(const Color& c) const
	{
		return !(*this == c);
	}

	Color Color::Lerp(const Color& from, const Color& to, float t, bool clamp_01)
	{
		if (clamp_01)
			t = clamp01(t);

		return Color(
			lerp(from.r, to.r, t),
			lerp(from.g, to.g, t),
			lerp(from.b, to.b, t),
			lerp(from.a, to.a, t));
	}

	Color Color::operator *(const Color& c) const
	{
		return Color(r * c.r, g * c.g, b * c.b, a * c.a);
	}

	Color& Color::operator *=(const Color& c)
	{
		r *= c.r;
		g *= c.g;
		b *= c.b;
		a *= c.a;
		return *this;
	}

	Color Color::operator *(float v) const
	{
		return Color(r * v, g * v, b * v, a * v);
	}

	Color Color::operator /(float v) const
	{
		return *this * (1 / v);
	}
}