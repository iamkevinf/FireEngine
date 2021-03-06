#ifndef __SCENE_H__
#define __SCENE_H__

#include "core/object.h"

#include <string>
#include <vector>

#include "../exportapi.h"

#include <bgfx/bgfx.h>

namespace FireEngine
{
	class SceneManager;

	enum class ActiveOption
	{
		Active = 1,
		Deactive = 0,
	};

	FIREENGINE_HANDLE(SceneHandle)

	typedef std::shared_ptr<class Transform> TransformPtr;

	class Scene : public IObject
	{
	public:
		Scene() = default;
		Scene(const Scene& ref) = delete;
		Scene& operator= (const Scene& scene) = delete;
		Scene& operator= (Scene&& scene) = delete;

	public:
		void Init();

	public:
		std::u16string name;
		ActiveOption active = ActiveOption::Active;
		TransformPtr root;
	};

	typedef std::shared_ptr<Scene> ScenePtr;

	class SceneManager
	{
	public:
		static void AddScene(ScenePtr scene);
		static void RemoveScene(ScenePtr scene);
		static void RemoveScene(SceneHandle handle);
		static ScenePtr GetScene(SceneHandle handle);

		static void Render(bgfx::ViewId viewId);
		static void TickTest(bgfx::ViewId viewId, float dTime);

	public:
		static std::vector<ScenePtr> scenePool;
	};
}

#endif // __SCENE_H__