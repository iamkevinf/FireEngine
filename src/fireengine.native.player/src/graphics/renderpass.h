#ifndef __RENDER_PASS_H__
#define __RENDER_PASS_H__

#include "core/object.h"
#include "rendertexture.h"
#include "cameraclearflags.h"
#include "framebuffer.h"
#include "color.h"
#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

namespace FireEngine
{
	class RenderPass
	{
	public:
		static RenderPass* GetRenderPassBinding() { return s_render_pass_binding; }
		static std::shared_ptr<RenderPass> Create(std::shared_ptr<RenderTexture> color,
			std::shared_ptr<RenderTexture> depth,
			CameraClearFlags clearFlag, bool needDepth, glm::vec4 rect);

		virtual ~RenderPass();

		virtual void Begin(const Color& clear_color);
		virtual void End();

		void Bind();
		void UnBind();

		FrameBuffer GetFrameBuffer() const { return frame_buffer; }
		bgfx::FrameBufferHandle GetFrameBufferHandle() const { return frame_buffer_handle; }

		uint32_t GetFrameBufferWidth() const;
		uint32_t GetFrameBufferHeight() const;

		bool HasFrameBuffer() const;
		bgfx::ViewId GetViewId();

	private:
		RenderPass();

	private:
		static RenderPass* s_render_pass_binding;
		FrameBuffer frame_buffer;
		bgfx::FrameBufferHandle frame_buffer_handle;
		CameraClearFlags clear_flag = CameraClearFlags::Color;
		bool need_depth;
		glm::vec4 rect;
	};
}

#endif // __RENDER_PASS_H__
