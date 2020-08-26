#include "shader.h"
#include "loader/loader.h"

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
		s_hash2shader[hashcode] = shader;

		shader->vs = loadShader("vs_buildin");
		shader->ps = loadShader("fs_buildin");
		shader->program = bgfx::createProgram(shader->vs, shader->ps, true);

		return shader;
	}
}