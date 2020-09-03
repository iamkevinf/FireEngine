#include "rendertexture.h"
#include "utils/time.h"
#include <bgfx/bgfx.h>

namespace FireEngine
{
	std::map<long long, std::list<RenderTexture::Temporary>> RenderTexture::s_temporaries;

	bool RenderTexture::Init()
	{
		return true;
	}

	void RenderTexture::Fini()
	{

	}

	void RenderTexture::SelfFini()
	{
		if (bgfx::isValid(texture_handle))
		{
			bgfx::destroy(texture_handle);
			texture_handle = BGFX_INVALID_HANDLE;
		}
	}


	std::shared_ptr<RenderTexture> RenderTexture::GetTemporary(uint32_t width, uint32_t height,
		RenderTextureFormat format, DepthBuffer depth, FilterMode filter_mode)
	{
		std::shared_ptr<RenderTexture> texture;

		long long w = width;
		long long h = height;
		long long f = (long long)format;
		long long d = (long long)depth;
		long long key = (w << 0) | (h << 16) | (f << 32) | (d << 48);

		std::list<Temporary>* list = nullptr;
		auto iter = s_temporaries.find(key);
		if (iter != s_temporaries.end())
		{
			list = &iter->second;
			for (auto& i : *list)
			{
				if (!i.in_use)
				{
					texture = i.texture;
					i.in_use = true;

					if (texture->GetFilterMode() != filter_mode)
					{
						texture->SetFilterMode(filter_mode);
						texture->UpdateSampler();
					}
					break;
				}
			}
		}

		if (!texture)
		{
			texture = Create(width, height, format, depth, filter_mode);

			Temporary t;
			t.texture = texture;
			t.in_use = true;

			if (list != nullptr)
			{
				list->push_back(t);
			}
			else
			{
				std::list<Temporary> list_neo;
				list_neo.push_back(t);
				s_temporaries[key] = list_neo;
			}
		}

		return texture;
	}

	void RenderTexture::ReleaseTemporary(std::shared_ptr<RenderTexture> texture)
	{
		long long w = texture->GetWidth();
		long long h = texture->GetHeight();
		long long f = (long long)texture->GetFormat();
		long long d = (long long)texture->GetDepth();
		long long key = (w << 0) | (h << 16) | (f << 32) | (d << 48);

		std::list<Temporary>* list;
		auto iter = s_temporaries.find(key);
		if (iter != s_temporaries.end())
		{
			list = &iter->second;

			for (auto& i : *list)
			{
				if (i.texture == texture)
				{
					i.in_use = false;
					i.used_time = Time::GetTime();
					break;
				}
			}
		}
	}

	std::shared_ptr<RenderTexture> RenderTexture::Create(uint32_t width, uint32_t height,
		RenderTextureFormat format, DepthBuffer depth, FilterMode filter_mode)
	{
		std::shared_ptr<RenderTexture> texture = std::shared_ptr<RenderTexture>(new RenderTexture());
		texture->SetWidth(width);
		texture->SetHeight(height);
		texture->SetFormat(format);
		texture->SetDepth(depth);
		texture->SetWrapMode(TextureWrapMode::Clamp);
		texture->SetFilterMode(filter_mode);

		if (format == RenderTextureFormat::Depth)
			texture->texture_handle = texture->CreateDepthRenderTexture();
		else
			texture->texture_handle = texture->CreateColorRenderTexture();

		return texture;
	}

	RenderTexture::RenderTexture()
	{
		SetName("RenderTexture");
	}
}