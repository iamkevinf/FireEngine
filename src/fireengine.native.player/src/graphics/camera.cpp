#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "core/transform.h"
#include "core/gameobject.h"
#include "framebuffer.h"
#include "rendertexture.h"
#include "renderpass.h"

#include <imgui.h>

#if Test
#include "loader/loader.h"
#include "imgui/imgui_impl_bgfx.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#endif

namespace FireEngine
{
	DEFINE_COM_CLASS(Camera);

	Camera* Camera::s_mainCamera;
	std::list<Camera*> Camera::s_cameras;

#if Test


	bgfx::VertexLayout g_layout;
	bgfx::ProgramHandle g_program;

	bgfx::VertexBufferHandle g_vb;
	bgfx::IndexBufferHandle g_ib;
	float g_time;

	struct myvec
	{
		float x;
		float y;
		float z;
		uint32_t m_argb;
	};

	static myvec s_cubeVertices[] =
	{
		{-1.0f,  1.0f,  1.0f, 0x01010101 },
		{ 1.0f,  1.0f,  1.0f, 0xff0101ff },
		{-1.0f, -1.0f,  1.0f, 0x0101ff01 },
		{ 1.0f, -1.0f,  1.0f, 0xff01ffff },
		{-1.0f,  1.0f, -1.0f, 0x01ff0101 },
		{ 1.0f,  1.0f, -1.0f, 0xffff01ff },
		{-1.0f, -1.0f, -1.0f, 0xffffff01 },
		{ 1.0f, -1.0f, -1.0f, 0xffffffff },
	};

	static const uint16_t s_cubeTriList[] =
	{
		0, 1, 2, // 0
		1, 3, 2,
		4, 6, 5, // 2
		5, 6, 7,
		0, 2, 4, // 4
		4, 2, 6,
		1, 5, 3, // 6
		5, 7, 3,
		0, 4, 1, // 8
		4, 5, 1,
		2, 3, 6, // 10
		6, 3, 7,
	};
#endif

	bool Camera::Init()
	{
#if Test
		//ÅäÖÃvertexbuffer ÄÚ´æ²¼¾Ö
		g_layout
			.begin()
			.add(bgfx::Attrib::Enum::Position, 3, bgfx::AttribType::Enum::Float, false)
			.add(bgfx::Attrib::Enum::Color0, 4, bgfx::AttribType::Enum::Uint8, true)
			.end();

		auto vs = loadShader("vs_buildin");
		auto fs = loadShader("fs_buildin");
		g_program = bgfx::createProgram(vs, fs, true);

		// Create static vertex buffer.
		g_vb = bgfx::createVertexBuffer(
			bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), g_layout
		);
		g_ib = bgfx::createIndexBuffer(
			bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));
#endif
		return true;
	}

	void Camera::Fini()
	{
		s_cameras.clear();
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

	Camera::Camera() :
		matrix_dirty(true)
	{
		s_cameras.push_back(this);
	}

	Camera::~Camera()
	{
#if Test
		if (bgfx::isValid(g_vb))
		{
			bgfx::destroy(g_vb);
			g_vb = BGFX_INVALID_HANDLE;
		}
		if (bgfx::isValid(g_ib))
		{
			bgfx::destroy(g_ib);
			g_ib = BGFX_INVALID_HANDLE;
		}
		if (bgfx::isValid(g_program))
		{
			bgfx::destroy(g_program);
			g_program = BGFX_INVALID_HANDLE;
		}
#endif
	}

	void Camera::DeepCopy(const ObjectPtr& source)
	{
		IM_ASSERT(0 && "Camera can not DeepCopy!");
	}

	bool Camera::CanRender() const
	{
		return GetGameObject()->IsActiveInHierarchy() && IsEnable();
	}

	void Camera::OnTransformChanged()
	{
		matrix_dirty = true;
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

		render_pass->Bind();

		//Renderer::PrepareAllPass();

		render_pass->UnBind();
	}

	void Camera::Render()
	{
		render_pass->Begin(GetClearColor());

		//Renderer::RenderAllPass();

#if Test

		glm::vec3 eye = GetTransform()->GetWorldPosition(); //glm::vec3(0.0f, 10.0f, -35.0f);

		glm::mat4 view = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), GetTransform()->GetRight());
		glm::mat4 proj = GetProjectionMatrix(); // glm::perspective(glm::radians(60.0f), float(1024) / 1024, 0.1f, 100.0f);

		glm::quat rot = GetTransform()->GetWorldRotation();

		//Set view and projection matrix for view 10.
		{
			g_time += 0.001f;

			bgfx::ViewId viewId = render_pass->GetViewId();
			bgfx::setViewTransform(viewId, &view, &proj);

			for (int i = 0; i < 11; ++i)
			{
				for (int j = 0; j < 11; ++j)
				{
					glm::mat4 mtx = glm::identity<glm::mat4>();
					mtx = glm::translate(mtx, glm::vec3(15.0f - float(j) * 3.0f, 0.0f, -15.0f + float(i) * 3.0f));
					mtx *= glm::yawPitchRoll(g_time + j * 0.21f, g_time + i * 0.37f, 0.0f);

					// Set model matrix for rendering.
					bgfx::setTransform(&mtx);

					// Set vertex and index buffer.
					bgfx::setVertexBuffer(0, g_vb);
					bgfx::setIndexBuffer(g_ib);

					uint64_t state = BGFX_STATE_DEFAULT;

					// Set render states.
					bgfx::setState(state);

					// Submit primitive for rendering to view 0.
					bgfx::submit(viewId, g_program);
				}
			}

		}
#endif

		render_pass->End();
	}


	bgfx::TextureHandle Camera::GetTexture()
	{
		union { struct { bgfx::TextureHandle handle; uint8_t flags; uint8_t mip; } s; ImTextureID id; } tex;
		Camera* camera = Camera::Main();
		if (!camera)
			return { bgfx::kInvalidHandle };

		auto render_pass = camera->render_pass;
		tex.s.handle = render_pass->GetFrameBuffer().color_texture->GetTextureHandle();
		tex.s.flags = IMGUI_FLAGS_ALPHA_BLEND;
		tex.s.mip = 0;
		return tex.s.handle;
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
			projection_matrix = glm::perspective(GetFieldOfView(), width / (float)height, GetClipNear(), GetClipFar());
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

	EXPORT_API CameraHandle CameraCreate(TransformHandle parent, const char16_t* name)
	{
		std::string _name = toUTF8(std::u16string(name));
		TransformPtr parentPtr = ObjectManager::Get(parent);
		CameraPtr camera = GameObject::Create(parentPtr, _name)->AddComponent<Camera>();

		ObjectManager::Register(camera, ObjectType::Component);

		return { camera->objectID };
	}

	EXPORT_API TransformHandle CameraGetTransformHandle(CameraHandle handle)
	{
		ObjectPtr objPtr = ObjectManager::GetRef(handle.idx);
		CameraPtr ret = std::static_pointer_cast<Camera>(objPtr);

		TransformPtr transform = ret->GetTransform();

		for (auto iter = ObjectManager::objPool.begin(); iter != ObjectManager::objPool.end(); ++iter)
		{
			if (iter->second.lock() == transform)
				return { iter->first };
		}

		return { kInvalidHandle };
	}

}