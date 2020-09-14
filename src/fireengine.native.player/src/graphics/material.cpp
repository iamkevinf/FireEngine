#include "material.h"
#include "texture2d.h"

#include "filesystem/FileSystem.h"
#include "utils/string_tool.h"
#include "utils/log.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "exportapi.h"

namespace YAML
{
	template<>
	struct convert<glm::vec4>
	{
		static YAML::Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace FireEngine
{

	bool Material::_LoadFromFile(std::shared_ptr<Material> ptr, const std::string& path)
	{
		if (!ptr)
			return false;

		YAML::Node mat = YAML::LoadFile(path);
		if (!mat.IsDefined())
			return false;

		if (!mat["Material"])
			return false;

		if (!mat["Material"]["m_ShaderPath"])
			return false;

		std::string shadername = mat["Material"]["m_ShaderPath"].as<std::string>();
		ptr->shader = Shader::Create(shadername);

		ptr->properties_vec4.clear();
		auto savedProperties = mat["Material"]["m_SavedProperties"];
		if (savedProperties)
		{
			auto vec4s = savedProperties["m_Vec4s"];
			if (vec4s)
			{
				for (size_t i = 0; i < vec4s.size(); ++i)
				{
					auto vec4 = vec4s[i];
					if (vec4)
					{
						std::string vec4Name = vec4["m_Name"].as<std::string>();
						std::string desc = vec4["m_Desc"].as<std::string>();
						glm::vec4 value = vec4["m_Value"].as<glm::vec4>();
						std::shared_ptr<Material::MaterialPropertyNodeVec4> propertyNode = std::shared_ptr<Material::MaterialPropertyNodeVec4>(new Material::MaterialPropertyNodeVec4());
						propertyNode->name = vec4Name;
						propertyNode->desc = desc;
						propertyNode->value = value;
						ptr->properties_vec4.push_back(propertyNode);

						ptr->SetVector(vec4Name, value);
					}
				}
			}
		}
		//else
		//{
		//	YAML::Node vec;
		//	vec["m_Name"] = "u_time";
		//	vec["m_Desc"] = "time";
		//	vec["m_Value"] = glm::vec4{1, 2, 3, 4};
		//	mat["Material"]["m_SavedProperties"]["m_Vec4s"].push_back(vec);

		//	std::ofstream fout(path + ".yaml");
		//	fout << mat;
		//}

		return true;
	}

	std::shared_ptr<Material> Material::Create(const std::string& materialname)
	{
		std::shared_ptr<Material> material(new Material());

		if (!_LoadFromFile(material, materialname))
		{
			FIREENGINE_LOG_ERROR(u"%s material load error.", ToUtf16String(materialname).c_str());
			return nullptr;
		}

		ObjectManager::Register(material, ObjectType::Asset);
		material->SetName(materialname);
		return material;
	}

	void Material::SetVector(const std::string& name, const glm::vec4& v)
	{
		vec4s[name] = v;

		for (auto& ele : properties_vec4)
		{
			if (ele->name == name)
				ele->value = v;
		}
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
		texs[name] = { idx , v };
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
		for (Uniform& uniform : pass.uniforms)
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
		for (Uniform& uniform : pass.uniforms)
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

	EXPORT_API Shader* MaterialGetShader(Material* mat)
	{
		if (!mat)
			return nullptr;
		return mat->GetShader().get();
	}

	EXPORT_API const char* MaterialGetName(Material* mat)
	{
		if (!mat)
			return nullptr;
		return mat->name.c_str();
	}

	EXPORT_API Material* MaterialCreate(const char16_t* name)
	{
		std::string shadername = ToUtf8String(std::u16string(name));
		auto ptr = Material::Create(shadername);
		return ptr.get();
	}

	EXPORT_API void MaterialSetVector(Material* mat, const char16_t* name, const glm::vec4& v)
	{
		if (!mat)
			return;

		std::u16string _name(name);
		std::string name8 = ToUtf8String(_name);

		mat->SetVector(name8, v);
	}

	EXPORT_API const glm::vec4& MaterialGetVector(Material* mat, const char16_t* name)
	{
		if (!mat)
			return { 0,0,0,0 };

		std::u16string _name(name);
		std::string name8 = ToUtf8String(_name);

		return mat->GetVector(name8);
	}

	EXPORT_API bool MaterialHasVector(Material* mat, const char16_t* name)
	{
		if (!mat)
			return false;

		std::u16string _name(name);
		std::string name8 = ToUtf8String(_name);

		return mat->HasVector(name8);
	}

	EXPORT_API void MaterialSetTexture(Material* mat, const char16_t* name, const std::shared_ptr<Texture2D>& v)
	{
		if (!mat)
			return;

		std::u16string _name(name);
		std::string name8 = ToUtf8String(_name);

		mat->SetTexture(name8, v);
	}

	EXPORT_API const std::shared_ptr<Texture2D>& MaterialGetTexture(Material* mat, const char16_t* name)
	{
		if (!mat)
			return nullptr;

		std::u16string _name(name);
		std::string name8 = ToUtf8String(_name);

		return mat->GetTexture(name8);
	}

	EXPORT_API bool MaterialHasTexture(Material* mat, const char16_t* name)
	{
		if (!mat)
			return false;

		std::u16string _name(name);
		std::string name8 = ToUtf8String(_name);

		return mat->HasTexture(name8);
	}

	EXPORT_API int MaterialGetPropertyVec4Count(Material* mat)
	{
		if (!mat)
			return -1;

		return (int)mat->GetPropertiesVec4().size();
	}

	EXPORT_API Material::MaterialPropertyNodeVec4* MaterialGetPropertyVec4(Material* mat, int index)
	{
		if (!mat)
			return nullptr;

		auto pool = mat->GetPropertiesVec4();
		if (index >= pool.size())
			return nullptr;

		return pool[index].get();
	}

	EXPORT_API const char* MaterialGetPropertyVec4Name(Material::MaterialPropertyNodeVec4* node)
	{
		if (!node)
			return nullptr;

		return node->name.c_str();
	}

	EXPORT_API const char* MaterialGetPropertyVec4Desc(Material::MaterialPropertyNodeVec4* node)
	{
		if (!node)
			return nullptr;

		return node->desc.c_str();
	}

	EXPORT_API glm::vec4 MaterialGetPropertyVec4Value(Material::MaterialPropertyNodeVec4* node)
	{
		if (!node)
			return {};

		return node->value;
	}

}