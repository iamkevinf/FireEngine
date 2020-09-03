#ifndef __TEXTURE_2D_H__
#define __TEXTURE_2D_H__

#include "texture.h"
#include "utils/bytebuffer.h"

#include <bgfx/bgfx.h>

namespace FireEngine
{
	class Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> LoadFromFile(const std::string& file,
			TextureWrapMode wrapMode = TextureWrapMode::Clamp,
			FilterMode filterMode = FilterMode::Bilinear,
			bool mipmap = false);

		static std::shared_ptr<Texture2D> Create(
			int width,
			int height,
			bgfx::TextureFormat::Enum format,
			TextureWrapMode wrap_mode,
			FilterMode filter_mode,
			bool mipmap,
			const ByteBuffer& colors);


		ByteBuffer GetColors() const { return colors; }
		bgfx::TextureFormat::Enum GetFormat() const { return format; }
		bool IsMipmap() const { return mipmap; }

	private:
		Texture2D();
		void SetFormat(bgfx::TextureFormat::Enum format) { this->format = format; }
		void SetMipmap(bool mipmap) { this->mipmap = mipmap; }

	private:
		bgfx::TextureFormat::Enum format;
		bool mipmap;
		ByteBuffer colors;
	};
}

#endif // __TEXTURE_2D_H__