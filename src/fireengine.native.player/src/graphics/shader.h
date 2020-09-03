#ifndef __SHADER_H__
#define __SHADER_H__

#include "core/object.h"
#include "renderstate.h"

#include <bgfx/bgfx.h>

namespace FireEngine
{
	struct Uniform
	{
		std::string name;
		uint16_t num;
		bgfx::UniformType::Enum type;
		bgfx::UniformHandle handle;
	};

	struct ShaderPass
	{
		std::string name;
		bgfx::ProgramHandle program;
		std::vector<Uniform> uniforms;
		RenderState rs;
	};

	class Shader : public IObject
	{
	public:
		virtual ~Shader();
		static std::shared_ptr<Shader> Create(const std::string& shadername);

		bgfx::ShaderHandle vs;
		bgfx::ShaderHandle ps;

		// ��ʱ��ֻ֧��һ��pass
		ShaderPass pass;

	private:
		static std::map<size_t, std::shared_ptr<Shader>> s_hash2shader;
	};
}

#endif // __SHADER_H__