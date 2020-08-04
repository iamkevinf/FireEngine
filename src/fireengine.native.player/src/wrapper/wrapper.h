#ifndef __WRAPPER_H__
#define __WRAPPER_H__

#include "bgfx/bgfx.h"
#include "bimg/bimg.h"

#define EXPORT_API extern "C" __declspec(dllexport)

namespace FireEngine
{
	EXPORT_API int feApp_MainLoop(const char16_t* title, void(*OnInit)(), void(*OnGUI)(), void(*OnExit)());
	EXPORT_API bool feApp_SetWindowTitle(const char16_t* title);

	EXPORT_API bgfx::TextureHandle feLoadTexture(const char* _name, uint64_t _flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, uint8_t _skip = 0, bgfx::TextureInfo* _info = NULL, bimg::Orientation::Enum* _orientation = NULL);
	EXPORT_API bgfx::ShaderHandle feLoadShader(const char* name);

}

#endif // __WRAPPER_H__