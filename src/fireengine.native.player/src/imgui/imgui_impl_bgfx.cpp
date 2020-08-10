#include "imgui_impl_bgfx.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bgfx/embedded_shader.h"
#include <map>
#include "innelshader/vs_imgui_image.bin.h"
#include "innelshader/fs_imgui_image.bin.h"
#include "innelshader/vs_imgui_texture.bin.h"
#include "innelshader/fs_imgui_texture.bin.h"
#include "innelshader/vs_ocornut_imgui.bin.h"
#include "innelshader/fs_ocornut_imgui.bin.h"
#include <bx/math.h>

static const bgfx::EmbeddedShader s_embeddedShaders[] =
{
	BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
	BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
	BGFX_EMBEDDED_SHADER(vs_imgui_image),
	BGFX_EMBEDDED_SHADER(fs_imgui_image),
	BGFX_EMBEDDED_SHADER(vs_imgui_texture),
	BGFX_EMBEDDED_SHADER(fs_imgui_texture),
	BGFX_EMBEDDED_SHADER_END()
};

struct viewData
{
	bgfx::FrameBufferHandle framebuffer;
	int width;
	int height;
};
//static std::map<int, int> mapViewID;
static bgfx::TextureHandle g_FontTexture = BGFX_INVALID_HANDLE;
static bgfx::ProgramHandle  m_program;
static bgfx::VertexLayout  m_layout;
static bgfx::UniformHandle s_tex;
static bgfx::UniformHandle u_imageLodEnabled;
static bgfx::ProgramHandle m_imageProgram;
//int GetBgfxViewID(int imguiID)
//{
//	auto it = mapViewID.find(imguiID);
//	if (it != mapViewID.end())
//	{
//		return it->second;
//	}
//	else
//	{
//		int id = mapViewID.size() + 1;
//		mapViewID[imguiID] = id;
//		return id;
//	}
//
//}

void bgfx_OnCreateSubWindow(ImGuiViewport* viewport)
{
	//int viewid = GetBgfxViewID(viewport->ID);
	int w = (int)viewport->Size.x;
	int h = (int)viewport->Size.y;
	void* hwnd = viewport->PlatformHandleRaw;
	auto fth = bgfx::createFrameBuffer(hwnd, w, h, bgfx::TextureFormat::Count, bgfx::TextureFormat::Count);

	viewport->RendererUserData = new viewData{ fth,w,h };
}
void bgfx_OnDestorySubWindow(ImGuiViewport* viewport)
{
	auto _viewData = (viewData*)viewport->RendererUserData;

	//这玩意多进来一次，主窗口干嘛从这跑一趟？
	if (_viewData == nullptr)
		return;

	if (bgfx::isValid(_viewData->framebuffer))
		bgfx::destroy(_viewData->framebuffer);
	viewport->RendererUserData = NULL;
	delete _viewData;
}
void bgfx_OnSetWindowSize(ImGuiViewport* viewport, ImVec2 size)
{
	//int viewid = GetBgfxViewID(viewport->ID);
	void* hwnd = viewport->PlatformHandleRaw;
	int w = (int)size.x;
	int h = (int)size.y;

	auto _viewData = (viewData*)viewport->RendererUserData;

	if (bgfx::isValid(_viewData->framebuffer))
	{
		if (w == _viewData->width && h == _viewData->height)
		{
			//donothing
		}
		else
		{
			bgfx::destroy(_viewData->framebuffer);
			_viewData->framebuffer = BGFX_INVALID_HANDLE;
		}
	}
	if (!bgfx::isValid(_viewData->framebuffer))
	{
		_viewData->framebuffer = bgfx::createFrameBuffer(hwnd, w, h, bgfx::TextureFormat::Count, bgfx::TextureFormat::Count);
	}


	//bgfx::setViewFrameBuffer(viewid, _viewData->framebuffer);
	//bgfx::setViewClear(viewid, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF00FFFF);
	//bgfx::setViewRect(viewid, 0, 0, w, h);

}
inline bool checkAvailTransientBuffers(uint32_t _numVertices, const bgfx::VertexLayout& _layout, uint32_t _numIndices)
{
	return _numVertices == bgfx::getAvailTransientVertexBuffer(_numVertices, _layout)
		&& (0 == _numIndices || _numIndices == bgfx::getAvailTransientIndexBuffer(_numIndices))
		;
}
void  bgfx_imgui_Render(int nViewId, ImDrawData* _drawData)
{
	// Avoid rendering when minimized
	if (_drawData->DisplaySize.x <= 0.0f || _drawData->DisplaySize.y <= 0.0f)
		return;

	const float width = _drawData->DisplaySize.x;
	const float height = _drawData->DisplaySize.y;

	bgfx::setViewName(nViewId, "ImGui");
	bgfx::setViewMode(nViewId, bgfx::ViewMode::Sequential);

	const bgfx::Caps* caps = bgfx::getCaps();
	{
		//這個0.5 會模糊
		float L = _drawData->DisplayPos.x;// +0.5f;
		float R = _drawData->DisplayPos.x + _drawData->DisplaySize.x;// +0.5f;
		float T = _drawData->DisplayPos.y;// +0.5f;
		float B = _drawData->DisplayPos.y + _drawData->DisplaySize.y;// +0.5f;

		//D3DMATRIX mat_identity = { { { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f } } };
		//float mat_projection[16] =
		//{
		//   2.0f / (R - L),   0.0f,         0.0f,  0.0f,
		//   0.0f,         2.0f / (T - B),   0.0f,  0.0f,
		//   0.0f,         0.0f,         0.5f,  0.0f,
		//   (L + R) / (L - R),  (T + B) / (B - T),  0.5f,  1.0f
		//};

		float view[16];
		bx::mtxIdentity(view);
		float ortho[16];
		bx::mtxOrtho(ortho, L, R, B, T, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
		bgfx::setViewTransform(nViewId, view, ortho);
		bgfx::setViewRect(nViewId, 0, 0, uint16_t(width), uint16_t(height));
	}

	// Render command lists
	for (int32_t ii = 0, num = _drawData->CmdListsCount; ii < num; ++ii)
	{
		bgfx::TransientVertexBuffer tvb;
		bgfx::TransientIndexBuffer tib;

		const ImDrawList* drawList = _drawData->CmdLists[ii];
		uint32_t numVertices = (uint32_t)drawList->VtxBuffer.size();
		uint32_t numIndices = (uint32_t)drawList->IdxBuffer.size();

		if (!checkAvailTransientBuffers(numVertices, m_layout, numIndices))
		{
			// not enough space in transient buffer just quit drawing the rest...
			break;
		}

		bgfx::allocTransientVertexBuffer(&tvb, numVertices, m_layout);
		bgfx::allocTransientIndexBuffer(&tib, numIndices);

		ImDrawVert* verts = (ImDrawVert*)tvb.data;
		bx::memCopy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert));

		ImDrawIdx* indices = (ImDrawIdx*)tib.data;
		bx::memCopy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx));

		uint32_t offset = 0;
		for (const ImDrawCmd* cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd; ++cmd)
		{
			if (cmd->UserCallback)
			{
				cmd->UserCallback(drawList, cmd);
			}
			else if (0 != cmd->ElemCount)
			{
				uint64_t state = 0
					| BGFX_STATE_WRITE_RGB
					| BGFX_STATE_WRITE_A
					| BGFX_STATE_MSAA
					;

				bgfx::TextureHandle th = g_FontTexture;
				bgfx::ProgramHandle program = m_program;

				if (NULL != cmd->TextureId)
				{
					union {
						ImTextureID ptr;
						struct
						{
							bgfx::TextureHandle handle; uint8_t flags; uint8_t mip;
						} s;
					} texture = { cmd->TextureId };
					state |= 0 != (IMGUI_FLAGS_ALPHA_BLEND & texture.s.flags)
						? BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
						: BGFX_STATE_NONE
						;
					th = texture.s.handle;
					if (0 != texture.s.mip)
					{
						const float lodEnabled[4] = { float(texture.s.mip), 1.0f, 0.0f, 0.0f };
						bgfx::setUniform(u_imageLodEnabled, lodEnabled);
						program = m_imageProgram;
					}
				}
				else
				{
					state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
				}


				uint16_t x = uint16_t(cmd->ClipRect.x);// -_drawData->DisplayPos.x;
				uint16_t y = uint16_t(cmd->ClipRect.y);// -_drawData->DisplayPos.y;
				const uint16_t w = uint16_t(cmd->ClipRect.z) - x;
				const uint16_t h = uint16_t(cmd->ClipRect.w) - y;
				x -= (int)_drawData->DisplayPos.x;
				y -= (int)_drawData->DisplayPos.y;
				bgfx::setScissor(x, y, w, h);

				bgfx::setState(state);
				bgfx::setTexture(0, s_tex, th);
				bgfx::setVertexBuffer(0, &tvb, 0, numVertices);
				bgfx::setIndexBuffer(&tib, offset, cmd->ElemCount);
				bgfx::submit(nViewId, program);
			}

			offset += cmd->ElemCount;
		}
	}
}
void bgfx_RenderWindow(ImGuiViewport* viewport, void* arg)
{
	int viewid = 0;// GetBgfxViewID(viewport->ID);
	auto _viewData = (viewData*)viewport->RendererUserData;
	bgfx::setViewFrameBuffer(viewid, _viewData->framebuffer);
	bgfx::setViewFrameBuffer(1, BGFX_INVALID_HANDLE);
	bgfx::setViewRect(viewid, 0, 0, _viewData->width, _viewData->height);
	bgfx::touch(viewid);
	bgfx_imgui_Render(viewid, viewport->DrawData);

	bgfx::frame();
}
void bgfx_SwapBuffers(ImGuiViewport* viewport, void* arg)
{
}
void ImGui_ImplBgfx_InitPlatformInterface()
{
	ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
	platform_io.Renderer_CreateWindow = bgfx_OnCreateSubWindow;
	platform_io.Renderer_DestroyWindow = bgfx_OnDestorySubWindow;
	platform_io.Renderer_SetWindowSize = bgfx_OnSetWindowSize;
	platform_io.Renderer_RenderWindow = bgfx_RenderWindow;
	platform_io.Renderer_SwapBuffers = bgfx_SwapBuffers;
}

bool ImGui_ImplBgfx_CreateFontsTexture()
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();

	unsigned char* pixels;
	int width, height, bytes_per_pixel;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);
	g_FontTexture = bgfx::createTexture2D(
		(uint16_t)width
		, (uint16_t)height
		, false
		, 1
		, bgfx::TextureFormat::BGRA8
		, 0
		, bgfx::copy(pixels, width * height * 4)
	);


	union { struct { bgfx::TextureHandle handle; uint8_t flags; uint8_t mip; } s; ImTextureID id; } tex;
	tex.s.handle = g_FontTexture;
	tex.s.flags = IMGUI_FLAGS_ALPHA_BLEND;
	tex.s.mip = 0;

	io.Fonts->TexID = tex.id;
	return true;
}
static void ImGui_ImplBgfx_CreateDeviceObjectsForPlatformWindows()
{
	ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
	for (int i = 1; i < platform_io.Viewports.Size; i++)
		if (!platform_io.Viewports[i]->RendererUserData)
		{
			bgfx_OnCreateSubWindow(platform_io.Viewports[i]);
		}
}
bool ImGui_ImplBgfx_CreateDeviceObjects()
{
	if (!ImGui_ImplBgfx_CreateFontsTexture())
		return false;
	ImGui_ImplBgfx_CreateDeviceObjectsForPlatformWindows();
	return true;
}
bx::AllocatorI* g_customalloc = NULL;
bx::AllocatorI* GetAlloc()
{
	return g_customalloc;
}
void     ImGui_Implbgfx_Init()
{
	//shader init
	bgfx::RendererType::Enum type = bgfx::getRendererType();
	m_program = bgfx::createProgram(
		bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_imgui_texture")
		, bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_ocornut_imgui")
		, true
	);

	u_imageLodEnabled = bgfx::createUniform("u_imageLodEnabled", bgfx::UniformType::Vec4);

	m_imageProgram = bgfx::createProgram(
		bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_imgui_image")
		, bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_imgui_image")
		, true
	);

	m_layout
		.begin()
		.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		.end();

	s_tex = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);

	ImGuiIO& io = ImGui::GetIO();
	io.BackendRendererName = "imgui_impl_bgfx";
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
	io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;  // We can create multi-viewports on the Renderer side (optional)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		ImGui_ImplBgfx_InitPlatformInterface();
}
void     ImGui_Implbgfx_Shutdown()
{
	bgfx::destroy(s_tex);
	bgfx::destroy(g_FontTexture);

	bgfx::destroy(u_imageLodEnabled);
	bgfx::destroy(m_imageProgram);
	bgfx::destroy(m_program);
}


void     ImGui_Implbgfx_NewFrame()
{
	if (!bgfx::isValid(g_FontTexture))
	{
		ImGui_ImplBgfx_CreateDeviceObjects();
	}
}
void     ImGui_Implbgfx_RenderDrawData(ImDrawData* draw_data)
{
	bgfx::setViewFrameBuffer(0, BGFX_INVALID_HANDLE);
	bgfx::setViewFrameBuffer(1, BGFX_INVALID_HANDLE);
	int w = (int)draw_data->DisplaySize.x;
	int h = (int)draw_data->DisplaySize.y;
	bgfx::setViewRect(0, 0, 0, w, h);
	bgfx::touch(0);

	bgfx_imgui_Render(0, draw_data);
	bgfx::frame();
}