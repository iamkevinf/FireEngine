#include "scene.h"

#include "core/transform.h"
#include <bx/math.h>

#include <cassert>
#include <debugdraw/debugdraw.h>
#include <imgui.h>
#include <imgui/ImGuizmo.h>
#include <graphics/camera.h>

namespace FireEngine
{
	std::vector<ScenePtr> SceneManager::scenePool;

	void Scene::Init()
	{
		root = TransformPtr(new Transform());
		root->name = "root";
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

	float fov = 90;
	void SceneManager::Render(bgfx::ViewId viewId)
	{
		Camera* camera = Camera::Main();
		if (camera)
		{
			ImGuiIO& io = ImGui::GetIO();

			float width = 1024;// pos.x + contentRegionMax.x - contentRegionMin.x;
			float height = 1024;// pos.y + contentRegionMax.y - contentRegionMin.y;

			ImGuizmo::BeginFrame();
			ImGui::SliderFloat("Fov", &fov, 20.f, 110.f);
			camera->SetFieldOfView(fov);

			ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);

			float* matrix = (float*)&camera->GetTransform()->GetLocal2WorldMatrix()[0][0];
			float matrixTranslation[3], matrixRotation[3], matrixScale[3];
			ImGuizmo::DecomposeMatrixToComponents(matrix,
				matrixTranslation, matrixRotation, matrixScale);

			ImGui::InputFloat3("Tr", matrixTranslation, 3);
			ImGui::InputFloat3("Rt", matrixRotation, 3);
			ImGui::InputFloat3("Sc", matrixScale, 3);

			ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation,
				matrixRotation, matrixScale, matrix);

			const float* view = &camera->GetViewMatrix()[0][0];
			const float* proj = &camera->GetProjectionMatrix()[0][0];
			ImGuizmo::DrawGrid(view, proj, &glm::identity<glm::mat4>()[0][0], 100.f);

			ImGuizmo::SetRect(0, 0, width, height);
			ImGuizmo::Manipulate(view, proj, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL,
				&glm::identity<glm::mat4>()[0][0], NULL, NULL, NULL, NULL);

			ImGuizmo::ViewManipulate((float*)view, 5,
				ImVec2(width - 128, 0), ImVec2(128, 128), 0x10101010);

			ImGuizmo::DrawCubes(view, proj, &glm::identity<glm::mat4>()[0][0], 1);
		}
	}

	void SceneManager::TickTest(bgfx::ViewId viewId, float dTime)
	{
		DebugDrawEncoder dde;
		dde.begin(viewId);

		dde.drawGrid(Axis::Y, { 0,0,0 });

		dde.end();
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