#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "core/object.h"
#include "shader.h"

#include <glm/glm.hpp>

namespace FireEngine
{
	class Texture2D;

	class Material : public IObject
	{
	public:
		static std::shared_ptr<Material> Create(const std::string& shadername);

		const std::shared_ptr<Shader>& GetShader()const { return shader; }

		void SetVector(const std::string& name, const glm::vec4& v);
		const glm::vec4& GetVector(const std::string& name);
		bool HasVector(const std::string& name)const;

		void SetTexture(const std::string& name, const std::shared_ptr<Texture2D>& v);
		const std::shared_ptr<Texture2D>& GetTexture(const std::string& name);
		bool HasTexture(const std::string& name)const;

		void UpdateUniforms(uint32_t pass_index);

	private:
		void SetUniform(uint32_t pass_index, const std::string& name, void* data);
		void SetUniformTexture(uint32_t pass_index, const std::string& name, uint8_t idx, const std::shared_ptr<Texture2D>& data);

	private:
		std::shared_ptr<Shader> shader;
		std::map<std::string, glm::vec4> vec4s;
		struct _Texture2DNode
		{
			uint8_t idx;
			std::shared_ptr<Texture2D> tex;
		};
		std::map<std::string, _Texture2DNode> texs;
	};
}

#endif // __MATERIAL_H__