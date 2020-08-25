#ifndef __CAMERA_CLEAR_FLAGS_H__
#define __CAMERA_CLEAR_FLAGS_H__

namespace FireEngine
{
	enum class CameraClearFlags
	{
		Invalidate = 1,
		Color = 2,
		Depth = 3,
		Nothing = 4,
	};
}

#endif // __CAMERA_CLEAR_FLAGS_H__