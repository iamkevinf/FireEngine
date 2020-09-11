#include "shader.h"

#include "loader/loader.h"

#include "io/parseshader.h"

#include "utils/string_tool.h"

#include "renderstate.h"

#include "app/project.h"

#include "exportapi.h"

namespace FireEngine
{
	struct HashStrPair
	{
		size_t hash;
		std::string str;
	};
	std::vector<HashStrPair> pool;
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

		std::string projectPath = Project::getInstance()->GetPath();
		std::string shaderFullname = projectPath + "/Assets/" + shadername;
		std::string name, nameVS, nameFS;
		if (!ParseShader::CompileShader(shaderFullname, nameVS, nameFS))
			return nullptr;

		std::string libararyPath = projectPath + "/Library";
		std::string prefix = libararyPath + "/shaderCache/";
		shader->vs = loadShader(nameVS.c_str(), prefix.c_str());
		shader->ps = loadShader(nameFS.c_str(), prefix.c_str());

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

		s_hash2shader[hashcode] = shader;
		pool.push_back({ hashcode, shadername });

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

	EXPORT_API Shader* ShaderCreate(const char16_t* name)
	{
		std::string shadername = ToUtf8String(std::u16string(name));
		auto shaderPtr = Shader::Create(shadername);
		return shaderPtr.get();
	}

	static std::string tmpNameList = "";
	EXPORT_API const char* ShaderGetNameList(int& size)
	{
		tmpNameList = "";
		for (int i = 0; i < pool.size(); ++i)
		{
			auto ele = pool[i];
			tmpNameList = tmpNameList + ele.str;
			if (i < pool.size() - 1)
				tmpNameList = tmpNameList + ",";
		}
		size = tmpNameList.size();
		return tmpNameList.c_str();
	}
}