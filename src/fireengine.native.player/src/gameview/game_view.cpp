#include "game_view.h"

#include "bgfx/bgfx.h"
#include "bx/bx.h"
#include "bx/math.h"

#include "../loader/loader.h"
#include <imgui.h>

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

		//配置vertexbuffer 内存布局
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

	void GameView::OnTick()
	{
		bgfx::setViewFrameBuffer(10, g_framebuffer);

		bgfx::setViewRect(10, 0, 0, 1024, 1024);
		bgfx::setViewClear(10, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF);
		bgfx::touch(10);


		const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
		const bx::Vec3 eye = { 0.0f, 0.0f, -35.0f };

		// Set view and projection matrix for view 10.
		{
			g_time += 0.001f;

			float view[16];
			bx::mtxLookAt(view, eye, at);

			float proj[16];
			bx::mtxProj(proj, 60.0f, float(1024) / float(1024), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
			bgfx::setViewTransform(10, view, proj);


			float mtx[16];
			for (int i = 0; i < 10; ++i)
			{
				for (int j = 0; j < 10; ++j)
				{
					bx::mtxIdentity(mtx);
					bx::mtxSRT(mtx, 1, 1, 1, g_time + 10 * 0.21f, g_time + 10 * 0.37f, 0, i * 5 - 25, j * 5 - 25, 0);

					// Set model matrix for rendering.
					bgfx::setTransform(mtx);

					// Set vertex and index buffer.
					bgfx::setVertexBuffer(0, g_vb);
					bgfx::setIndexBuffer(g_ib);

					uint64_t state = 0
						| BGFX_STATE_WRITE_MASK  //1.write mask （rgb a z）
						| BGFX_STATE_DEPTH_TEST_LESS //2.z test(less lessequal...
						| BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ZERO) //3.alphablend 
						//or BGFX_STATE_BLEND_FUNC_SEPARATE;
						//4.BLEND_EQUATION 大部分情况下都是使用Func_add，基本上用不到改
						| BGFX_STATE_CULL_CW //5 cull ， //CW CCW or none
						| BGFX_STATE_PT_TRISTRIP //6 primitive type，也在state中，不加就是trilist 绘制模式也是放在state里面的，
						| BGFX_STATE_MSAA //7.msaa 
						;

					// Set render states.
					bgfx::setState(state);

					// Submit primitive for rendering to view 0.
					bgfx::submit(10, g_program);
				}
			}

		}
		bgfx::frame();

	}

	void GameView::OnEditorGUI()
	{
		static bool s_a = true;
		if (ImGui::Begin("test RT", &s_a, ImGuiWindowFlags_NoCollapse))
		{
			uint32_t mem = 0;
			((uint16_t*)&mem)[0] = g_frame_tex.idx;
			((uint8_t*)&mem)[2] = 1;//1 =IMGUI_FLAGS_ALPHA_BLEND
			((uint8_t*)&mem)[3] = 0;
			auto TexID = (ImTextureID)mem;

			ImGui::Image(TexID, ImGui::GetWindowSize());
			ImGui::End();
		}
	}

	void GameView::OnExit()
	{

	}

}