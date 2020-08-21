#include "transform.h"

#include "core/gameobject.h"

#include "exportapi.h"
#include "scene/scene.h"

#include <glm/gtc/quaternion.hpp>

namespace FireEngine
{
	DEFINE_COM_CLASS(Transform);


	void Transform::SetParent(const TransformWeakPtr& _parent)
	{
		auto obj = GetGameObject();

		Transform* pFrom = this->parent.lock().get();
		if (pFrom != _parent.lock().get())
		{
			TransformPtr whoPtr = nullptr;
			for (auto iter = pFrom->children.begin(); iter != pFrom->children.end(); ++iter)
			{
				if ((*iter).get() == this)
				{
					whoPtr = *iter;
					pFrom->children.erase(iter);
					break;
				}
			}

			if (whoPtr)
			{
				this->parent = _parent;
				_parent.lock()->children.push_back(whoPtr);
			}
		}
	}


	std::shared_ptr<Transform> Transform::GetChild(uint32_t index)const
	{
		if (index > children.size() - 1)
		{
			return std::shared_ptr<Transform>();
		}

		return children[index];
	}

	void Transform::DeepCopy(const ObjectPtr& source)
	{
		Component::DeepCopy(source);

		auto src = std::dynamic_pointer_cast<Transform>(source);

		for (auto i = 0; i < src->children.size(); i++)
		{
			auto src_child = src->children[i];
			auto child = GameObject::Instantiate(src_child->GetGameObject());

			child->GetTransform()->SetParent(std::dynamic_pointer_cast<Transform>(GetRef()));
		}
	}

	void Transform::SetLocalPosition(const glm::vec3& pos)
	{
		if (local_position != pos)
		{
			local_position = pos;
			DeltyTransform();
			NotifyDelty();
		}
	}

	void Transform::SetLocalRotation(const glm::quat& rotation)
	{
		glm::quat r = rotation;
		glm::normalize(r);

		if (local_rotation != r)
		{
			local_rotation = r;
			DeltyTransform();
			NotifyDelty();
		}
	}

	void Transform::SetLocalScale(const glm::vec3& scale)
	{
		if (local_scale != scale)
		{
			local_scale = scale;
			DeltyTransform();
			NotifyDelty();
		}
	}

	void Transform::SetWorldPosition(const glm::vec3& pos)
	{
		if (!delty && world_position == pos)
			return;

		if (IsRoot())
		{
			SetLocalPosition(pos);
		}
		else
		{
			glm::vec3 local;
			parent.lock()->World2Local(pos, local);
			SetLocalPosition(local);
		}
	}

	void Transform::SetWorldRotation(const glm::quat& rotation)
	{
		if (!delty && local_rotation == rotation)
			return;

		if (IsRoot())
		{
			SetLocalRotation(rotation);
		}
		else
		{
			glm::quat local = glm::inverse(parent.lock()->GetWorldRotation()) * rotation;
			SetLocalRotation(local);
		}
	}

	void Transform::SetWorldScale(const glm::vec3& scale)
	{
		if (!delty && local_scale == scale)
			return;

		if (IsRoot())
		{
			SetLocalPosition(scale);
		}
		else
		{
			const glm::vec3& parent_scale = parent.lock()->GetWorldScale();
			float x = scale.x / parent_scale.x;
			float y = scale.y / parent_scale.y;
			float z = scale.z / parent_scale.z;
			SetLocalScale(glm::vec3(x, y, z));
		}
	}

	const glm::vec3& Transform::GetWorldPosition()
	{
		ApplyDelta();

		return world_position;
	}

	const glm::quat& Transform::GetWorldRotation()
	{
		ApplyDelta();

		return world_rotation;
	}

	const glm::vec3& Transform::GetWorldScale()
	{
		ApplyDelta();

		return world_scale;
	}


	void Transform::DeltyTransform()
	{
		delty = true;

		for (TransformWeakPtr child : children)
		{
			child.lock()->DeltyTransform();
		}
	}

	void Transform::ApplyDelta()
	{
		if (!delty)
			return;

		delty = false;

		if (IsRoot())
		{
			world_position = local_position;
			world_rotation = local_rotation;
			world_scale = local_scale;
		}
		else
		{
			parent.lock()->Local2World(local_position, world_position);
			world_rotation = parent.lock()->GetWorldRotation() * local_rotation;

			world_scale = parent.lock()->GetWorldScale() * local_scale;
		}

		glm::mat4 t(1);
		glm::mat4 r(1);
		glm::mat4 s(1);

		t = glm::translate(t, world_position);
		r = glm::mat4_cast(world_rotation);
		s = glm::scale(s, world_scale);

		glm::mat4 trs = t * r * s;

		local_to_world_matrix = trs;
	}

	void Transform::NotifyDelty()
	{
		delty_notifying = true;
		GetGameObject()->OnTransformChanged();

		for (auto& child : children)
			child->NotifyDelty();

		delty_notifying = false;
	}


	const glm::mat4& Transform::GetWorld2LocalMatrix()
	{
		glm::mat4 tmp = GetLocal2WorldMatrix();
		tmp = glm::inverse(tmp);
		world_to_local_matrix = tmp;

		return world_to_local_matrix;
	}

	const glm::mat4& Transform::GetLocal2WorldMatrix()
	{
		ApplyDelta();

		return local_to_world_matrix;
	}

	void Transform::World2Local(const glm::vec3& world, glm::vec3& local)
	{
		glm::vec4 world4 = glm::vec4(world, 1);
		local = world4 * GetWorld2LocalMatrix();
	}

	void Transform::Local2World(const glm::vec3& local, glm::vec3& world)
	{

		glm::vec4 local4 = glm::vec4(local, 1);
		world = local4 * GetLocal2WorldMatrix();
	}

	glm::vec3 Transform::GetRight()
	{
		return GetWorldRotation() * glm::vec3(1, 0, 0);
	}

	glm::vec3 Transform::GetUp()
	{
		return GetWorldRotation() * glm::vec3(0, 1, 0);
	}

	glm::vec3 Transform::GetForward()
	{
		return GetWorldRotation() * glm::vec3(0, 0, 1);
	}

	EXPORT_API TransformHandle TransformCreate(TransformHandle parent, const char16_t* name)
	{
		TransformPtr parentPtr = ObjectManager::Get(parent);

		Transform* transform = new Transform();
		transform->name = toUTF8(std::u16string(name));
		TransformPtr transformPtr = TransformPtr(transform);
		parentPtr->children.push_back(transformPtr);

		transform->parent = parentPtr;

		ObjectManager::Register(transformPtr, ObjectType::Transform);

		return { transform->objectID };
	}

	EXPORT_API void TransformRemove(TransformHandle handle)
	{
		auto object = ObjectManager::Get(handle);
		Transform* pTransform = (Transform*)object.get();

		auto parent = pTransform->parent.lock();
		for (auto iter = parent->children.begin(); iter != parent->children.end(); ++iter)
		{
			if (*iter == object)
			{
				parent->children.erase(iter);
				break;
			}
		}
	}

	EXPORT_API int TransformChildCount(TransformHandle handle)
	{
		auto object = ObjectManager::Get(handle);
		Transform* pTransform = (Transform*)object.get();
		return (int)pTransform->children.size();
	}

	EXPORT_API void TransformMove(TransformHandle who, TransformHandle to)
	{
		auto whoObjPtr = ObjectManager::Get(who);
		Transform* pWho = (Transform*)whoObjPtr.get();

		auto toObjPtr = ObjectManager::Get(to);

		pWho->SetParent(toObjPtr);
	}

	EXPORT_API TransformHandle TransformFindChildByIndex(TransformHandle handle, int index)
	{
		auto object = ObjectManager::Get(handle);
		Transform* pTransform = (Transform*)object.get();

		auto child = pTransform->children[index];
		return { child->objectID };
	}

	EXPORT_API void TransformSetName(TransformHandle handle, const char16_t* name)
	{
		auto object = ObjectManager::Get(handle);
		Transform* pTransform = (Transform*)object.get();
		pTransform->name = toUTF8(std::u16string(name));
	}

	EXPORT_API const char* TransformGetName(TransformHandle handle)
	{
		auto object = ObjectManager::Get(handle);
		Transform* pTransform = (Transform*)object.get();
		return pTransform->name.c_str();
	}


	EXPORT_API void TransformSetActive(TransformHandle handle, ActiveOption active)
	{
		auto object = ObjectManager::Get(handle);
		Transform* pTransform = (Transform*)object.get();
		pTransform->active = active;

	}

	EXPORT_API ActiveOption TransformGetActive(TransformHandle handle)
	{
		auto object = ObjectManager::Get(handle);
		Transform* pTransform = (Transform*)object.get();
		return pTransform->active;
	}


	EXPORT_API void TransformSetMatrix(TransformHandle handle, glm::mat4 mat)
	{
		auto object = ObjectManager::Get(handle);
		Transform* pTransform = (Transform*)object.get();
		pTransform->world_to_local_matrix = mat;
	}

	EXPORT_API void TransformGetMatrix(TransformHandle handle, glm::mat4& mat)
	{
		auto object = ObjectManager::Get(handle);
		Transform* pTransform = (Transform*)object.get();
		mat = pTransform->world_to_local_matrix;
	}

	EXPORT_API void TransformSetWorldPosition(TransformHandle handle, glm::vec3 pos)
	{
		auto object = ObjectManager::Get(handle);
		Transform* pTransform = (Transform*)object.get();
		pTransform->SetWorldPosition(pos);
	}

	EXPORT_API void TransformGetWorldPosition(TransformHandle handle, glm::vec3* pos)
	{
		auto object = ObjectManager::Get(handle);
		Transform* pTransform = (Transform*)object.get();
		*pos = pTransform->GetWorldPosition();
	}


}