#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "object.h"
#include "transform.h"
#include <list>
#include "exportapi.h"

namespace FireEngine
{
	class Component;

	FIREENGINE_HANDLE(GameObjectHandle);

	typedef std::shared_ptr<GameObject> GameObjectPtr;

	class GameObject : public IObject
	{
		friend class Transform;

	public:
		static std::shared_ptr<GameObject> Create(TransformWeakPtr parent, const std::string& name, bool add_to_world = true);
		static void Destroy(std::shared_ptr<GameObject> obj);
		static std::shared_ptr<GameObject> Instantiate(const std::shared_ptr<GameObject>& source);

		std::shared_ptr<Component> AddComponent(const std::string& name);
		std::shared_ptr<Component> GetComponent(const std::string& name) const;
		std::vector<std::shared_ptr<Component>> GetComponentsInChildren(const std::string& name) const;

		void DeepCopy(const ObjectPtr& source) override;

		template<class T> std::shared_ptr<T> AddComponent();
		template<class T> std::shared_ptr<T> GetComponent() const;
		template<class T> bool GetComponents(std::vector<std::shared_ptr<T>>& coms) const;
		template<class T> std::vector<std::shared_ptr<T>> GetComponentsInChildren() const;
		template<class T> std::shared_ptr<T> GetComponentInParent() const;
		std::shared_ptr<Component> GetComponentRef(const Component* com) const;

		std::shared_ptr<Transform> GetTransform() const { return transform.lock(); }


		bool IsActiveInHierarchy() const { return active_in_hierarchy; }
		bool IsActiveSelf() const { return active_self; }

		void SetActive(bool active);

	private:
		GameObject(const std::string& name);

		void AddComponent(const std::shared_ptr<Component>& com);

		void CopyComponent(const std::shared_ptr<Component>& com);

		void Delete();
		void Start();
		void Update();
		void LateUpdate();
		void SetActiveInHierarchy(bool active);

	private:
		bool deleted;
		bool inWorld;
		std::list<std::shared_ptr<Component>> components;
		std::list<std::shared_ptr<Component>> components_neo;

		std::weak_ptr<Transform> transform;

		bool active_in_hierarchy;
		bool active_self;

	};


	template<class T> std::shared_ptr<T> GameObject::AddComponent()
	{
		return std::dynamic_pointer_cast<T>(AddComponent(T::ClassName()));
	}

	template<class T> std::shared_ptr<T> GameObject::GetComponent() const
	{
		for (auto i : components)
		{
			auto t = std::dynamic_pointer_cast<T>(i);
			if (t && !i->deleted)
			{
				return t;
			}
		}

		for (auto i : components_neo)
		{
			auto t = std::dynamic_pointer_cast<T>(i);
			if (t && !i->deleted)
			{
				return t;
			}
		}

		return std::shared_ptr<T>();
	}

	template<class T> bool GameObject::GetComponents(std::vector<std::shared_ptr<T>>& coms) const
	{
		for (auto i : components)
		{
			auto t = std::dynamic_pointer_cast<T>(i);
			if (t && !i->deleted)
			{
				coms.push_back(t);
			}
		}

		for (auto i : components_neo)
		{
			auto t = std::dynamic_pointer_cast<T>(i);
			if (t && !i->deleted)
			{
				coms.push_back(t);
			}
		}

		return coms.size() > 0;
	}

	template<class T> std::vector<std::shared_ptr<T>> GameObject::GetComponentsInChildren() const
	{
		std::vector<std::shared_ptr<T>> coms;

		for (auto i : components)
		{
			auto t = std::dynamic_pointer_cast<T>(i);
			if (t && !i->deleted)
			{
				coms.push_back(t);
			}
		}

		for (auto i : components_neo)
		{
			auto t = std::dynamic_pointer_cast<T>(i);
			if (t && !i->deleted)
			{
				coms.push_back(t);
			}
		}

		auto transform = GetTransform();
		int child_count = transform->GetChildCount();
		for (int i = 0; i < child_count; i++)
		{
			auto child = transform->GetChild(i);
			auto child_coms = child->GetGameObject()->GetComponentsInChildren<T>();

			if (!child_coms.empty())
			{
				auto old_size = coms.size();
				coms.resize(old_size + child_coms.size());
				for (auto i = 0; i < child_coms.size(); ++i)
					coms[old_size + i] = child_coms[i];
			}
		}

		return coms;
	}

	template<class T> std::shared_ptr<T> GameObject::GetComponentInParent() const
	{
		std::shared_ptr<T> com;

		auto parent = GetTransform()->GetParent().lock();

		while (parent)
		{
			com = parent->GetGameObject()->GetComponent<T>();

			if (com)
			{
				break;
			}
			else
			{
				parent = parent->GetParent().lock();
			}
		}

		return com;
	}
}

#endif // __GAMEOBJECT_H__
