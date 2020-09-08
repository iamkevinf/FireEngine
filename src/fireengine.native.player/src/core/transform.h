#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "scene/scene.h"
#include "core/component.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
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

		void DeltyTransform();
		void SetLocalPosition(const glm::vec3& pos);
		void SetLocalRotation(const glm::quat& rotation);
		void SetLocalScale(const glm::vec3& scale);

		void SetWorldPosition(const glm::vec3& pos);
		void SetWorldRotation(const glm::quat& rotation);
		void SetWorldScale(const glm::vec3& scale);


		const glm::vec3& GetWorldPosition();
		const glm::quat& GetWorldRotation();
		const glm::vec3& GetWorldScale();

		void World2Local(const glm::vec3& world, glm::vec3& local);
		void Local2World(const glm::vec3& local, glm::vec3& world);

		const glm::mat4& GetWorld2LocalMatrix();
		const glm::mat4& GetLocal2WorldMatrix();

		glm::vec3 GetRight();
		glm::vec3 GetUp();
		glm::vec3 GetForward();

	private:
		void ApplyDelta();;
		void NotifyDelty();

	public:
		TransformWeakPtr parent;
		std::vector<TransformPtr> children;

		ActiveOption active = ActiveOption::Active;
		glm::mat4 local_to_world_matrix = glm::identity<glm::mat4>();
		glm::mat4 world_to_local_matrix = glm::identity<glm::mat4>();

		glm::vec3  local_position = glm::vec3(0, 0, 0);
		glm::quat  local_rotation = glm::quat(1, 0, 0, 0);
		glm::vec3  local_scale = glm::vec3(1, 1, 1);

		glm::vec3 world_position = glm::vec3(0, 0, 0);
		glm::quat world_rotation = glm::quat(1, 0, 0, 0);
		glm::vec3 world_scale = glm::vec3(1, 1, 1);

		bool delty = true;
		bool delty_notifying = false;

	};
}

#endif // __TRANSFORM_H__
