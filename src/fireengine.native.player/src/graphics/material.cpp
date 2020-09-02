#include "material.h"

namespace FireEngine
{
	std::shared_ptr<Material> Material::Create(const std::string& shadername)
	{
		std::shared_ptr<Material> material(new Material());
		material->shader = Shader::Create(shadername);
		ObjectManager::Register(material, ObjectType::Asset);
		return material;
	}

	void Material::SetVector(const std::string& name, const glm::vec4& v)
	{
		vec4s[name] = v;


	}

	const glm::vec4& Material::GetVector(const std::string& name)
	{
		return vec4s[name];
	}

	bool Material::HasVector(const std::string& name)const
	{
		auto iter = vec4s.find(name);
		return iter != vec4s.end();
	}

	void Material::SetUniform(uint32_t pass_index, const std::string& name, void* data)
	{
		ShaderPass pass = shader->pass;
		for (Uniform& uniform : pass.unoforms)
		{
			if (uniform.name == name)
			{
				bgfx::setUniform(uniform.handle, data, uniform.num);
			}
		}
	}

	void Material::UpdateUniforms(uint32_t pass_index)
	{
		for (auto& i : vec4s)
		{
			SetUniform(pass_index, i.first, (void*)(&i.second));
		}
	}
}