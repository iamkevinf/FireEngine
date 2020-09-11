#include "component.h"
#include "gameobject.h"
#include "transform.h"

#include "graphics/camera.h"
#include "graphics/meshrenderer.h"

#include <imgui.h>

namespace FireEngine
{
	DEFINE_COM_BASE(Component);

	/*static*/void Component::RegisterComponents()
	{
		Transform::RegisterComponent();
		Camera::RegisterComponent();

		MeshRenderer::RegisterComponent();
	}

	/*static*/void Component::Destroy(std::shared_ptr<Component> com)
	{
		if (com)
			com->Delete();
	}

	Component::~Component()
	{

	}

	void Component::Enable(bool enable)
	{

	}

	void Component::Delete()
	{
		if (!deleted)
		{
			deleted = true;
			Enable(false);
		}
	}

	void Component::DeepCopy(const std::shared_ptr<IObject>& source)
	{
		IObject::DeepCopy(source);

		auto src = std::dynamic_pointer_cast<Component>(source);
		deleted = src->deleted;
		enable = src->enable;
		started = false;
	}

	std::shared_ptr<GameObject> Component::GetGameObject() const
	{
		return gameObject.lock();
	}

	std::shared_ptr<Transform> Component::GetTransform() const
	{
		return transform.lock();
	}

	std::shared_ptr<Component> Component::GetRef() const
	{
		return GetGameObject()->GetComponentRef(this);
	}

	bool Component::IsComponent(const std::string& type) const
	{
		auto& names = GetClassNames();

		for (auto& name : names)
		{
			if (name == type)
				return true;
		}

		return false;
	}

	void GComponentCallScriptable(Component* native, ScriptableFuncType type)
	{
		if (native)
		{
			switch (type)
			{
			case ScriptableFuncType::Awake:
				if(native->delegateAwake)
					native->delegateAwake();
				break;
			case ScriptableFuncType::Start:
				if(native->delegateStart)
					native->delegateStart();
				break;
			case ScriptableFuncType::Update:
				if(native->delegateUpdate)
					native->delegateUpdate();
				break;
			case ScriptableFuncType::LateUpdate:
				if(native->delegateLateUpdate)
					native->delegateLateUpdate();
				break;
			case ScriptableFuncType::OnEnable:
				if(native->delegateOnEnable)
					native->delegateOnEnable();
				break;
			case ScriptableFuncType::OnDisable:
				if(native->delegateOnDisable)
					native->delegateOnDisable();
				break;
			default:
				break;
			}
		}
	}

	void GComponentRegisterCallScriptable(Component* native,
		ScriptableFuncType type, ScriptableDelegate delegate)
	{
		if (native)
		{
			switch (type)
			{
			case ScriptableFuncType::Awake:
				native->delegateAwake = delegate;
				break;
			case ScriptableFuncType::Start:
				native->delegateStart = delegate;
				break;
			case ScriptableFuncType::Update:
				native->delegateUpdate = delegate;
				break;
			case ScriptableFuncType::LateUpdate:
				native->delegateLateUpdate = delegate;
				break;
			case ScriptableFuncType::OnEnable:
				native->delegateOnEnable = delegate;
				break;
			case ScriptableFuncType::OnDisable:
				native->delegateOnDisable = delegate;
				break;
			default:
				break;
			}
		}
	}

	EXPORT_API void ComponentRegisterCallScriptable(Component* native,
		ScriptableFuncType type, ScriptableDelegate delegate)
	{
		GComponentRegisterCallScriptable(native, type, delegate);
	}

	EXPORT_API GameObject* ComponentGetGameObject(Component* native)
	{
		if (!native)
			return nullptr;

		return native->GetGameObject().get();
	}

	EXPORT_API ComponentType ComponentGetComponentType(Component* native)
	{
		if (!native)
			return ComponentType::None;
		return native->GetComponentType();
	}
}
