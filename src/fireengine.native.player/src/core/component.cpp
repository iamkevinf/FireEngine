#include "component.h"
#include "gameobject.h"
#include "transform.h"
#include "graphics/camera.h"

namespace FireEngine
{
	DEFINE_COM_BASE(Component);

	/*static*/void Component::RegisterComponents()
	{
		Transform::RegisterComponent();
		Camera::RegisterComponent();
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
}
