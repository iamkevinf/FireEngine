#ifndef __LOADER_H__
#define __LOADER_H__

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include <memory>
#include <vector>

struct aiMesh;
struct aiScene;
class ByteBuffer;
class Material;

namespace FireEngine
{
	class Mesh;

	bgfx::TextureHandle loadTexture(const char* _name, uint64_t _flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, uint8_t _skip = 0, bgfx::TextureInfo* _info = NULL, bimg::Orientation::Enum* _orientation = NULL, ByteBuffer* mem = NULL);
	bgfx::ShaderHandle loadShader(const char* name, const char* prefix);
	void loadMesh(const char* path, std::shared_ptr<Mesh> mesh, std::vector<std::shared_ptr<Material>>& mats);
}; // end of namespace RC

#endif //__LOADER_H__

