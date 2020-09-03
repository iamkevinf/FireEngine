#include "texture2d.h"

#include "loader/loader.h"

namespace FireEngine
{
	std::shared_ptr<Texture2D> Texture2D::Create(
		int width,
		int height,
		bgfx::TextureFormat::Enum format,
		TextureWrapMode wrap_mode,
		FilterMode filter_mode,
		bool mipmap,
		const ByteBuffer& colors)
	{
		std::shared_ptr<Texture2D> texture = std::shared_ptr<Texture2D>(new Texture2D());
		texture->SetWidth(width);
		texture->SetHeight(height);
		texture->SetFormat(format);
		texture->SetWrapMode(wrap_mode);
		texture->SetFilterMode(filter_mode);
		texture->SetMipmap(mipmap);
		texture->colors = colors;

		return texture;
	}


	std::shared_ptr<Texture2D> Texture2D::LoadFromFile(const std::string& file,
		TextureWrapMode wrap_mode,
		FilterMode filter_mode,
		bool mipmap)
	{
		std::shared_ptr<Texture2D> texture;

		bgfx::TextureInfo info;
		ByteBuffer buffer;
		auto handle = loadTexture(file.c_str(), 0, 0, &info, nullptr, &buffer);
		if (!bgfx::isValid(handle))
			return nullptr;

		texture = Texture2D::Create(info.width, info.height, info.format,
			wrap_mode, filter_mode, info.numMips > 1, buffer);
		texture->texture_handle = handle;

		return texture;
	}

	Texture2D::Texture2D()
	{
		SetName("Texture2D");
	}
}