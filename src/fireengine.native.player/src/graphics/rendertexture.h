#ifndef __RENDER_TEXTURE_H__
#define __RENDER_TEXTURE_H__

#include "texture.h"
#include "rendertextureformat.h"
#include "depthbuffer.h"
#include "filtermode.h"

#include <list>
#include <bgfx/bgfx.h>

namespace FireEngine
{
	class RenderTexture : public Texture
	{
	public:
		static bool Init();
		static void Fini();
		void SelfFini();

		static std::shared_ptr<RenderTexture> Create(uint32_t width, uint32_t height,
			RenderTextureFormat format, DepthBuffer depth, FilterMode filter_mode);
		static std::shared_ptr<RenderTexture> GetTemporary(uint32_t width, uint32_t height,
			RenderTextureFormat format, DepthBuffer depth, FilterMode filter_mode);
		static void ReleaseTemporary(std::shared_ptr<RenderTexture> texture);

		RenderTextureFormat GetFormat()const { return format; }
		DepthBuffer GetDepth() const { return depth; }

	protected:
		void SetFormat(RenderTextureFormat format) { this->format = format; }
		void SetDepth(DepthBuffer depth) { this->depth = depth; }

	private:
		RenderTexture();

		struct Temporary
		{
			std::shared_ptr<RenderTexture> texture;
			bool in_use;
			float used_time;
		};

	private:
		static std::map<long long, std::list<Temporary>> s_temporaries;

		DepthBuffer depth;
		RenderTextureFormat format;
	};
}

#endif // __RENDER_TEXTURE_H__
