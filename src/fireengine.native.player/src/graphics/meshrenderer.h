#ifndef __MESH_RENDERER_H__
#define __MESH_RENDERER_H__

#include "renderer.h"
#include "mesh.h"

namespace FireEngine
{
	class MeshRenderer : public Renderer
	{
		DECLARE_COM_CLASS(MeshRenderer, Renderer);
	public:
		virtual const VertexBuffer* GetVertexBuffer()const;
		virtual const IndexBuffer* GetIndexBuffer()const;
		virtual bgfx::VertexBufferHandle GetVertexBufferHandle()const;
		virtual bgfx::IndexBufferHandle GetIndexBufferHandle()const;

		virtual bool IsValidPass(uint32_t material_index) const;

		void SetSharedMesh(const std::shared_ptr<Mesh>& mesh) { this->mesh = mesh; }
		const std::shared_ptr<Mesh>& GetSharedMesh() const { return mesh; }

	private:
		std::shared_ptr<Mesh> mesh;
	};
}

#endif // __MESH_ERNDERER_H__
