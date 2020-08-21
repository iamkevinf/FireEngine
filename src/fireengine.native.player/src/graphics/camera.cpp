#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "core/transform.h"
#include "core/gameobject.h"

#include <imgui.h>

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

	}

	void Camera::DeepCopy(const ObjectPtr& source)
	{
		IM_ASSERT(0 && "Camera can not DeepCopy!");
	}

	bool Camera::CanRender() const
	{
		return GetGameObject()->IsActiveInHierarchy() && IsEnable();
	}

	void Camera::Render()
	{
		// TODO
	}

	void Camera::UpdateMatrix()
	{
		matrix_dirty = false;

		uint32_t width = 1024 * 2;
		uint32_t height = 1024 * 2;

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