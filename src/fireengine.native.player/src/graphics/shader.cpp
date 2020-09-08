#include "shader.h"
#include "loader/loader.h"
#include "renderstate.h"

#include "io/parseshader.h"

namespace FireEngine
{
	std::map<size_t, std::shared_ptr<Shader>> Shader::s_hash2shader;

	std::shared_ptr<Shader> Shader::Create(const std::string& shadername)
	{
		std::hash<std::string> hash;
		size_t hashcode = hash(shadername);

		auto iter = s_hash2shader.find(hashcode);
		if (iter != s_hash2shader.end())
			return iter->second;

		std::shared_ptr<Shader> shader(new Shader());
		ObjectManager::Register(shader, ObjectType::Asset);
		s_hash2shader[hashcode] = shader;

		ParseShader::Begin(shadername);
		{
			std::string name, nameVS, nameFS;
			if (!ParseShader::GenShader(name, nameVS, nameFS))
				return nullptr;

			shader->vs = loadShader(nameVS.c_str(), "shaders_src/");
			shader->ps = loadShader(nameFS.c_str(), "shaders_src/");

			shader->name = name;
			shader->pass.name = name;
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
		}
		ParseShader::End();



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