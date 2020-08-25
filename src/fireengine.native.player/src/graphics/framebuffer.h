#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__

#include <memory>

namespace FireEngine
{
	class RenderTexture;

	struct FrameBuffer
	{
		std::shared_ptr<RenderTexture> color_texture;
		std::shared_ptr<RenderTexture> depth_texture;
	};
}

#endif // __FRAME_BUFFER_H__