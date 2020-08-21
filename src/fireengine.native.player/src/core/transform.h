#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "scene/scene.h"
#include "core/component.h"

#include <glm/glm.hpp>
#include <string>
#include <memory>

namespace FireEngine
{
	typedef std::shared_ptr<class Transform> TransformPtr;
	typedef std::weak_ptr<class Transform> TransformWeakPtr;

	FIREENGINE_HANDLE(TransformHandle);

	class Transform : public Component
	{
		DECLARE_COM_CLASS(Transform, Component);

	private:
		friend class GameObject;

	public:
		TransformWeakPtr GetParent()const { return parent; }
		void SetParent(const TransformWeakPtr& parent);
		bool IsRoot()const { return parent.expired(); }

		uint32_t GetChildCount() { return (uint32_t)children.size(); }
		TransformPtr GetChild(uint32_t index)const;

	public:
		ActiveOption active = ActiveOption::Active;
		glm::mat4 worldMatrix;
		TransformWeakPtr parent;
		std::vector<TransformPtr> children;
	};
}

#endif // __TRANSFORM_H__
