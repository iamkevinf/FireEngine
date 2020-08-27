#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "core/transform.h"
#include "core/gameobject.h"
#include "framebuffer.h"
#include "rendertexture.h"
#include "renderpass.h"
#include "renderer.h"
#include "meshrenderer.h"
#include "material.h"
#include "io/memorystream.h"

#include <imgui.h>
#include "imgui/imgui_impl_bgfx.h"

namespace FireEngine
{
	DEFINE_COM_CLASS(Camera);

	Camera* Camera::s_mainCamera;
	std::list<Camera*> Camera::s_cameras;

	bool Camera::Init()
	{
		return true;
	}

	void Camera::Fini()
	{
		s_cameras.clear();
	}

	bool Camera::IsValidCamera(Camera* camera)
	{
		for (Camera* v : s_cameras)
		{
			if (v == camera)
				return true;
		}

		return false;
	}

	void Camera::PrepareAll()
	{
		for (Camera* camera : s_cameras)
		{
			if (camera->CanRender())
			{
				s_mainCamera = camera;
				camera->Prepare();
			}
		}

		//s_mainCamera = nullptr;
	}

	void Camera::RenderAll()
	{
		for (Camera* camera : s_cameras)
		{
			if (camera->CanRender())
			{
				s_mainCamera = camera;
				camera->Render();
			}
		}

		//s_mainCamera = nullptr;
	}

	Camera::Camera() : matrix_dirty(true)
		, culling_mask(-1)
	{
		s_cameras.push_back(this);
	}

	Camera::~Camera()
	{
	}

	void Camera::SetCullingMask(uint32_t mask)
	{
		if (culling_mask != mask)
		{
			culling_mask = mask;

			Renderer::SetCullingDirty(this);
		}
	}

	void Camera::DeepCopy(const ObjectPtr& source)
	{
		IM_ASSERT(0 && "Camera can not DeepCopy!");
	}

	bool Camera::CanRender() const
	{
		return GetGameObject()->IsActiveInHierarchy() && IsEnable();
	}

	bool Camera::IsCulling(const GameObjectPtr& obj) const
	{
		return (GetCullingMask() & (1 << obj->GetLayer())) == 0;
	}

	void Camera::OnTransformChanged()
	{
		matrix_dirty = true;

		Renderer::SetCullingDirty(this);
	}


	void Camera::Prepare()
	{
		if (!render_pass)
		{
			uint32_t width = 1024;
			uint32_t height = 1024;
			auto color = RenderTexture::Create(width, height, RenderTextureFormat::RGBA8, DepthBuffer::Depth_16, FilterMode::Bilinear);
			auto depth = RenderTexture::Create(width, height, RenderTextureFormat::Depth, DepthBuffer::Depth_16, FilterMode::Bilinear);
			render_pass = RenderPass::Create(
				std::shared_ptr<RenderTexture>(color),
				std::shared_ptr<RenderTexture>(depth), GetClearFlags(), true,
				glm::vec4(0, 0, width, height));
		}

		Renderer::PrepareAllPass();
	}

	void Camera::Render()
	{
		viewId = render_pass->Begin(GetClearColor());
		Renderer::RenderAllPass();
		render_pass->End();
	}

	void Camera::UpdateMatrix()
	{
		matrix_dirty = false;

		uint32_t width = 1024;
		uint32_t height = 1024;

		TransformPtr transform = GetTransform();

		view_matrix = glm::lookAt(transform->GetWorldPosition(), transform->GetWorldPosition() + transform->GetForward(), transform->GetUp());

		if (!IsOrthographic())
		{
			projection_matrix = glm::perspective(GetFieldOfView() * Deg2Rad, width / (float)height, GetClipNear(), GetClipFar());
		}
		else
		{
			float ortho_size = GetOrthographicSize();
			auto rect = GetRect();

			float top = ortho_size;
			float bottom = -ortho_size;
			float plane_h = ortho_size * 2;
			float plane_w = plane_h * (width * rect.z) / (height * rect.w);
			projection_matrix = glm::ortho(-plane_w / 2, plane_w / 2, bottom, top, GetClipNear(), GetClipFar());
		}

		view_projection_matrix = projection_matrix * view_matrix;
	}

	const glm::mat4& Camera::GetViewMatrix()
	{
		if (matrix_dirty)
			UpdateMatrix();

		return view_matrix;
	}

	const glm::mat4& Camera::GetProjectionMatrix()
	{
		if (matrix_dirty)
			UpdateMatrix();

		return projection_matrix;
	}

	const glm::mat4& Camera::GetViewProjectionMatrix()
	{
		if (matrix_dirty)
			UpdateMatrix();

		return view_projection_matrix;
	}


	uint32_t Camera::GetTargetWidth() const
	{
		return frame_buffer ? frame_buffer->color_texture->GetWidth() :
			1024;// Graphics::GetDisplay()->GetWidth();
	}

	uint32_t Camera::GetTargetHeight() const
	{
		return frame_buffer ? frame_buffer->color_texture->GetHeight() :
			1024;// Graphics::GetDisplay()->GetHeight();
	}

	EXPORT_API Camera* CameraCreate(TransformHandle parent, const char16_t* name)
	{
		std::string _name = toUTF8(std::u16string(name));
		TransformPtr parentPtr = ObjectManager::Get(parent);
		CameraPtr camera = GameObject::Create(parentPtr, _name)->AddComponent<Camera>();
		camera->SetCullingMask(1 << 0);

		ObjectManager::Register(camera, ObjectType::Component);

		return camera.get();
	}

	EXPORT_API TransformHandle CameraGetTransformHandle(Camera* camera)
	{
		ObjectPtr objPtr = ObjectManager::GetRef(camera->objectID);
		CameraPtr ret = std::static_pointer_cast<Camera>(objPtr);

		TransformPtr transform = ret->GetTransform();

		for (auto iter = ObjectManager::objPool.begin(); iter != ObjectManager::objPool.end(); ++iter)
		{
			if (iter->second.lock() == transform)
				return { iter->first };
		}

		return { kInvalidHandle };
	}

	bgfx::TextureHandle GGetFrameBufferTexture(Camera* camera)
	{
		if (!camera)
			return { bgfx::kInvalidHandle };

		union { struct { bgfx::TextureHandle handle; uint8_t flags; uint8_t mip; } s; ImTextureID id; } tex;
		auto render_pass = camera->render_pass;
		tex.s.handle = render_pass->GetFrameBuffer().color_texture->GetTextureHandle();
		tex.s.flags = IMGUI_FLAGS_ALPHA_BLEND;
		tex.s.mip = 0;
		return tex.s.handle;
	}

	EXPORT_API Camera* CameraGetMainCamera()
	{
		return Camera::Main();
	}

	EXPORT_API bgfx::TextureHandle CameraGetFrameBufferTexture(Camera* camera)
	{
		return GGetFrameBufferTexture(camera);
	}

	EXPORT_API uint32_t CameraGetClearColor(Camera* camera)
	{
		Color color = camera->GetClearColor();
		return color.GetHex();
	}

	EXPORT_API void CameraSetClearColor(Camera* camera, uint32_t colorHex)
	{
		Color color(colorHex);
		camera->SetClearColor(color);
	}

}