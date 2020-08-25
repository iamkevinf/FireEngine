#include "game_view.h"

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <debugdraw/debugdraw.h>

#include "loader/loader.h"

#include <imgui.h>
#include "imgui/imgui_impl_bgfx.h"

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "scene/scene.h"
#include "core/component.h"
#include "core/transform.h"
#include "core/world.h"
#include "utils/time.h"
#include "graphics/camera.h"
#include <imgui\ImGuizmo.h>

namespace FireEngine
{
	
	void GameView::OnInit()
	{
		IM_ASSERT(World::Init() && "World Init Error");

		ddInit();
	}

	void GameView::OnGui()
	{
		//SceneManager::Render(viewId);
	}

	void GameView::OnTick(float dTime)
	{
		Time::Tick();
		World::Tick();
		Camera::PrepareAll();
		Camera::RenderAll();
	}

	void GameView::OnExit()
	{

		ddShutdown();

		World::Fini();
	}

}