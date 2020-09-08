#ifndef __WRAPPER_H__
#define __WRAPPER_H__

#include "bgfx/bgfx.h"
#include "bimg/bimg.h"
#include "utils/bytebuffer.h"

#include "exportapi.h"

namespace FireEngine
{
	EXPORT_API int feApp_MainLoop(const char16_t* title, void(*OnInit)(), void(*OnGUI)(), void(*OnTick)(), void(*OnExit)());
	EXPORT_API bool feApp_SetWindowTitle(const char16_t* title);

	EXPORT_API bgfx::TextureHandle feLoadTexture(const char* _name, uint64_t _flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, uint8_t _skip = 0, bgfx::TextureInfo* _info = NULL, bimg::Orientation::Enum* _orientation = NULL, ByteBuffer* mem = NULL);
	EXPORT_API bgfx::ShaderHandle feLoadShader(const char* name, const char* prefix);

	EXPORT_API void feApp_ShowState();

	struct Target;
	struct TargetHandle
	{
		Target* ptr;
	};

	EXPORT_API TargetHandle feTarget_Create(int width, int height);
	EXPORT_API void feTarget_Destory(TargetHandle handle);
	EXPORT_API void feTarget_Reset(TargetHandle handle, int width, int height);
	EXPORT_API void* feTarget_GetImGuiTexID(TargetHandle handle);

	EXPORT_API void feFrame_SetTarget(TargetHandle handle);
	EXPORT_API void feFrame_SetMainScreen();

	EXPORT_API void feFrame_Clear(uint32_t color, bool withColor, bool withDepth);
	EXPORT_API void feFrame_Flush();


}

#endif // __WRAPPER_H__