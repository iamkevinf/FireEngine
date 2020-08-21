#ifndef __WORLD_H__
#define __WORLD_H__

#include "gameobject.h"
#include <mutex>

namespace FireEngine
{
	class World
	{
	public:
		static void AddGameObject(const GameObjectPtr& obj);
		static void AddGameObjects(const std::list<GameObjectPtr>& objs);

		static bool Init();
		static void Fini();
		static void Tick();

	private:
		static std::list<GameObjectPtr> gameObjects;
		static std::list<GameObjectPtr> gameObjects_start;
		static std::mutex mutex;
	};
}

#endif // __WORLD_H__