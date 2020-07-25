#include "RC_wrapper.h"

#include "RC_utils.h"

#include "loader.h"

RC_API bgfx::TextureHandle rcLoadTexture(const char* _name, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
{
	return RC::loadTexture(_name, _flags, _skip, _info, _orientation);
}