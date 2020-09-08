#include "parseshader.h"
#include "filesystem/FileSystem.h"

#include "utils/log.h"
#include "utils/string_tool.h"

#include <bx/process.h>

#include <iostream>
#include <sstream>

namespace FireEngine
{
	void* ParseShader::data = nullptr;
	uint32_t ParseShader::size = 0;
	std::string ParseShader::content = "";
	static std::string curPath;

	void ParseShader::Begin(const std::string& path)
	{
		data = FileSystem::getInstance()->load(
			getFileReader(), getAllocator(), path.c_str(), &size);

		content = std::string((const char*)data, size);
		curPath = path;

	}
	bool ParseShader::ReadShaderName(std::string& name)
	{
		if (content.empty())
			return false;

		size_t index = content.find('"');
		if (index == content.npos)
			return false;

		size_t index_end = content.find('"', index + 1);
		if (index_end == content.npos)
			return false;

		name = content.substr(index + 1, index_end - index - 1);
		return name.length() > 0;
	}

	bool ParseShader::ReadShaderFuncName(std::string& nameVS, std::string& nameFS, std::string& nameVarying)
	{
		if (content.empty())
			return false;

		size_t macroPragmaIndex = content.find("#pragma");
		if (macroPragmaIndex == content.npos)
			return false;
		macroPragmaIndex += 8;
		size_t vertexIndex = content.find("vertex", macroPragmaIndex, 6);
		if (vertexIndex == content.npos)
			return false;

		size_t index = vertexIndex + 7;
		while (content[index] == ' ')
			index++;

		size_t index_end = index;
		while (content[index_end] != ' '
			&& content[index_end] != '\r'
			&& content[index_end] != '\n')
			index_end++;

		if (index_end <= index)
			return false;

		nameVS = content.substr(index, index_end - index);

		if (nameVS.empty())
			return false;

		size_t fragmentIndex = content.find("fragment", index_end, 8);
		if (fragmentIndex < 0)
			return false;

		index = fragmentIndex + 9;
		while (content[index] == ' ')
			index++;

		index_end = index;
		while (content[index_end] != ' '
			&& content[index_end] != '\r'
			&& content[index_end] != '\n')
			index_end++;

		nameFS = content.substr(index, index_end - index);
		if (nameFS.empty())
			return false;


		size_t varyingIndex = content.find("varying", index_end, 7);
		if (varyingIndex < 0)
			return false;

		index = varyingIndex + 9;
		while (content[index] == ' ')
			index++;

		index_end = index;
		while (content[index_end] != ' '
			&& content[index_end] != '\r'
			&& content[index_end] != '\n')
			index_end++;

		nameVarying = content.substr(index, index_end - index);
		if (nameVarying.empty())
			return false;

		return true;
	}

	bool CompileShader(std::string args)
	{
		std::string exe = "shadercRelease";
		bx::Error error;
		bx::ProcessReader processReader;
		if (!processReader.open(exe.c_str(), args.c_str(), &error) )
		{
			FIREENGINE_LOG_ERROR(u"Unable to load shader file");
			return false;
		}

		char buffer[2048];
		bx::memSet(buffer, 0, 2048);
		int32_t numCharactersWritten = processReader.read(buffer, 2048, &error);
		if (!error.isOk())
		{
			if (error.get().code != bx::kErrorReaderWriterEof.code)
			{
				std::basic_ostringstream<char16_t> msg;
				msg << u"Unable to load shader file:" << error.getMessage().getPtr();
				std::u16string strMsg = msg.str();
				FIREENGINE_LOG_ERROR(strMsg.c_str());
				return false;
			}
		}


		if (buffer[0] != '\0')
		{
			std::basic_ostringstream<char16_t> msg;
			msg << u"" << buffer;
			std::u16string strMsg = msg.str();

			FIREENGINE_LOG_ERROR(u"shader compile error %s", strMsg.c_str());
			return false;
		}

		int32_t result = processReader.getExitCode();
		processReader.close();

		if (0 != result)
		{

			std::basic_ostringstream<char16_t> msg;
			msg << u"Unable to compile shader file: :" << error.getMessage().getPtr();
			std::u16string strMsg = msg.str();
			FIREENGINE_LOG_ERROR(strMsg.c_str());
			return false;
		}

		return true;
	}

	bool ParseShader::GenShader(std::string& shadername, std::string& nameVSWithoutExt, std::string& nameFSWithoutExt)
	{
		if (content.empty())
			return false;

		if (!ReadShaderName(shadername))
			return false;

		std::string nameVS, nameFS, nameVarying;
		if (!ReadShaderFuncName(nameVS, nameFS, nameVarying))
			return false;

		std::string prefix = "";

		size_t idx = curPath.find_last_of('/');
		if (idx != curPath.npos)
			prefix = curPath.substr(0, idx);

		std::string shader_pathi = prefix;
		std::string shader_patho = prefix + "/shaders/dx11";
		std::string vs_profile = "vs_4_0";
		std::string plantform = "windows";

		nameVSWithoutExt = RemoveFileExt(nameVS);
		std::string argsVS = "-f " + shader_pathi + "/" + nameVS + " -o " + shader_patho + "/" + nameVSWithoutExt + ".bin --varying.def " + shader_pathi + "/" + nameVarying + " --type vertex --platform " + plantform + " --profile " + vs_profile + " -O 3";
		if (!CompileShader(argsVS))
			return false;

		nameFSWithoutExt = RemoveFileExt(nameFS);
		std::string fs_profile = "ps_4_0";
		std::string argsFS = "-f " + shader_pathi + "/" + nameFS + " -o " + shader_patho + "/" + nameFSWithoutExt + ".bin --varying.def " + shader_pathi + "/" + nameVarying + " --type fragment --platform " + plantform + " --profile " + fs_profile + " -O 3";
		return CompileShader(argsFS);
	}

	void ParseShader::End()
	{
		content = "";
		data = nullptr;
		size = 0;
	}
}