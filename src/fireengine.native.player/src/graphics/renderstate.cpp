#include "renderstate.h"

namespace FireEngine
{
	RenderState RenderState::defaultRenderState;

	RenderState::RenderState()
	{
		cull = Cull::CCW;
		ztest = ZTest::LESS;
		src = Blend::ONE;
		dst = Blend::ZERO;
		srcA = Blend::ONE;
		dstA = Blend::ZERO;
		primitiveType = PrimitiveType::None;
		msaa = MSAA::On;
		alphaTest = AlphaTest::Off;
		zwrite = ZWrite::RGBAZ;
	}

	uint64_t RenderState::GetState()
	{
		uint64_t state = 0
			| (uint64_t)zwrite
			| (uint64_t)ztest
			| (uint64_t)cull
			| (uint64_t)BGFX_STATE_BLEND_FUNC_SEPARATE((uint64_t)src, (uint64_t)dst, (uint64_t)srcA, (uint64_t)dstA)
			| (uint64_t)primitiveType
			| (msaa == MSAA::On ? BGFX_STATE_MSAA : 0);

		return state;
	}
}
