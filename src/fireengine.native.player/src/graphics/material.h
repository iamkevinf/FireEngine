#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "core/object.h"
#include "shader.h"

namespace FireEngine
{
	class Material : public IObject
	{
	public:
		static std::shared_ptr<Material> Create(const std::string& shadername);

		const std::shared_ptr<Shader>& GetShader()const { return shader; }

	public:

	private:
		std::shared_ptr<Shader> shader;
	};
}

#endif // __MATERIAL_H__