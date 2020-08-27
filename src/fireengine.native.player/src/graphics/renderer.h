#ifndef __REDERER_H__
#define __REDERER_H__

#include "core/component.h"
#include <list>
#include <map>
#include <bgfx/bgfx.h>

namespace FireEngine
{
	class Material;
	class VertexBuffer;
	class IndexBuffer;
	class Camera;

	class Renderer : public Component
	{
		DECLARE_COM_CLASS_ABSTRACT(Renderer, Component);
	public:

		static bool IsRenderersDirty();
		static void SetRenderersDirty(bool dirty);
		static std::list<Renderer*>& GetRenderers();
		static void SetCullingDirty(Camera* cam);

		static void PrepareAllPass();
		static void RenderAllPass();

		static void ClearPasses();

		virtual const VertexBuffer* GetVertexBuffer() const = 0;
		virtual const IndexBuffer* GetIndexBuffer() const = 0;
		virtual bgfx::VertexBufferHandle GetVertexBufferHandle() const = 0;
		virtual bgfx::IndexBufferHandle GetIndexBufferHandle() const = 0;
		virtual bgfx::DynamicVertexBufferHandle GetDynamicVertexBufferHandle() const = 0;
		virtual bgfx::DynamicIndexBufferHandle GetDynamicIndexBufferHandle() const = 0;
		virtual bool IsDynamic() const = 0;

		virtual bool IsValidPass(uint32_t material_index) const { return true; }

		const std::vector<std::shared_ptr<Material>>&
			GetSharedMaterials() const { return shared_materials; }
		void SetSharedMaterials(const std::vector<std::shared_ptr<Material>>& mats) { shared_materials = mats; }

		virtual ~Renderer();

	protected:
		Renderer();
		virtual void Start();
		virtual void OnEnable();
		virtual void OnDisable();

	private:
		struct MaterialPass
		{
			Renderer* renderer;
			uint32_t material_index;
			uint32_t shader_id;
			uint32_t material_id;
		};

		struct Passes
		{
			std::list<std::list<MaterialPass>> list;
			std::list<Renderer*> culled_renderers;
			bool passes_dirty;
			bool culling_dirty;

			Passes() : passes_dirty(true), culling_dirty(true) { }
		};

		static void CheckPasses();
		static void CameraCulling();
		static void BuildPasses();
		static void BuildPasses(const std::list<Renderer*>& renderers,
			std::list<std::list<MaterialPass>>& passes);
		static void PreparePass(std::list<MaterialPass>& pass);
		static void CommitPass(Camera* cam, std::list<MaterialPass>& pass);

		static bool s_renderers_dirty;
		static std::list<Renderer*> s_renderers;
		static std::map<Camera*, Renderer::Passes> s_passes;

	protected:
		std::vector<std::shared_ptr<Material>> shared_materials;
	};
}

#endif // __REDERER_H__