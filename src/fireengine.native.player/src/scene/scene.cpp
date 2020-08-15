#include "scene.h"

#include "../graphics/transform.h"

namespace FireEngine
{
	std::vector<ScenePtr> SceneManager::scenePool;

	void Scene::Init()
	{
		root = TransformPtr(new Transform());
		root->name = u"Empty";
		ObjectManager::Register(root, ObjectType::Transform);
	}

	void SceneManager::AddScene(ScenePtr scene)
	{
		SceneManager::scenePool.push_back(scene);
	}

	void SceneManager::RemoveScene(ScenePtr scene)
	{
		SceneManager::scenePool.erase(
			std::find(
				SceneManager::scenePool.begin(),
				SceneManager::scenePool.end(),
				scene)
		);
	}

	void SceneManager::RemoveScene(SceneHandle handle)
	{
		ScenePtr scene = SceneManager::GetScene(handle);

		SceneManager::RemoveScene(scene);
	}

	ScenePtr SceneManager::GetScene(SceneHandle handle)
	{
		ObjectHandle* objHandle = reinterpret_cast<ObjectHandle*>(&handle);
		auto scene = ObjectManager::Get(*objHandle);
		if (scene == nullptr)
			return nullptr;

		Scene* sptr = (Scene*)scene.get();
		for (auto iter = SceneManager::scenePool.begin(); iter != SceneManager::scenePool.end();)
		{
			if (iter->get() == sptr)
				return *iter;
		}

		return nullptr;
	}

	EXPORT_API SceneHandle SceneCreate(const char16_t* name)
	{
		ScenePtr scene = ScenePtr(new Scene());
		ObjectManager::Register(scene, ObjectType::Scene);
		scene->name = name;
		scene->active = ActiveOption::Active;
		SceneManager::AddScene(scene);

		scene->Init();
		return { scene->objectID };
	}

	EXPORT_API void SceneRemove(SceneHandle handle)
	{
		SceneManager::RemoveScene(handle);
	}

	EXPORT_API int SceneCount()
	{
		return (int)SceneManager::scenePool.size();
	}


	EXPORT_API SceneHandle SceneFindByIndex(int index)
	{
		auto scene = SceneManager::scenePool[index];

		if (index < 0 || index >= SceneManager::scenePool.size())
			return { kInvalidHandle };
		return { scene->objectID };
	}


	EXPORT_API void SceneSetName(SceneHandle handle, const char16_t* name)
	{
		ObjectHandle* objHandle = reinterpret_cast<ObjectHandle*>(&handle);
		auto scene = ObjectManager::Get(*objHandle);
		auto pscene = (Scene*)scene.get();
		pscene->name = name;

	}
	EXPORT_API const char16_t* SceneGetName(SceneHandle handle)
	{
		ObjectHandle* objHandle = reinterpret_cast<ObjectHandle*>(&handle);
		auto scene = ObjectManager::Get(*objHandle);
		auto pscene = (Scene*)scene.get();
		return pscene->name.c_str();
	}

	//场景是否活动
	EXPORT_API void SceneSetActive(SceneHandle handle, ActiveOption active)
	{
		ObjectHandle* objHandle = reinterpret_cast<ObjectHandle*>(&handle);
		auto scene = ObjectManager::Get(*objHandle);
		auto pscene = (Scene*)scene.get();
		pscene->active = active;
	}
	EXPORT_API ActiveOption SceneGetActive(SceneHandle handle)
	{
		ObjectHandle* objHandle = reinterpret_cast<ObjectHandle*>(&handle);
		auto scene = ObjectManager::Get(*objHandle);
		auto pscene = (Scene*)scene.get();
		return pscene->active;
	}

	//每场景有且仅有一个root，root transform 不可操作
	EXPORT_API TransformHandle SceneGetRoot(SceneHandle handle)
	{
		ObjectHandle* objHandle = reinterpret_cast<ObjectHandle*>(&handle);
		auto scene = ObjectManager::Get(*objHandle);
		auto pscene = (Scene*)scene.get();
		return { pscene->root->objectID };
	}



}