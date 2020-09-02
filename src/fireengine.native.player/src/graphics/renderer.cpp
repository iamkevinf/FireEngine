#include "renderer.h"
#include "material.h"

#include <debugdraw/debugdraw.h>

#include "camera.h"
#include "core/gameobject.h"

namespace FireEngine
{
	DEFINE_COM_CLASS(Renderer);

	bool Renderer::s_renderers_dirty = false;
	std::list<Renderer*> Renderer::s_renderers;
	std::map<Camera*, Renderer::Passes> Renderer::s_passes;

	bool Renderer::IsRenderersDirty()
	{
		bool dirty;
		dirty = s_renderers_dirty;
		return dirty;
	}

	void Renderer::SetRenderersDirty(bool dirty)
	{
		s_renderers_dirty = dirty;
	}


	void Renderer::SetCullingDirty(Camera* cam)
	{
		auto iter = s_passes.find(cam);
		if (s_passes.count(cam) > 0)
		{
			s_passes[cam].culling_dirty = true;
		}
	}

	std::list<Renderer*>& Renderer::GetRenderers()
	{
		return s_renderers;
	}

	void Renderer::ClearPasses()
	{
		s_passes.clear();
	}

	void Renderer::CheckPasses()
	{
		std::vector<Camera*> invalid_cams;
		for (auto& i : s_passes)
		{
			if (!Camera::IsValidCamera(i.first))
			{
				invalid_cams.push_back(i.first);
			}
		}
		for (auto i : invalid_cams)
		{
			s_passes.erase(i);
		}

		auto cam = Camera::Main();
		if (!(s_passes.count(cam) > 0))
		{
			s_passes[cam] = Passes();
		}
	}

	void Renderer::CameraCulling()
	{
		auto cam = Camera::Main();

		if (s_passes[cam].culling_dirty)
		{
			s_passes[cam].culling_dirty = false;

			std::list<Renderer*> renderers;
			auto& culled_renderers = s_passes[cam].culled_renderers;
			bool diff = false;

			// TODO: m_renderers in octree

			for (auto i : s_renderers)
			{
				if (!i->GetGameObject()->IsActiveInHierarchy() ||
					!i->IsEnable() ||
					cam->IsCulling(i->GetGameObject()))
				{
					continue;
				}

				if (cam->IsOrthographic())
				{
					renderers.push_back(i);
				}
				else
				{
					renderers.push_back(i);
				}
			}

			if (renderers.size() != culled_renderers.size())
			{
				diff = true;
			}
			else
			{
				auto ia = renderers.begin();
				auto ib = culled_renderers.begin();

				while (ia != renderers.end() && ib != culled_renderers.end())
				{
					auto a = *ia;
					auto b = *ib;
					if (a != b)
					{
						diff = true;
						break;
					}

					ia++;
					ib++;
				}
			}

			if (diff)
			{
				culled_renderers = renderers;
				s_passes[cam].passes_dirty = true;
			}
		}
	}

	void Renderer::BuildPasses()
	{
		auto cam = Camera::Main();

		if (s_passes[cam].passes_dirty)
		{
			s_passes[cam].passes_dirty = false;

			BuildPasses(s_passes[cam].culled_renderers, s_passes[cam].list);
		}
	}

	void Renderer::BuildPasses(const std::list<Renderer*>& renderers, std::list<std::list<MaterialPass>>& passes)
	{
		std::list<Renderer::MaterialPass> mat_passes;

		for (auto i : renderers)
		{
			auto& mats = i->GetSharedMaterials();

			for (uint32_t j = 0; j < mats.size(); j++)
			{
				auto& mat = mats[j];

				if (!mat || !i->IsValidPass(j))
					continue;

				auto shader = mat->GetShader();

				MaterialPass pass;
				pass.renderer = i;
				pass.material_index = j;
				pass.shader_id = shader->objectID;
				pass.material_id = mat->objectID;

				mat_passes.push_back(pass);
			}
		}

		passes.clear();

		std::list<MaterialPass> pass;
		for (auto& i : mat_passes)
		{
			if (pass.empty())
			{
				pass.push_back(i);
			}
			else
			{
				const auto& last = pass.back();
				if (i.shader_id == last.shader_id)
				{
					pass.push_back(i);
				}
				else
				{
					passes.push_back(pass);

					pass.clear();
					pass.push_back(i);
				}
			}
		}

		if (!pass.empty())
		{
			passes.push_back(pass);
		}
	}

	void Renderer::PrepareAllPass()
	{
		CheckPasses();
		CameraCulling();
		BuildPasses();

		auto& passes = s_passes[Camera::Main()].list;
		for (auto& i : passes)
		{
			Renderer::PreparePass(i);
		}
	}

	void Renderer::PreparePass(std::list<MaterialPass>& pass)
	{
		auto& first = pass.front();
		auto& shader = first.renderer->GetSharedMaterials()[first.material_index]->GetShader();
	}

	void Renderer::RenderAllPass()
	{

		auto cam = Camera::Main();
		auto& passes = s_passes[cam].list;

		for (auto& i : passes)
		{
			Renderer::CommitPass(cam, i);
		}

	}

	void Renderer::CommitPass(Camera* cam, std::list<MaterialPass>& pass)
	{
		if (!cam)
			return;

		bgfx::ViewId viewId = cam->GetViewID();
		glm::vec3 eye = cam->GetTransform()->GetWorldPosition();
		glm::mat4 view = cam->GetViewMatrix();
		glm::mat4 proj = cam->GetProjectionMatrix();
		bgfx::setViewTransform(viewId, &view, &proj);

		for (auto& ele : pass)
		{
			auto& mat = ele.renderer->GetSharedMaterials()[ele.material_index];

			glm::mat4 worldMatrix = ele.renderer->GetTransform()->GetLocal2WorldMatrix();

			// Set model matrix for rendering.
			bgfx::setTransform(&worldMatrix[0][0]);

			// Set vertex and index buffer.
			if (ele.renderer->IsDynamic())
			{
				bgfx::setVertexBuffer(0, ele.renderer->GetDynamicVertexBufferHandle());
				bgfx::setIndexBuffer(ele.renderer->GetDynamicIndexBufferHandle(ele.material_index));
			}
			else
			{
				bgfx::setVertexBuffer(0, ele.renderer->GetVertexBufferHandle());
				bgfx::setIndexBuffer(ele.renderer->GetIndexBufferHandle(ele.material_index));
			}

			uint64_t state = BGFX_STATE_DEFAULT;

			// Set render states.
			bgfx::setState(state);

			// Submit primitive for rendering to view 0.
			bgfx::submit(viewId, mat->GetShader()->program);
		}

		DebugDrawEncoder dde;
		dde.begin(viewId);
		dde.drawGrid(Axis::Y, { 0,0,0 }, 128, 1.0f);
		dde.end();

		bgfx::setDebug(cam->GetDebug());
	}

	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
		SetRenderersDirty(true);
	}

	void Renderer::Start()
	{
		SetRenderersDirty(true);
	}

	void Renderer::OnEnable()
	{
		SetRenderersDirty(true);
	}

	void Renderer::OnDisable()
	{
		SetRenderersDirty(true);
	}

	void Renderer::DeepCopy(const ObjectPtr& source)
	{
		Component::DeepCopy(source);

		auto src = std::dynamic_pointer_cast<Renderer>(source);
		SetSharedMaterials(src->GetSharedMaterials());
	}
}