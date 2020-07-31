#pragma once

#include "FW.h"

#include "bgfx/bgfx.h"
#include "bimg/bimg.h"

FW_API bgfx::TextureHandle rcLoadTexture(const char* _name, uint64_t _flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, uint8_t _skip = 0, bgfx::TextureInfo* _info = NULL, bimg::Orientation::Enum* _orientation = NULL);
FW_API long rcCreateCube();
FW_API void rcDestroyCube(long handle);
FW_API void rcDrawCube(bgfx::ViewId viewId, long handle);
