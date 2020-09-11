#ifndef __PROJECT_H__
#define __PROJECT_H__

#include <string>

namespace FireEngine
{
	class Project
	{
	public:
		static Project* getInstance();

		void SetPath(const std::string& _path) { pathProject = _path; };
		std::string GetProjectPath()const { return pathProject; }
		std::string GetEnginePath()const { return "./"; }
		std::string GetPath()const
		{
			auto path = GetProjectPath();
			return path == "" ? GetEnginePath() : path;
		}

	private:
		static Project s_instance;
		std::string pathProject;
	};
}

#endif // __PROJECT_H__