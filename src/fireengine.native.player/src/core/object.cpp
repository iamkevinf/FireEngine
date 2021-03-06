#include "object.h"

#include "transform.h"

#include <memory>
#include <iostream>

namespace FireEngine
{
	std::map<uint16_t, ObjectWeakPtr> ObjectManager::objPool;
	uint16_t ObjectManager::s_cur = 1;

	ObjectHandle ObjectManager::Register(ObjectPtr obj, ObjectType type)
	{
		uint16_t id = ObjectManager::s_cur++;
		obj->objectID = id;
		obj->type = type;
		ObjectManager::objPool[id] = obj;
		return {id};
	}

	void ObjectManager::UnRegister(ObjectHandle handle)
	{
		ObjectManager::objPool.erase(ObjectManager::objPool.find(handle.idx));
	}

	ObjectPtr ObjectManager::GetRef(uint16_t objectId)
	{
		return ObjectManager::objPool[objectId].lock();
	}

	ObjectPtr ObjectManager::Get(ObjectHandle handle)
	{
		return ObjectManager::objPool[handle.idx].lock();
	}

	TransformPtr ObjectManager::Get(TransformHandle handle)
	{
		ObjectPtr objPtr = ObjectManager::objPool[handle.idx].lock();
		TransformPtr ret = std::static_pointer_cast<Transform>(objPtr);
		return ret;
	}
}