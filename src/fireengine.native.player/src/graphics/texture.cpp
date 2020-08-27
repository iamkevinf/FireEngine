#include "texture.h"

#include "rendertexture.h"
#include "rendertextureformat.h"

#include <bgfx/bgfx.h>
#include <imgui.h>

namespace FireEngine
{
	void Texture::UpdateSampler()
	{
	}

	bgfx::TextureHandle Texture::CreateColorRenderTexture()
	{
		RenderTexture* texture = dynamic_cast<RenderTexture*>(this);
		RenderTextureFormat texture_format = texture->GetFormat();

		if (texture_format == RenderTextureFormat::RGBA8)
		{
			format = bgfx::TextureFormat::Enum::RGBA8;
		}
		else
		{
			IM_ASSERT(!"color format not invalid");
		}

		auto texframe = bgfx::createTexture2D(width, height, false, 1, format, BGFX_TEXTURE_RT);
		return texframe;
	}

	bgfx::TextureHandle Texture::CreateDepthRenderTexture()
	{
		RenderTexture* texture = dynamic_cast<RenderTexture*>(this);
		DepthBuffer depth = texture->GetDepth();

		switch (depth)
		{
		case DepthBuffer::Depth_16:
			format = bgfx::TextureFormat::D16;
			break;
		case DepthBuffer::Depth_24:
			format = bgfx::TextureFormat::D24;
			break;
		case DepthBuffer::Depth_24_Stencil_8:
			format = bgfx::TextureFormat::D24S8;
			break;
		case DepthBuffer::Depth_32:
			format = bgfx::TextureFormat::D32;
			break;
		default:
			format = bgfx::TextureFormat::D16;
			IM_ASSERT(!"depth format not invalid");
			break;
		}

		auto tex = bgfx::createTexture2D(width, height, false, 1, format, BGFX_TEXTURE_RT_WRITE_ONLY);
		return tex;
	}
}