#ifndef __RECT_H__
#define __RECT_H__

#include "math/math.h"

namespace FireEngine
{
	class Rect
	{
	public:
		explicit Rect(float x = 0, float y = 0, float width = 0, float height = 0) :
			x(x),
			y(y),
			width(width),
			height(height)
		{
		}

		void Set(float x, float y, float width, float height)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}

		bool operator ==(const Rect& r) const
		{
			return almostEqual(x, r.x) &&
				almostEqual(y, r.y) &&
				almostEqual(width, r.width) &&
				almostEqual(height, r.height);
		}

		bool operator !=(const Rect& r) const
		{
			return !(*this == r);
		}

		float x;
		float y;
		float width;
		float height;

	};
}


#endif //__KGE_MATH_RECT_H__