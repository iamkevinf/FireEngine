#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "object.h"
#include "exportapi.h"

namespace FireEngine
{
	class GameObject;
	class Transform;

	enum class ScriptableFuncType
	{
		Awake,
		Start,
		Update,
		LateUpdate,
		OnEnable,
		OnDisable
	};

	enum class ComponentType
	{
		None = -1,
		Transform,
		Camera,
		Renderer,
		MeshRenderer,
	};

	typedef void (*ScriptableDelegate)();

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

		void SetComponentType(ComponentType _type) { type = _type; }
		ComponentType GetComponentType()const { return type; }

	protected:
		virtual void Awake() { GComponentCallScriptable(this, ScriptableFuncType::Awake); }
		virtual void Start() { GComponentCallScriptable(this, ScriptableFuncType::Start); }
		virtual void Update() { GComponentCallScriptable(this, ScriptableFuncType::Update); }
		virtual void LateUpdate() { GComponentCallScriptable(this, ScriptableFuncType::LateUpdate); }
		virtual void OnEnable() { GComponentCallScriptable(this, ScriptableFuncType::OnEnable); }
		virtual void OnDisable() { GComponentCallScriptable(this, ScriptableFuncType::OnDisable); }
		virtual void OnTransformChanged() { }

		friend void GComponentCallScriptable(Component* native, ScriptableFuncType type);
		friend void GComponentRegisterCallScriptable(Component* native,
			ScriptableFuncType type, ScriptableDelegate delegate);

	private:
		void Delete();

		ScriptableDelegate delegateAwake = nullptr;
		ScriptableDelegate delegateStart = nullptr;
		ScriptableDelegate delegateUpdate = nullptr;
		ScriptableDelegate delegateLateUpdate = nullptr;
		ScriptableDelegate delegateOnEnable = nullptr;
		ScriptableDelegate delegateOnDisable = nullptr;

	protected:
		std::weak_ptr<GameObject> gameObject;
		std::weak_ptr<Transform> transform;
		ComponentType type;

	private:
		bool deleted = false;
		bool started = false;
		bool enable = true;
	};

}

#endif // __COMPONENT_H__
