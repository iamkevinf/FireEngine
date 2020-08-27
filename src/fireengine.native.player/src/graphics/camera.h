#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "core/component.h"
#include "cameraclearflags.h"
#include "color.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <list>
#include <bgfx/bgfx.h>

namespace FireEngine
{
	typedef std::shared_ptr<class Camera> CameraPtr;
	typedef std::weak_ptr<class Camera> CameraWeakPtr;

	class RenderPass;
	struct FrameBuffer;
	class GameObject;

	class Camera : public Component
	{
		DECLARE_COM_CLASS(Camera, Component);
	public:
		virtual ~Camera();
		static bool Init();
		static void Fini();
		static void PrepareAll();
		static void RenderAll();
		static Camera* Main() { return s_mainCamera; }
		static bool IsValidCamera(Camera* camera);
		
		bgfx::ViewId GetViewID()const { return viewId; }

		CameraClearFlags GetClearFlags() const { return clear_flags; }
		void SetClearFlags(CameraClearFlags flags) { clear_flags = flags; }

		void SetClearColor(const Color& color) { clear_color = color; }
		const Color& GetClearColor()const { return clear_color; }

		void SetOrthographic(bool value) { orthographic = value; }
		bool IsOrthographic()const { return orthographic; }

		float GetOrthographicSize() const { return orthographic_size; }
		void SetOrthographicSize(float size) { orthographic_size = size; matrix_dirty = true; }

		float GetFieldOfView() const { return field_of_view; }
		void SetFieldOfView(float fov) { field_of_view = fov;  matrix_dirty = true; }

		float GetClipNear() const { return clip_near; }
		void SetClipNear(float value) { clip_near = value; }

		float GetClipFar() const { return clip_far; }
		void SetClipFar(float value) { clip_far = value; }

		const glm::vec4& GetRect() const { return rect; }
		void SetRect(const glm::vec4& rect) { this->rect = rect; }

		uint32_t GetCullingMask() const { return culling_mask; }
		void SetCullingMask(uint32_t mask);

		bool CanRender() const;
		bool IsCulling(const std::shared_ptr<GameObject>& obj) const;

		const glm::mat4& GetViewMatrix();
		const glm::mat4& GetProjectionMatrix();
		const glm::mat4& GetViewProjectionMatrix();

		void SetFrameBuffer(const std::shared_ptr<FrameBuffer>& frame_buffer) { matrix_dirty = true; this->frame_buffer = frame_buffer; };
		uint32_t GetTargetWidth() const;
		uint32_t GetTargetHeight() const;

	protected:
		virtual void OnTransformChanged() override;

	private:
		Camera();
		void Prepare();
		void Render();

		void UpdateMatrix();

		friend size_t GGetFrameBufferTexture(Camera* camera);

	private:
		static Camera* s_mainCamera;
		static std::list<Camera*> s_cameras;

		Color clear_color = Color(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);
		CameraClearFlags clear_flags = CameraClearFlags::Color;

		bool orthographic = false;
		float orthographic_size = 10;

		float field_of_view = 90;
		float clip_near = 0.01f;
		float clip_far = 1000;;
		glm::vec4 rect = {0, 0, 1, 1};

		std::shared_ptr<FrameBuffer> frame_buffer;

		bool matrix_dirty = false;
		glm::mat4 view_matrix = glm::identity<glm::mat4>();
		glm::mat4 projection_matrix = glm::identity<glm::mat4>();
		glm::mat4 view_projection_matrix = glm::identity<glm::mat4>();

		std::shared_ptr<RenderPass> render_pass;

		uint32_t culling_mask;
		bgfx::ViewId viewId;
	};
}

#endif // __CAMERA_H__