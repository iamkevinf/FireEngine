#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "core/object.h"
#include "texturewrapmode.h"
#include "filtermode.h"
#include <bgfx/bgfx.h>

namespace FireEngine
{
	class Texture : public IObject
	{
	public:
		virtual ~Texture();
		uint32_t GetWidth()const { return width; }
		uint32_t GetHeight()const { return height; }
		TextureWrapMode GetWrapMode() const { return wrap_mode; }
		void SetWrapMode(TextureWrapMode mode) { wrap_mode = mode; }
		FilterMode GetFilterMode() const { return filter_mode; }
		void SetFilterMode(FilterMode mode) { filter_mode = mode; }
		void UpdateSampler();

		bgfx::TextureHandle GetTextureHandle()const { return texture_handle; }

	protected:
		bgfx::TextureHandle CreateColorRenderTexture();
		bgfx::TextureHandle CreateDepthRenderTexture();

		void SetWidth(uint32_t width) { this->width = width; }
		void SetHeight(uint32_t height) { this->height = height; }

	protected:
		uint32_t width = 0;
		uint32_t height = 0;
		TextureWrapMode wrap_mode;
		FilterMode filter_mode;
		bgfx::TextureFormat::Enum format;
		bgfx::TextureHandle texture_handle;
	};
}

#endif // __TEXTURE_H__