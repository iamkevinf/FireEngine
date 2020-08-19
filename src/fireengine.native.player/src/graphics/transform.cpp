#include "transform.h"

#include "../exportapi.h"
#include "../scene/scene.h"

namespace FireEngine
{
	EXPORT_API TransformHandle TransformCreate(TransformHandle parent, const char16_t* name)
	{
		TransformPtr parentPtr = ObjectManager::Get(parent);

		Transform* transform = new Transform();
		transform->name = name;
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
		Transform* pTo = (Transform*)toObjPtr.get();

		Transform* pFrom = pWho->parent.lock().get();
		if (pFrom != pTo)
		{
			TransformPtr whoPtr = nullptr;
			for (auto iter = pFrom->children.begin(); iter != pFrom->children.end(); ++iter)
			{
				if ((*iter).get() == pWho)
				{
					whoPtr = *iter;
					pFrom->children.erase(iter);
					break;
				}
			}

			if (whoPtr)
			{
				pWho->parent = toObjPtr;
				pTo->children.push_back(whoPtr);
			}
		}
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
		pTransform->name = name;
	}

	EXPORT_API const char16_t* TransformGetName(TransformHandle handle)
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
		pTransform->worldMatrix = mat;
	}

	EXPORT_API void TransformGetMatrix(TransformHandle handle, glm::mat4& mat)
	{
		auto object = ObjectManager::Get(handle);
		Transform* pTransform = (Transform*)object.get();
		mat = pTransform->worldMatrix;
	}

}