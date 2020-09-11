#ifndef __PARSE_SHADER_H__
#define __PARSE_SHADER_H__

#include <string>
#include <vector>

namespace FireEngine
{
	class ParseShader
	{
	public:
		static bool CompileShader(const std::string& path, std::string& nameVS, std::string& nameFS);
	};
}

#endif // __PARSE_SHADER_H__