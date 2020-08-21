#include "world.h"

#include "graphics/camera.h"

namespace FireEngine
{
	std::list<GameObjectPtr> World::gameObjects;
	std::list<GameObjectPtr> World::gameObjects_start;
	std::mutex World::mutex;

	void World::AddGameObject(const GameObjectPtr& obj)
	{
		mutex.lock();
		gameObjects_start.push_back(obj);
		mutex.unlock();
	}

	void World::AddGameObjects(const std::list<GameObjectPtr>& objs)
	{
		mutex.lock();
		for (GameObjectPtr obj : objs)
			gameObjects_start.push_back(obj);
		mutex.unlock();
	}

	bool World::Init()
	{
		Component::RegisterComponents();

		if (!Camera::Init())
			return false;

		return true;
	}

	void World::Fini()
	{
		gameObjects.clear();
		gameObjects_start.clear();

		Camera::Fini();
	}

	void World::Tick()
	{
		mutex.lock();
		for (GameObjectPtr obj : gameObjects_start)
		{
			if (!obj->deleted)
			{
				if (obj->IsActiveInHierarchy())
					obj->Start();

				if (!obj->in_world_update)
				{
					obj->in_world_update = true;
					gameObjects.push_back(obj);
				}
			}
		}
		gameObjects_start.clear();
		mutex.unlock();

		for (auto iter = gameObjects.begin(); iter != gameObjects.end();)
		{
			auto& obj = *iter;
			if (!obj->deleted)
			{
				if (obj->IsActiveInHierarchy())
				{
					obj->Start();
					obj->Update();
				}
			}
			else
			{
				iter = gameObjects.erase(iter);
				continue;
			}

			iter++;
		}

		for (auto iter = gameObjects.begin(); iter != gameObjects.end();)
		{
			auto& obj = *iter;
			if (!obj->deleted)
			{
				if (obj->IsActiveInHierarchy())
					obj->LateUpdate();
			}
			else
			{
				iter = gameObjects.erase(iter);
				continue;
			}

			iter++;
		}
	}
}