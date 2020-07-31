#include "RC_wrapper.h"

#include <cube.h>
#include <loader.h>

FW_API bgfx::TextureHandle rcLoadTexture(const char* _name, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
{
	return RC::loadTexture(_name, _flags, _skip, _info, _orientation);
}

FW_API long rcCreateCube()
{
	return Cube::Create();
}

FW_API void rcDestroyCube(long handle)
{
	return Cube::Destroy(handle);
}

FW_API void rcDrawCube(bgfx::ViewId view, long handle)
{
	return Cube::DrawCube(view, handle);
}


