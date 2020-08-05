#include "game_view.h"

#include "bgfx/bgfx.h"
#include "bx/bx.h"
#include "bx/math.h"

#include "../loader/loader.h"
#include <imgui.h>

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "../imgui/imgui_impl_bgfx.h"


namespace FireEngine
{
	bgfx::TextureHandle g_frame_tex;
	bgfx::FrameBufferHandle g_framebuffer;
	bgfx::TextureHandle g_ctex;
	bgfx::VertexLayout g_layout;
	bgfx::ProgramHandle g_program;

	bgfx::VertexBufferHandle g_vb;
	bgfx::IndexBufferHandle g_ib;
	float g_time;

	struct myvec
	{
		float x;
		float y;
		float z;
		uint32_t m_argb;
	};

	static myvec s_cubeVertices[] =
	{
		{-1.0f,  1.0f,  1.0f, 0x01010101 },
		{ 1.0f,  1.0f,  1.0f, 0xff0101ff },
		{-1.0f, -1.0f,  1.0f, 0x0101ff01 },
		{ 1.0f, -1.0f,  1.0f, 0xff01ffff },
		{-1.0f,  1.0f, -1.0f, 0x01ff0101 },
		{ 1.0f,  1.0f, -1.0f, 0xffff01ff },
		{-1.0f, -1.0f, -1.0f, 0xffffff01 },
		{ 1.0f, -1.0f, -1.0f, 0xffffffff },
	};

	static const uint16_t s_cubeTriList[] =
	{
		0, 1, 2, // 0
		1, 3, 2,
		4, 6, 5, // 2
		5, 6, 7,
		0, 2, 4, // 4
		4, 2, 6,
		1, 5, 3, // 6
		5, 7, 3,
		0, 4, 1, // 8
		4, 5, 1,
		2, 3, 6, // 10
		6, 3, 7,
	};


	void GameView::OnInit()
	{
		//create rt
		g_frame_tex = bgfx::createTexture2D(1024, 1024, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT);
		bgfx::TextureHandle fbtexture = g_frame_tex;
		g_framebuffer = bgfx::createFrameBuffer(1, &fbtexture, true);

		//≈‰÷√vertexbuffer ƒ⁄¥Ê≤ºæ÷
		g_layout
			.begin()
			.add(bgfx::Attrib::Enum::Position, 3, bgfx::AttribType::Enum::Float, false)
			.add(bgfx::Attrib::Enum::Color0, 4, bgfx::AttribType::Enum::Uint8, true)
			.end();

		auto vs = loadShader("vs_buildin");
		auto fs = loadShader("fs_buildin");
		g_program = bgfx::createProgram(vs, fs, true);

		// Create static vertex buffer.
		g_vb = bgfx::createVertexBuffer(
			bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), g_layout
		);
		g_ib = bgfx::createIndexBuffer(
			bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));
		auto dvb = bgfx::createDynamicVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), g_layout);
		bgfx::update(dvb, 0, bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)));

		g_ctex = loadTexture("images/test_texture2.png");
	}

	void GameView::OnTick(float dTime)
	{
		bgfx::setViewFrameBuffer(10, g_framebuffer);

		bgfx::ViewId viewId = 10;
		bgfx::setViewRect(viewId, 0, 0, 1024, 1024);
		bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF);
		bgfx::touch(viewId);

		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -35.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 proj = glm::perspective(glm::radians(60.0f), float(1024) / 1024, 0.1f, 100.0f);

		// Set view and projection matrix for view 10.
		{
			g_time += dTime;

			bgfx::setViewTransform(viewId, &view, &proj);

			for (int i = 0; i < 11; ++i)
			{
				for (int j = 0; j < 11; ++j)
				{
					glm::mat4 mtx = glm::identity<glm::mat4>();
					mtx = glm::translate(mtx, glm::vec3(15.0f - float(j) * 3.0f, -15.0f + float(i) * 3.0f, 0.0f));
					mtx *= glm::yawPitchRoll(g_time + j * 0.21f, g_time + i * 0.37f, 0.0f);

					// Set model matrix for rendering.
					bgfx::setTransform(&mtx);

					// Set vertex and index buffer.
					bgfx::setVertexBuffer(0, g_vb);
					bgfx::setIndexBuffer(g_ib);

					uint64_t state = BGFX_STATE_DEFAULT;

					// Set render states.
					bgfx::setState(state);

					// Submit primitive for rendering to view 0.
					bgfx::submit(viewId, g_program);
				}
			}

		}
		bgfx::frame();

	}

	void GameView::OnEditorGUI()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		if (ImGui::Begin("game view", 0, ImGuiWindowFlags_NoCollapse))
		{
			uint32_t mem = 0;
			((uint16_t*)&mem)[0] = g_frame_tex.idx;
			((uint8_t*)&mem)[2] = IMGUI_FLAGS_ALPHA_BLEND;
			((uint8_t*)&mem)[3] = 0;
			auto TexID = (ImTextureID)mem;

			ImGui::Image(TexID, ImGui::GetWindowSize());

			ImGui::PopStyleVar();

			ImGui::End();
		}

	}

	void GameView::OnExit()
	{

	}

}