#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "core/component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <list>

namespace FireEngine
{
	typedef std::shared_ptr<class Camera> CameraPtr;
	typedef std::weak_ptr<class Camera> CameraWeakPtr;

	FIREENGINE_HANDLE(CameraHandle);

	class Camera : public Component
	{
		DECLARE_COM_CLASS(Camera, Component);
	public:
		virtual ~Camera();
		static bool Init();
		static void Fini();
		static void RenderAll();
		static Camera* Main() { return s_mainCamera; }

		void SetOrthographic(bool value) { orthographic = value; }
		bool IsOrthographic()const { return orthographic; }

		float GetOrthographicSize() const { return orthographic_size; }
		void SetOrthographicSize(float size) { orthographic_size = size; }

		float GetFieldOfView() const { return field_of_view; }
		void SetFieldOfView(float fov) { field_of_view = fov; }

		float GetClipNear() const { return clip_near; }
		void SetClipNear(float value) { clip_near = value; }

		float GetClipFar() const { return clip_far; }
		void SetClipFar(float value) { clip_far = value; }

		const glm::vec4& GetRect() const { return rect; }
		void SetRect(const glm::vec4& rect) { this->rect = rect; }

		bool CanRender() const;

		const glm::mat4& GetViewMatrix();
		const glm::mat4& GetProjectionMatrix();
		const glm::mat4& GetViewProjectionMatrix();

	private:
		Camera();

		void Render();

		void UpdateMatrix();

	private:
		static Camera* s_mainCamera;
		static std::list<Camera*> s_cameras;

		bool orthographic = false;
		float orthographic_size = 10;

		float field_of_view = 90;
		float clip_near = 0.1f;
		float clip_far = 100;;
		glm::vec4 rect = {0, 0, 1, 1};

		bool matrix_dirty = false;
		glm::mat4 view_matrix = glm::identity<glm::mat4>();
		glm::mat4 projection_matrix = glm::identity<glm::mat4>();
		glm::mat4 view_projection_matrix = glm::identity<glm::mat4>();
	};
}

#endif // __CAMERA_H__