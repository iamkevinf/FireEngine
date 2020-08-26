#ifndef __SHADER_H__
#define __SHADER_H__

#include "core/object.h"

#include <bgfx/bgfx.h>

namespace FireEngine
{
	class Shader : public IObject
	{
	public:
		static std::shared_ptr<Shader> Create(const std::string& shadername);

		bgfx::ShaderHandle vs;
		bgfx::ShaderHandle ps;
		bgfx::ProgramHandle program;

	private:
		static std::map<size_t, std::shared_ptr<Shader>> s_hash2shader;
	};
}

#endif // __SHADER_H__