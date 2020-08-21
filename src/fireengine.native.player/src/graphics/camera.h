#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "core/object.h"

namespace FireEngine
{
	typedef std::shared_ptr<class Camera> CameraPtr;
	typedef std::weak_ptr<class Camera> CameraWeakPtr;

	class Camera : public IObject
	{

	};
}

#endif // __CAMERA_H__