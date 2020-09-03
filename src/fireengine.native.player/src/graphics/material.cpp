#include "material.h"
#include "texture2d.h"

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


	void Material::SetTexture(const std::string& name, const std::shared_ptr<Texture2D>& v)
	{
		uint8_t idx = (uint8_t)texs.size();
		texs[name] = { idx , v};
	}

	const std::shared_ptr<Texture2D>& Material::GetTexture(const std::string& name)
	{
		return texs[name].tex;
	}

	bool Material::HasTexture(const std::string& name)const
	{
		auto iter = texs.find(name);
		return iter != texs.end();
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

	void Material::SetUniformTexture(uint32_t pass_index, const std::string& name, uint8_t idx, const std::shared_ptr<Texture2D>& texture)
	{
		ShaderPass pass = shader->pass;
		for (Uniform& uniform : pass.unoforms)
		{
			if (uniform.name == name)
			{
				bgfx::setTexture(idx, uniform.handle, texture->GetTextureHandle());
			}
		}
	}


	void Material::UpdateUniforms(uint32_t pass_index)
	{
		for (auto& i : vec4s)
		{
			SetUniform(pass_index, i.first, (void*)(&i.second));
		}

		for (auto& i : texs)
		{
			SetUniformTexture(pass_index, i.first, i.second.idx, i.second.tex);
		}
	}
}