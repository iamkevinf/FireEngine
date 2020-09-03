#include "shader.h"
#include "loader/loader.h"
#include "renderstate.h"

namespace FireEngine
{
	std::map<size_t, std::shared_ptr<Shader>> Shader::s_hash2shader;

	std::shared_ptr<Shader> Shader::Create(const std::string& shadername)
	{
		std::hash<std::string> hash;
		size_t hashcode = hash(shadername);

		auto iter = s_hash2shader.find(hashcode);
		if (iter != s_hash2shader.find(hashcode))
			return iter->second;

		std::shared_ptr<Shader> shader(new Shader());
		ObjectManager::Register(shader, ObjectType::Asset);
		s_hash2shader[hashcode] = shader;

		shader->vs = loadShader("vs_buildin");
		shader->ps = loadShader("fs_buildin");

		shader->name = shadername;
		shader->pass.name = shadername;
		shader->pass.program = bgfx::createProgram(shader->vs, shader->ps, true);
		shader->pass.rs = RenderState::defaultRenderState;

		bgfx::UniformHandle handlearray[256];
		int countVS = bgfx::getShaderUniforms(shader->vs, handlearray, 256);
		int countFS = bgfx::getShaderUniforms(shader->ps, handlearray + countVS, 256 - countVS);
		bgfx::UniformInfo info;
		for (auto i = 0; i < countVS + countFS; i++)
		{
			bgfx::getUniformInfo(handlearray[i], info);
			Uniform uniform;
			uniform.name = info.name;
			uniform.handle = handlearray[i];
			uniform.num = info.num;
			uniform.type = info.type;
			shader->pass.uniforms.push_back(uniform);
		}

		return shader;
	}

	Shader::~Shader()
	{
		//if (bgfx::isValid(pass.program))
		//{
		//	bgfx::destroy(pass.program);
		//	pass.program = BGFX_INVALID_HANDLE;
		//}
	}
}