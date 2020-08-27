#ifndef __RENDER_PASS_H__
#define __RENDER_PASS_H__

#include "core/object.h"
#include "rendertexture.h"
#include "cameraclearflags.h"
#include "framebuffer.h"
#include "color.h"
#include <bgfx/bgfx.h>
#include "math/rect.hpp"

namespace FireEngine
{
	class RenderPass
	{
	public:
		static std::shared_ptr<RenderPass> Create(std::shared_ptr<RenderTexture> color,
			std::shared_ptr<RenderTexture> depth,
			CameraClearFlags clearFlag, bool needDepth, Rect rect);

		virtual ~RenderPass();

		virtual bgfx::ViewId Begin(const Color& clear_color);
		virtual void End();

		FrameBuffer GetFrameBuffer() const { return frame_buffer; }
		bgfx::FrameBufferHandle GetFrameBufferHandle() const { return frame_buffer_handle; }

		uint32_t GetFrameBufferWidth() const;
		uint32_t GetFrameBufferHeight() const;

		bool HasFrameBuffer() const;
		bgfx::ViewId GetViewId();

	private:
		RenderPass();

	private:
		FrameBuffer frame_buffer;
		bgfx::FrameBufferHandle frame_buffer_handle;
		CameraClearFlags clear_flag = CameraClearFlags::Color;
		bool need_depth;
		Rect rect;
	};
}

#endif // __RENDER_PASS_H__
