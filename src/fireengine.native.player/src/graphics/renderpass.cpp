#include "renderpass.h"

#include "camera.h"

#include <bgfx/bgfx.h>

namespace FireEngine
{
	RenderPass* RenderPass::s_render_pass_binding;

	std::shared_ptr<RenderPass> RenderPass::Create(
		std::shared_ptr<RenderTexture> color_texture,
		std::shared_ptr<RenderTexture> depth_texture,
		CameraClearFlags clear_flag, bool need_depth,
		glm::vec4 rect)
	{
		std::shared_ptr<RenderPass> pass = std::shared_ptr<RenderPass>(new RenderPass());
		pass->frame_buffer.color_texture = color_texture;
		pass->frame_buffer.depth_texture = depth_texture;
		pass->clear_flag = clear_flag;
		pass->need_depth = need_depth;

		bgfx::TextureHandle fbtextures[] = { color_texture->GetTextureHandle(), depth_texture->GetTextureHandle() };
		pass->frame_buffer_handle = bgfx::createFrameBuffer(1,
			&(color_texture->GetTextureHandle()), true);

		pass->rect = rect;

		return pass;
	}

	RenderPass::RenderPass()
	{

	}

	RenderPass::~RenderPass()
	{
	}

	static bgfx::ViewId s_viewId = -1;
	bgfx::ViewId RenderPass::Begin(const Color& clear_color)
	{
		Bind();

		s_viewId++;

		bgfx::setViewFrameBuffer(s_viewId, frame_buffer_handle);
		bgfx::setViewRect(s_viewId, rect.x, rect.y, rect.z, rect.w);

		uint16_t flag = 0;
		Color clearColor = clear_color;
		uint32_t hexColor = clearColor.GetHex();

		switch (clear_flag)
		{
		case CameraClearFlags::Invalidate:
			break;

		case CameraClearFlags::Color:
		{
			flag |= BGFX_CLEAR_COLOR;
			break;
		}
		case CameraClearFlags::Depth:
		{
			flag |= BGFX_CLEAR_DEPTH;
			break;
		}
		case CameraClearFlags::Nothing:
			break;
		default:
			break;
		}

		if (flag > 0)
			bgfx::setViewClear(s_viewId, flag, hexColor);
		bgfx::touch(s_viewId);

		return s_viewId;
	}

	void RenderPass::End()
	{
		UnBind();

		s_viewId = -1;

		bgfx::frame();
	}

	void RenderPass::Bind()
	{
		s_render_pass_binding = this;
	}

	void RenderPass::UnBind()
	{
		s_render_pass_binding = nullptr;
	}

	bgfx::ViewId RenderPass::GetViewId()
	{
		return s_viewId;
	}

	bool RenderPass::HasFrameBuffer()const
	{
		return frame_buffer.color_texture || frame_buffer.depth_texture;
	}

	uint32_t RenderPass::GetFrameBufferWidth() const
	{
		if (!HasFrameBuffer())
		{
			return Camera::Main()->GetTargetWidth();
		}
		else
		{
			if (frame_buffer.color_texture)
				return frame_buffer.color_texture->GetWidth();

			if (frame_buffer.depth_texture)
				return frame_buffer.depth_texture->GetWidth();
		}

		return -1;
	}

	uint32_t RenderPass::GetFrameBufferHeight() const
	{
		if (!HasFrameBuffer())
		{
			return Camera::Main()->GetTargetHeight();
		}
		else
		{
			if (frame_buffer.color_texture)
				return frame_buffer.color_texture->GetHeight();

			if (frame_buffer.depth_texture)
				return frame_buffer.depth_texture->GetHeight();
		}

		return -1;
	}
}