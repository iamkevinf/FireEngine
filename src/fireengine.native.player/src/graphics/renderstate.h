#ifndef __RENDER_STATE_H__
#define __RENDER_STATE_H__

#include <stdint.h>
#include <bgfx/bgfx.h>

namespace FireEngine
{
	struct RenderState
	{
		enum class Cull : uint64_t
		{
			None  = 0,
			CW = BGFX_STATE_CULL_CW,
			CCW = BGFX_STATE_CULL_CCW
		};

		enum class ZTest : uint64_t
		{
			LESS = BGFX_STATE_DEPTH_TEST_LESS,
			LEQUAL = BGFX_STATE_DEPTH_TEST_LEQUAL,
			EQUAL = BGFX_STATE_DEPTH_TEST_EQUAL,
			GEQUAL = BGFX_STATE_DEPTH_TEST_GEQUAL,
			GREATER = BGFX_STATE_DEPTH_TEST_GREATER,
			NOTEQUAL = BGFX_STATE_DEPTH_TEST_NOTEQUAL,
			NEVER = BGFX_STATE_DEPTH_TEST_NEVER,
			ALWAYS = BGFX_STATE_DEPTH_TEST_ALWAYS
		};

		enum class ZWrite : uint64_t
		{
			R = BGFX_STATE_WRITE_R,
			G = BGFX_STATE_WRITE_G,
			B = BGFX_STATE_WRITE_R,
			A = BGFX_STATE_WRITE_A,
			Z = BGFX_STATE_WRITE_Z,
			RGB = BGFX_STATE_WRITE_RGB,
			RGBA = 0 | RGB | A,
			RGBAZ = 0 | RGB | A | Z,
		};

		enum class AlphaTest : uint64_t
		{
			On,
			Off
		};

		enum class Blend : uint64_t
		{
			ZERO          = BGFX_STATE_BLEND_ZERO,
			ONE           = BGFX_STATE_BLEND_ONE,
			SRC_COLOR     = BGFX_STATE_BLEND_SRC_COLOR,
			INV_SRC_COLOR = BGFX_STATE_BLEND_INV_SRC_COLOR,
			SRC_ALPHA     = BGFX_STATE_BLEND_SRC_ALPHA,
			INV_SRC_ALPHA = BGFX_STATE_BLEND_INV_SRC_ALPHA,
			DST_ALPHA     = BGFX_STATE_BLEND_DST_ALPHA,
			INV_DST_ALPHA = BGFX_STATE_BLEND_INV_DST_ALPHA,
			DST_COLOR     = BGFX_STATE_BLEND_DST_COLOR,
			INV_DST_COLOR = BGFX_STATE_BLEND_INV_DST_COLOR,
			SRC_ALPHA_SAT = BGFX_STATE_BLEND_SRC_ALPHA_SAT,
			FACTOR        = BGFX_STATE_BLEND_FACTOR,
			INV_FACTOR    = BGFX_STATE_BLEND_INV_FACTOR,
		};

		enum class PrimitiveType : uint64_t
		{
			None      = 0,
			TRISTRIP  = BGFX_STATE_PT_TRISTRIP,
			LINES     = BGFX_STATE_PT_LINES,
			LINESTRIP = BGFX_STATE_PT_LINESTRIP,
			POINTS    = BGFX_STATE_PT_POINTS,
		};

		enum class MSAA : uint64_t
		{
			On,
			Off
		};

		Cull cull;
		ZTest ztest;
		ZWrite zwrite;
		AlphaTest alphaTest;
		Blend src;
		Blend dst;
		Blend srcA;
		Blend dstA;
		PrimitiveType primitiveType;
		MSAA msaa;

		uint64_t GetState();

		static RenderState defaultRenderState;

		RenderState();
	};
}


#endif // __RENDER_STATE_H__