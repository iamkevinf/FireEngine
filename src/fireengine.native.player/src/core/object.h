#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <stdint.h>
#include <map>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <memory>

#include "FireEngine.h"


#define DECLARE_COM_BASE(base)																				\
public:																										\
	typedef void* (*gen)();																					\
	static base* Create(const std::string& class_name)														\
	{																										\
		return (base*)GetClassPool()[class_name]();															\
	}																										\
	static std::string ClassName(){return #base;}															\
	static const std::vector<std::string>& ClassNames()														\
	{																										\
		if(_class_names.empty())																			\
		{																									\
			_class_names.push_back(#base);																	\
		}																									\
		return _class_names;																				\
	}																										\
	virtual std::string GetTypeName() const { return #base; }												\
		virtual const std::vector<std::string>& GetClassNames() const { return base::ClassNames(); }		\
		virtual void DeepCopy(const std::shared_ptr<IObject>& source);													\
	protected:																								\
		static void Register(const std::string& class_name, gen class_gen)									\
		{																									\
			GetClassPool()[class_name] = class_gen;															\
		}																									\
    private:																								\
        static std::map<std::string, gen>& GetClassPool()													\
		{																									\
            return _class_pool;																				\
        }																									\
        static std::map<std::string, gen> _class_pool;														\
		static std::vector<std::string> _class_names;


#define DEFINE_COM_BASE(base)																				\
    std::map<std::string, base::gen> base::_class_pool;														\
	std::vector<std::string> base::_class_names;

#define DECLARE_COM_CLASS(derived, super)																	\
    public:																									\
		static void RegisterComponent()																		\
		{																									\
			Register(#derived, derived::Create);															\
		}																									\
		static std::string ClassName() { return #derived; }													\
		static const std::vector<std::string>& ClassNames() {												\
			if(_class_names.empty()) {																		\
				_class_names = super::ClassNames();															\
				_class_names.push_back(#derived);															\
			}																								\
			return _class_names;																			\
		}																									\
        virtual std::string GetTypeName() const { return #derived; }										\
		virtual const std::vector<std::string>& GetClassNames() const { return derived::ClassNames(); }		\
		virtual void DeepCopy(const std::shared_ptr<IObject>& source);													\
    private:																								\
        static void* Create()																				\
		{																									\
            return new derived();																			\
        }																									\
		static std::vector<std::string> _class_names;

#define DECLARE_COM_CLASS_ABSTRACT(derived, super)															\
    public:																									\
		static std::string ClassName() { return #derived; }													\
		static const std::vector<std::string>& ClassNames() {												\
			if(_class_names.empty()) {																		\
				_class_names = super::ClassNames();															\
				_class_names.push_back(#derived);															\
			}																								\
			return _class_names;																			\
		}																									\
        virtual std::string GetTypeName() const { return #derived; }										\
		virtual const std::vector<std::string>& GetClassNames() const { return derived::ClassNames(); }		\
		virtual void DeepCopy(const std::shared_ptr<IObject>& source);													\
    private:																								\
		static std::vector<std::string> _class_names;

#define DEFINE_COM_CLASS(derived)																			\
    std::vector<std::string> derived::_class_names;

namespace FireEngine
{
	enum class ObjectType
	{
		Unknow,
		Scene,
		GameObject,
		Transform,
		Component,
		Asset,
		Max,
	};

	typedef std::shared_ptr<class IObject> ObjectPtr;
	typedef std::weak_ptr<class IObject> ObjectWeakPtr;
	typedef std::shared_ptr<class Transform> TransformPtr;

	class IObject
	{
	public:
		virtual void DeepCopy(const ObjectPtr& source) {}
		void SetName(const std::string& name) { this->name = name; }

		uint16_t objectID;
		ObjectType type;
		std::string name;
	};

	struct TransformHandle;

	class ObjectManager
	{
	public:
		static ObjectHandle Register(ObjectPtr obj, ObjectType type);
		static void UnRegister(ObjectHandle handle);
		static ObjectPtr GetRef(uint16_t objectId);
		static ObjectPtr Get(ObjectHandle handle);
		static TransformPtr Get(TransformHandle handle);


	public:
		static std::map<uint16_t, ObjectWeakPtr> objPool;
		static uint16_t s_cur;
	};


	template <typename T>
	std::string toUTF8(const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& source)
	{
		std::string result;

		std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
		result = convertor.to_bytes(source);

		return result;
	}

	template <typename T>
	void fromUTF8(const std::string& source, std::basic_string<T, std::char_traits<T>, std::allocator<T>>& result)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
		result = convertor.from_bytes(source);
	}

}

#endif // __OBJECT_H__