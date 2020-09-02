#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "core/object.h"
#include "shader.h"

#include <glm/glm.hpp>

namespace FireEngine
{
	class Material : public IObject
	{
	public:
		static std::shared_ptr<Material> Create(const std::string& shadername);

		const std::shared_ptr<Shader>& GetShader()const { return shader; }

		void SetVector(const std::string& name, const glm::vec4& v);
		const glm::vec4& GetVector(const std::string& name);
		bool HasVector(const std::string& name)const;

		void UpdateUniforms(uint32_t pass_index);

	private:
		void SetUniform(uint32_t pass_index, const std::string& name, void* data);

	private:
		std::shared_ptr<Shader> shader;
		std::map<std::string, glm::vec4> vec4s;
	};
}

#endif // __MATERIAL_H__