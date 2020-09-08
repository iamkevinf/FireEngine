#ifndef __PARSE_SHADER_H__
#define __PARSE_SHADER_H__

#include <string>
#include <vector>

namespace FireEngine
{
	class ParseShader
	{
	public:
		static void Begin(const std::string& path);
		static void End();
		static bool ReadShaderName(std::string& name);

		static bool ReadShaderFuncName(std::string& nameVS, std::string& nameFS, std::string& nameVarying);

		static bool GenShader(std::string& shadername, std::string& nameVS, std::string& nameFS);
	private:
		static uint32_t size;
		static void* data;
		static std::string content;
	};
}

#endif // __PARSE_SHADER_H__