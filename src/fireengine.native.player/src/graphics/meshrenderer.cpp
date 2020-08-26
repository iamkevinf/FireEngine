#include "meshrenderer.h"

namespace FireEngine
{
	DEFINE_COM_CLASS(MeshRenderer);

	void MeshRenderer::DeepCopy(const std::shared_ptr<IObject>& source)
	{
		Renderer::DeepCopy(source);

		auto src = std::dynamic_pointer_cast<MeshRenderer>(source);
		SetSharedMesh(src->GetSharedMesh());
	}

	const VertexBuffer* MeshRenderer::GetVertexBuffer() const
	{
		return GetSharedMesh()->GetVertexBuffer().get();
	}

	const IndexBuffer* MeshRenderer::GetIndexBuffer() const
	{
		return GetSharedMesh()->GetIndexBuffer().get();
	}

	bgfx::VertexBufferHandle MeshRenderer::GetVertexBufferHandle() const
	{
		return GetSharedMesh()->GetVertexBufferHandle();
	}

	bgfx::IndexBufferHandle MeshRenderer::GetIndexBufferHandle() const
	{
		return GetSharedMesh()->GetIndexBufferHandle();
	}

	bool MeshRenderer::IsValidPass(uint32_t material_index) const
	{
		if (mesh)
		{
			auto submesh_count = mesh->submeshes.size();

			if (material_index == 0)
				return true;
			else if (material_index > 0 && submesh_count > 0 && material_index < submesh_count)
				return true;
		}

		return false;
	}
}