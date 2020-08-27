#include "world.h"

#include "graphics/camera.h"
#include "graphics/renderer.h"

namespace FireEngine
{
	std::list<GameObjectPtr> World::gameObjects;
	std::list<GameObjectPtr> World::gameObjects_start;
	std::mutex World::mutex;

	void World::AddGameObject(const GameObjectPtr& obj)
	{
		mutex.lock();
		gameObjects_start.push_back(obj);
		Renderer::SetCullingDirty(Camera::Main());
		mutex.unlock();
	}

	void World::AddGameObjects(const std::list<GameObjectPtr>& objs)
	{
		mutex.lock();
		for (GameObjectPtr obj : objs)
			gameObjects_start.push_back(obj);
		Renderer::SetCullingDirty(Camera::Main());
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

		if (Renderer::IsRenderersDirty())
		{
			Renderer::SetRenderersDirty(false);

			std::list<Renderer*>& renderers = Renderer::GetRenderers();
			renderers.clear();

			FindAllRenders(gameObjects, renderers, false, false, false);
		}
	}

	void World::FindAllRenders(
		const std::list<GameObjectPtr>& objs, std::list<Renderer*>& renderers,
		bool include_inactive, bool include_disable, bool static_only)
	{
		for (GameObjectPtr obj : objs)
		{
			if (obj->deleted)
				continue;

			if (include_inactive || obj->IsActiveInHierarchy())
			{
				if (!static_only || obj->IsStatic())
				{
					std::vector<std::shared_ptr<Renderer>> rs;
					if (!obj->GetComponents<Renderer>(rs))
						continue;

					for (std::shared_ptr<Renderer>& renderer : rs)
					{
						if (include_disable || renderer->IsEnable())
							renderers.push_back(renderer.get());
					}
				}
			}
		}
	}
}