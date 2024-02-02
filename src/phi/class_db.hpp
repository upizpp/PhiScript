#pragma once

#include <functional>
#include <phi/exception.hpp>
#include <phi/traits.hpp>
#include <phi/typedef.hpp>
#include <phi/variant.hpp>

#define METHOD_R(name, rename) ClassDB::registerMethod(#rename, &Test::name);
#define METHOD(name) METHOD_R(name, name)
#define PROPERTY_OFFSET(class, name) size_t(&((class *)0)->name)
#define CLASS
#define PROPERTY_R(name, rename) ClassDB::registerProperty<decltype(((CLASS *)0)->name)>(#rename, PROPERTY_OFFSET(CLASS, name))
#define PROPERTY(name) PROPERTY_R(name, name)
#define PhiClass(class_name, parent_class) \
public:                                    \
    static void static_register();         \
    static string className()              \
    {                                      \
        return #class_name;                \
    }                                      \
    static string parentClassName()        \
    {                                      \
        return #parent_class;              \
    }                                      \
    virtual string getClass()              \
    {                                      \
        return #class_name;                \
    }                                      \
    virtual string getParentClass()        \
    {                                      \
        return #parent_class;              \
    }                                      \
                                           \
private:                                   \
    ClassRegister<class_name, parent_class> _;

namespace phi
{
	struct ClassInfo
	{
		using type = Variant;
		using arg_list = vector<type>;
		using method = std::function<type(Object*, arg_list&)>;

		ClassInfo* parent;
		map<string, method> methods;
		map<string, std::pair<std::function<type(Object*)>, std::function<void(Object*, const type&)>>> properties;

		ClassInfo() {}
		ClassInfo(ClassInfo* p): parent(p) {}

		Variant call(Object*, const string&, arg_list&);

		method& getMethod(const string&);
	};

	class ClassDB
	{
	public:
		using type = Variant;
		using arg_list = vector<type>;

	private:
		static map<string, ClassInfo> _M_classes;
		static string _M_bound;
		static string _M_calling;

	public:
		static void bind(const string& class_name) { _M_bound = class_name; }
		static void registerClass(const string& class_name, const string& parent)
		{
			ClassInfo* parent_info = _M_classes.find(parent) != _M_classes.end() ? &_M_classes[parent] : nullptr;
			_M_classes.insert({ class_name, ClassInfo{parent_info} });
			bind(class_name);
		}

		template <typename F>
		static void registerMethod(const string& name, const F& func)
		{
			auto caller = [=](Object* obj, arg_list& args) -> type
				{
					using seq = gen_index_seq_t<function_traits<F>::arity>;
					return call_impl<seq, F>::call(func, obj, args);
				};
			getInfo().methods.insert({ name, caller });
		}
		template <typename T>
		static void registerProperty(const string& name, size_t offset)
		{
			auto setter = [=](Object* obj, const type& value) -> void
				{
					char* addr = (char*)obj + offset;
					*reinterpret_cast<T*>(addr) = static_cast<T>(value);
				};
			auto getter = [=](Object* obj) -> type
				{
					char* addr = (char*)obj + offset;
					return type{ *reinterpret_cast<T*>(addr) };
				};
			getInfo().properties.insert({ name, {getter, setter} });
		}

		friend struct call_impl;

		template <typename I, typename F>
		struct call_impl;

		template <int... I, typename F>
		struct call_impl<index_seq<I...>, F>
		{
			static type call(F f, Object* obj, arg_list& args)
			{
				try
				{
					using class_type = typename function_traits<F>::class_type;
					return (static_cast<class_type*>(obj)->*f)(
						args.at(I).seeAs<typename function_traits<F>::template args<I>::type>()...
					);
				}
				catch (std::out_of_range)
				{
					throw ArgumentException(sizeof...(I), args.size(), ClassDB::_M_calling);
				};
			}
		};

		static ClassInfo& getInfo() { return _M_classes[_M_bound]; }

		static type call(Object* obj, const string& method_name, arg_list args = {});
		static void set(Object* obj, const string& property_name, const type& value);
		static type get(Object* obj, const string& property_name);

		static ClassInfo& parent(const string& class_name) { return *_M_classes[class_name].parent; }
	};

	template <typename T, typename P>
	struct ClassRegister
	{
		ClassRegister()
		{
			ClassDB::registerClass(T::className(), P::className());
			T::static_register();
		}
	};
} // namespace phi
