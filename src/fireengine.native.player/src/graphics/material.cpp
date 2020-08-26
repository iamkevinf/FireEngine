#include "material.h"

namespace FireEngine
{
	std::shared_ptr<Material> Material::Create(const std::string& shadername)
	{
		std::shared_ptr<Material> material(new Material());
		material->shader = Shader::Create(shadername);
		return material;
	}
}