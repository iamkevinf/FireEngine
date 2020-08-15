#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "../object/object.h"

#include "../scene/scene.h"

#include <glm/glm.hpp>
#include <string>
#include <memory>

namespace FireEngine
{
	typedef std::shared_ptr<class Transform> TransformPtr;
	typedef std::weak_ptr<class Transform> TransformWeakPtr;

	FIREENGINE_HANDLE(TransformHandle);

	class Transform : public IObject
	{
	public:
		std::u16string name;
		ActiveOption active = ActiveOption::Active;
		glm::mat4 worldMatrix;
		TransformWeakPtr parent;
		std::vector<TransformPtr> children;
	};
}

#endif // __TRANSFORM_H__
