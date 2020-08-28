#include "wrapper.h"

#include <glm/glm.hpp>
#include <bx/string.h>
#include <imgui.h>
#include <imgui/imgui_impl_bgfx.h>

#include "app/app.h"
#include "loader/loader.h"
#include "gameview/game_view.h"
#include "graphics/camera.h"

namespace FireEngine
{
	int feApp_MainLoop(const char16_t* title, void(*OnInit)(), void(*OnGUI)(), void(*OnTick)(), void(*OnExit)())
	{
		return WindowMain(title, OnInit, OnGUI, OnTick, OnExit);
	}

	bool feApp_SetWindowTitle(const char16_t* title)
	{
		return SetWindowTitle(title);
	}

	bgfx::TextureHandle feLoadTexture(const char* _name, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
	{
		return loadTexture(_name, _flags, _skip, _info, _orientation);
	}

	bgfx::ShaderHandle feLoadShader(const char* name)
	{
		return loadShader(name);
	}


	struct SampleData
	{
		static constexpr uint32_t kNumSamples = 100;

		SampleData()
		{
			reset();
		}

		void reset()
		{
			m_offset = 0;
			::memset(m_values, 0, sizeof(m_values));

			m_min = 0.0f;
			m_max = 0.0f;
			m_avg = 0.0f;
		}

		void pushSample(float value)
		{
			m_values[m_offset] = value;
			m_offset = (m_offset + 1) % kNumSamples;

			float min = 3.402823e+38f;
			float max = -3.402823e+38f;
			float avg = 0.0f;

			for (uint32_t ii = 0; ii < kNumSamples; ++ii)
			{
				const float val = m_values[ii];
				min = glm::min(min, val);
				max = glm::max(max, val);
				avg += val;
			}

			m_min = min;
			m_max = max;
			m_avg = avg / kNumSamples;
		}

		int32_t m_offset;
		float m_values[kNumSamples];

		float m_min;
		float m_max;
		float m_avg;
	};

	static SampleData s_frameTime;

	void feApp_ShowState()
	{
		ImGui::SetNextWindowPos(
			ImVec2(10.0f, 50.0f)
			, ImGuiCond_FirstUseEver
		);
		ImGui::SetNextWindowSize(
			ImVec2(300.0f, 210.0f)
			, ImGuiCond_FirstUseEver
		);

		ImGui::Begin("debug show state");

		const bgfx::Stats* stats = bgfx::getStats();
		const double toMsCpu = 1000.0 / stats->cpuTimerFreq;
		const double toMsGpu = 1000.0 / stats->gpuTimerFreq;
		const double frameMs = double(stats->cpuTimeFrame) * toMsCpu;

		s_frameTime.pushSample(float(frameMs));

		char frameTextOverlay[256];
		bx::snprintf(frameTextOverlay, BX_COUNTOF(frameTextOverlay), "Min: %.3fms, Max: %.3fms\nAvg: %.3fms, %.1f FPS"
			, s_frameTime.m_min
			, s_frameTime.m_max
			, s_frameTime.m_avg
			, 1000.0f / s_frameTime.m_avg
		);

		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImColor(0.0f, 0.5f, 0.15f, 1.0f).Value);
		ImGui::PlotHistogram("Frame"
			, s_frameTime.m_values
			, SampleData::kNumSamples
			, s_frameTime.m_offset
			, frameTextOverlay
			, 0.0f
			, 60.0f
			, ImVec2(0.0f, 45.0f)
		);
		ImGui::PopStyleColor();

		ImGui::Text("Submit CPU %0.3f, GPU %0.3f (L: %d)"
			, double(stats->cpuTimeEnd - stats->cpuTimeBegin) * toMsCpu
			, double(stats->gpuTimeEnd - stats->gpuTimeBegin) * toMsGpu
			, stats->maxGpuLatency
		);

		if (-INT64_MAX != stats->gpuMemoryUsed)
		{
			char tmp0[64];
			bx::prettify(tmp0, BX_COUNTOF(tmp0), stats->gpuMemoryUsed);

			char tmp1[64];
			bx::prettify(tmp1, BX_COUNTOF(tmp1), stats->gpuMemoryMax);

			ImGui::Text("GPU mem: %s / %s", tmp0, tmp1);
		}

		ImGui::End();

	}

	struct Target
	{
	public:
		bgfx::TextureHandle     frametex;
		bgfx::FrameBufferHandle framebuffer;
		int width;
		int height;
	};

	EXPORT_API TargetHandle feTarget_Create(int width, int height)
	{
		auto texframe = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT);
		auto t = new Target();
		t->frametex = texframe;
		t->framebuffer = bgfx::createFrameBuffer(1, &texframe, true);
		t->width = width;
		t->height = height;
		return TargetHandle{ t };
	}
	EXPORT_API ImTextureID feTarget_GetImGuiTexID(TargetHandle handle)
	{
		union { struct { bgfx::TextureHandle handle; uint8_t flags; uint8_t mip; } s; ImTextureID id; } tex;
		tex.s.handle = handle.ptr->frametex;
		tex.s.flags = IMGUI_FLAGS_ALPHA_BLEND;
		tex.s.mip = 0;
		return tex.id;
	}
	EXPORT_API void feTarget_Destory(TargetHandle handle)
	{
		bgfx::destroy(handle.ptr->framebuffer);
		handle.ptr->framebuffer = BGFX_INVALID_HANDLE;
		handle.ptr->frametex = BGFX_INVALID_HANDLE;
		delete handle.ptr;
	}
	EXPORT_API void feTarget_Reset(TargetHandle handle, int width, int height)
	{
		if (bgfx::isValid(handle.ptr->framebuffer))
		{
			if (width != handle.ptr->width || height != handle.ptr->height)
			{
				bgfx::destroy(handle.ptr->framebuffer);
				handle.ptr->framebuffer = BGFX_INVALID_HANDLE;
				handle.ptr->frametex = BGFX_INVALID_HANDLE;
			}
		}
		if (!bgfx::isValid(handle.ptr->framebuffer))
		{
			auto texframe = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT);
			auto texd16 = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::D16, BGFX_TEXTURE_RT_WRITE_ONLY);
			bgfx::TextureHandle fbtextures[] = { texframe,texd16 };
			handle.ptr->frametex = texframe;
			handle.ptr->framebuffer = bgfx::createFrameBuffer(2, fbtextures, true);
			handle.ptr->width = width;
			handle.ptr->height = height;
		}
	}

	int fviewid;
	TargetHandle target;
	EXPORT_API void feFrame_Clear(uint32_t color, bool withColor, bool withDepth)
	{
		fviewid++;

		IM_ASSERT((withColor || withDepth) && "error clear args");

		uint16_t flag = 0;
		if (withColor)flag |= BGFX_CLEAR_COLOR;
		if (withDepth)flag |= BGFX_CLEAR_DEPTH;

		if (target.ptr != NULL)
		{
			bgfx::setViewFrameBuffer(fviewid, target.ptr->framebuffer);
			bgfx::setViewRect(fviewid, 0, 0, target.ptr->width, target.ptr->height);
		}
		else
		{
			auto w = bgfx::getStats()->width;
			auto h = bgfx::getStats()->height;
			bgfx::setViewFrameBuffer(fviewid, BGFX_INVALID_HANDLE);
			bgfx::setViewRect(fviewid, 0, 0, w, h);
		}
		bgfx::setViewClear(fviewid, flag, color);
		bgfx::touch(fviewid);
	}
	EXPORT_API void feFrame_SetTarget(TargetHandle handle)
	{
		target = handle;
		fviewid = -1;
	}
	EXPORT_API void feFrame_SetMainScreen()
	{
		target.ptr = NULL;
		fviewid = -1;
	}
	EXPORT_API void feFrame_Flush()
	{
		bgfx::frame();
		fviewid = -1;
	}

	EXPORT_API void febgfxSetDebug(uint32_t debug, bool debugGame)
	{
		if (debugGame)
			Camera::Main()->SetDebug(debug);
		else
			bgfx::setDebug(debug);
	}


}

