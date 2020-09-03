#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__

#include <memory>
#include "rendertexture.h"

namespace FireEngine
{
	class RenderTexture;

	struct FrameBuffer
	{
		std::shared_ptr<RenderTexture> color_texture;
		std::shared_ptr<RenderTexture> depth_texture;
		void Fini()
		{
			if (color_texture)
				color_texture->SelfFini();
			if (depth_texture)
				depth_texture->SelfFini();
		}
	};
}

#endif // __FRAME_BUFFER_H__