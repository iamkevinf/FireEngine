#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <stdint.h>
#include <map>
#include <memory>

#include "../FireEngine.h"

namespace FireEngine
{
	enum class ObjectType
	{
		Unknow,
		Scene,
		Transform,
		Max,
	};

	class IObject
	{
	public:
		uint16_t objectID;
		ObjectType type;
	};

	typedef std::shared_ptr<IObject> ObjectPtr;
	typedef std::weak_ptr<IObject> ObjectWeakPtr;

	class ObjectManager
	{
	public:
		static ObjectHandle Register(ObjectPtr obj, ObjectType type);
		static void UnRegister(ObjectHandle handle);
		static ObjectPtr Get(ObjectHandle handle);

	public:
		static std::map<uint16_t, ObjectWeakPtr> objPool;
		static uint16_t s_cur;
	};
}

#endif // __OBJECT_H__