#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "object.h"

namespace FireEngine
{
	class GameObject;
	class Transform;

	class Component : public IObject
	{
		DECLARE_COM_BASE(Component);
		friend GameObject;

	public:
		virtual ~Component();

		Component() = default;
		Component(const Component& ref) = delete;
		Component& operator= (const Component& scene) = delete;
		Component& operator= (Component&& scene) = delete;

		static void RegisterComponents();
		static void Destroy(std::shared_ptr<Component> com);

		std::shared_ptr<GameObject> GetGameObject() const;
		std::shared_ptr<Transform> GetTransform() const;
		std::shared_ptr<Component> GetRef() const;

		void Enable(bool enable);
		bool IsEnable() const { return enable; }
		bool IsStarted() const { return started; }
		bool IsComponent(const std::string& type) const;

	protected:
		virtual void Awake() { }
		virtual void Start() { }
		virtual void Update() { }
		virtual void LateUpdate() { }
		virtual void OnEnable() { }
		virtual void OnDisable() { }
		virtual void OnTransformChanged() { }

	private:
		void Delete();

	protected:
		std::weak_ptr<GameObject> gameObject;
		std::weak_ptr<Transform> transform;

	private:
		bool deleted = false;
		bool started = false;
		bool enable = true;
	};
}

#endif // __COMPONENT_H__
