#include "wrapper.h"

#include <glm/glm.hpp>
#include <bx/string.h>
#include <imgui.h>

#include "../app/app.h"
#include "../loader/loader.h"

namespace FireEngine
{
	int feApp_MainLoop(const char16_t* title, void(*OnInit)(), void(*OnGUI)(), void(*OnExit)())
	{
		return WindowMain(title, OnInit, OnGUI, OnExit);
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

}

