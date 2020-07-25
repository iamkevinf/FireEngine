#ifndef __LOADER_H__
#define __LOADER_H__

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

namespace RC
{
	bgfx::TextureHandle loadTexture(const char* _name, uint64_t _flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, uint8_t _skip = 0, bgfx::TextureInfo* _info = NULL, bimg::Orientation::Enum* _orientation = NULL);
}; // end of namespace RC

#endif //__LOADER_H__

