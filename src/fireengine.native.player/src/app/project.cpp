#include "project.h"

#include "exportapi.h"
#include "utils/string_tool.h"

namespace FireEngine
{

	Project Project::s_instance;

	Project* Project::getInstance()
	{
		return &s_instance;
	}

	EXPORT_API void SetProjectPath(const char16_t* path)
	{
		std::string _path = ToUtf8String(std::u16string(path));
		Project::getInstance()->SetPath(_path);
	}

	EXPORT_API std::u16string GetProjectPath()
	{
		std::string path = Project::getInstance()->GetProjectPath();
		return ToUtf16String(path);
	}
}