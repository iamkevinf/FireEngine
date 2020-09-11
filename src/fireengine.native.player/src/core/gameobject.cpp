#include "gameobject.h"

#include "exportapi.h"
#include "world.h"
#include "graphics/renderer.h"
#include "graphics/layer.h"
#include "utils/string_tool.h"

namespace FireEngine
{
	std::shared_ptr<GameObject> GameObject::Create(TransformWeakPtr parent, const std::string& name, bool add_to_world)
	{
		GameObject* pObj = new GameObject(name);
		GameObjectPtr obj = GameObjectPtr(pObj);
		ObjectManager::Register(obj, ObjectType::GameObject);

		if (add_to_world)
		{
			obj->in_world = true;
			World::AddGameObject(obj);
		}

		TransformPtr transform = TransformPtr((Transform*)Component::Create(Transform::ClassName()));
		ObjectManager::Register(transform, ObjectType::Transform);
		transform->gameObject = obj;
		transform->name = name;
		obj->transform = transform;
		obj->AddComponent(transform);

		parent.lock()->children.push_back(transform);
		transform->parent = parent;

		return obj;
	}

	void GameObject::Destroy(std::shared_ptr<GameObject> obj)
	{
		if (!obj)
			return;

		obj->GetTransform()->SetParent(std::weak_ptr<Transform>());
		obj->Delete();
	}

	std::shared_ptr<GameObject> GameObject::Instantiate(const std::shared_ptr<GameObject>& source)
	{
		TransformWeakPtr parent = source->transform.lock()->GetParent();
		std::shared_ptr<GameObject> clone = GameObject::Create(parent, source->name);

		clone->DeepCopy(std::dynamic_pointer_cast<IObject>(source));

		return clone;
	}

	GameObject::GameObject(const std::string& name) :deleted(false)
		, layer((uint32_t)Layer::Default)
		, active_in_hierarchy(true)
		, active_self(true)
		, isstatic(false)
	{
		this->name = name;
	}

	void GameObject::CopyComponent(const std::shared_ptr<Component>& com)
	{
		auto transform = std::dynamic_pointer_cast<Transform>(com);

		if (transform)
		{
			GetTransform()->DeepCopy(transform);
		}
		else
		{
			auto class_name = com->GetTypeName();
			auto* p_com = Component::Create(class_name);
			if (p_com != NULL)
			{
				AddComponent(std::shared_ptr<Component>(p_com));
				p_com->DeepCopy(com);
			}
		}
	}

	void GameObject::DeepCopy(const ObjectPtr& source)
	{
		IObject::DeepCopy(source);

		auto src = std::dynamic_pointer_cast<GameObject>(source);

		for (const auto& i : src->components)
		{
			CopyComponent(i);
		}

		for (const auto& i : src->components_neo)
		{
			CopyComponent(i);
		}

		this->SetLayer(src->GetLayer());

		active_in_hierarchy = src->active_in_hierarchy;
		active_self = src->active_self;
		deleted = src->deleted;
		isstatic = src->isstatic;
	}


	void GameObject::SetLayer(uint32_t layer)
	{
		if (this->layer != layer)
		{
			this->layer = layer;

			auto renderer = GetComponent<Renderer>();
			if (renderer)
			{
				Renderer::ClearPasses();
			}
		}
	}

	void GameObject::SetActive(bool active)
	{
		if (active_self != active)
		{
			active_self = active;

			std::shared_ptr<Transform> t = transform.lock();
			if (active_in_hierarchy != active &&
				(t->IsRoot() || t->GetParent().lock()->GetGameObject()->IsActiveInHierarchy()))
			{
				SetActiveInHierarchy(active);
			}
		}
	}

	void GameObject::SetActiveInHierarchy(bool active)
	{
		if (active_in_hierarchy != active)
		{
			active_in_hierarchy = active;

			for (const auto& i : components)
			{
				if (i->IsEnable())
				{
					if (active_in_hierarchy)
					{
						i->OnEnable();
					}
					else
					{
						i->OnDisable();
					}
				}
			}

			int child_count = transform.lock()->GetChildCount();
			for (int i = 0; i < child_count; i++)
			{
				auto child = transform.lock()->GetChild(i);
				if (child->GetGameObject()->IsActiveSelf())
				{
					child->GetGameObject()->SetActiveInHierarchy(active);
				}
			}
		}
	}

	std::shared_ptr<Component> GameObject::AddComponent(const std::string& name)
	{
		if (deleted)
			return std::shared_ptr<Component>();

		std::shared_ptr<Component> t = std::shared_ptr<Component>(Component::Create(name));
		AddComponent(t);

		return t;
	}

	std::shared_ptr<Component> GameObject::GetComponent(const std::string& name) const
	{
		for (std::shared_ptr<Component> com : components)
		{
			if (!com->deleted && com->IsComponent(name))
				return com;
		}

		for (std::shared_ptr<Component> com : components_neo)
		{
			if (!com->deleted && com->IsComponent(name))
				return com;
		}

		return std::shared_ptr<Component>();
	}

	std::vector<std::shared_ptr<Component>> GameObject::GetComponentsInChildren(const std::string& name) const
	{
		std::vector<std::shared_ptr<Component>> coms;

		for (auto com : components)
		{
			if (!com->deleted && com->IsComponent(name))
				coms.push_back(com);
		}

		for (auto com : components_neo)
		{
			if (!com->deleted && com->IsComponent(name))
				coms.push_back(com);
		}

		auto transform = GetTransform();
		int child_count = transform->GetChildCount();
		for (int i = 0; i < child_count; i++)
		{
			auto child = transform->GetChild(i);
			std::vector<std::shared_ptr<Component>> child_coms = child->GetGameObject()->GetComponentsInChildren(name);

			if (!child_coms.empty())
			{
				size_t old_size = coms.size();
				coms.resize(old_size + child_coms.size());
				for (size_t i = 0; i < child_coms.size(); ++i)
					coms[old_size + i] = child_coms[i];
			}
		}

		return coms;
	}

	std::shared_ptr<Component> GameObject::GetComponentRef(const Component* com) const
	{
		for (const auto& i : components)
		{
			if (i.get() == com && !i->deleted)
			{
				return i;
			}
		}

		for (const auto& i : components_neo)
		{
			if (i.get() == com && !i->deleted)
			{
				return i;
			}
		}

		return std::shared_ptr<Component>();
	}

	void GameObject::AddComponent(const std::shared_ptr<Component>& com)
	{
		components_neo.push_back(com);

		com->transform = transform;
		com->gameObject = transform.lock()->gameObject;
		com->name = name;
		com->Awake();

		if (in_world)
			World::AddGameObject(GetTransform()->GetGameObject());
	}


	void GameObject::Delete()
	{
		if (!deleted)
			deleted = true;

		std::shared_ptr<Transform> transform = GetTransform();
		uint32_t child_count = transform->GetChildCount();
		for (uint32_t i = 0; i < child_count; ++i)
			transform->GetChild(i)->GetGameObject()->Delete();

	}

	void GameObject::OnTransformChanged()
	{
		for (const auto& i : components)
		{
			i->OnTransformChanged();
		}

		for (const auto& i : components_neo)
		{
			i->OnTransformChanged();
		}
	}

	void GameObject::Start()
	{
		std::list<std::shared_ptr<Component>> starts(components);
		do
		{
			for (auto& i : starts)
			{
				if (deleted)
					break;

				if (i->IsEnable() && !i->IsStarted())
				{
					i->started = true;
					i->Start();
				}
			}
			starts.clear();

			starts = components_neo;
			components.insert(components.end(), components_neo.begin(), components_neo.end());
			components_neo.clear();
		} while (!starts.empty());
	}

	void GameObject::Update()
	{
		for (const auto& i : components)
		{
			if (!IsActiveInHierarchy() || deleted)
				break;

			if (i->IsEnable())
				i->Update();
		}
	}

	void GameObject::LateUpdate()
	{
		for (const auto& i : components)
		{
			if (deleted)
				break;

			if (i->IsEnable())
				i->LateUpdate();
		}

		//delete component
		auto it = components.begin();
		while (it != components.end())
		{
			if ((*it)->deleted)
			{
				it = components.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	EXPORT_API GameObjectHandle GameObjectCreate(TransformHandle parent, const char16_t* name)
	{
		TransformPtr parentPtr = ObjectManager::Get(parent);
		auto go = GameObject::Create(parentPtr, ToUtf8String(std::u16string(name)));


		return { go->objectID };
	}

	EXPORT_API TransformHandle GameObjectGetTransformHandle(GameObjectHandle handle)
	{
		ObjectPtr objPtr = ObjectManager::GetRef(handle.idx);
		GameObjectPtr ret = std::static_pointer_cast<GameObject>(objPtr);

		TransformPtr transform = ret->GetTransform();

		for (auto iter = ObjectManager::objPool.begin(); iter != ObjectManager::objPool.end(); ++iter)
		{
			if (iter->second.lock() == transform)
				return { iter->first };
		}

		return { kInvalidHandle };
	}

	EXPORT_API Transform* GameObjectGetTransform(GameObject* native)
	{
		if (!native)
			return nullptr;

		return native->GetTransform().get();
	}
}
